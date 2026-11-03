#include "core/geometry/ObjLoader.h"

#include "core/AssetPath.h"
#include "core/geometry/MeshData.h"
#include "MeshChecks.h"
#include "TestFramework.h"

#include <cstdint>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr double kEpsilon = 1e-5;

// z = 0 평면에 놓인 삼각형의 꼭짓점. 1 → 2 → 3 순서가 +Z 에서 볼 때 반시계다.
const std::string kTrianglePositions = "v 0 0 0\nv 1 0 0\nv 0 1 0\n";

MeshData parse(const std::string& text)
{
    std::istringstream in(text);
    return parseObj(in, "test.obj");
}

}  // namespace

TEST_CASE(triangleKeepsPositionsAndFileNormals)
{
    const MeshData mesh = parse(kTrianglePositions + "vn 0 0 1\nf 1//1 2//1 3//1\n");
    CHECK(mesh.vertices.size() == 3);
    CHECK(mesh.indices.size() == 3);
    CHECK_VEC3_NEAR(mesh.vertices.at(1).position, glm::vec3(1.0f, 0.0f, 0.0f), kEpsilon);
    CHECK_VEC3_NEAR(mesh.vertices.at(2).normal, glm::vec3(0.0f, 0.0f, 1.0f), kEpsilon);
    CHECK_VEC3_NEAR(glm::vec3(mesh.vertices.at(0).uv, 0.0f), glm::vec3(0.0f), kEpsilon);
}

TEST_CASE(quadIsSplitIntoTwoTriangles)
{
    const MeshData mesh = parse("v 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\nvn 0 0 1\nf 1//1 2//1 3//1 4//1\n");
    CHECK(mesh.vertices.size() == 4);
    CHECK(mesh.indices == std::vector<std::uint32_t>({0, 1, 2, 0, 2, 3}));
}

TEST_CASE(uvIsReadWithAndWithoutNormals)
{
    const std::string uvs = "vt 0 0\nvt 1 0\nvt 0.25 0.75\nvn 0 0 1\n";
    const MeshData full = parse(kTrianglePositions + uvs + "f 1/1/1 2/2/1 3/3/1\n");
    const MeshData uvOnly = parse(kTrianglePositions + uvs + "f 1/1 2/2 3/3\n");
    CHECK_NEAR(full.vertices.at(2).uv.x, 0.25, kEpsilon);
    CHECK_NEAR(full.vertices.at(2).uv.y, 0.75, kEpsilon);
    CHECK_NEAR(uvOnly.vertices.at(1).uv.x, 1.0, kEpsilon);
}

TEST_CASE(negativeIndicesCountBackFromLatest)
{
    const MeshData mesh = parse("v 9 9 9\n" + kTrianglePositions + "f -3 -2 -1\n");
    CHECK_VEC3_NEAR(mesh.vertices.at(0).position, glm::vec3(0.0f, 0.0f, 0.0f), kEpsilon);
    CHECK_VEC3_NEAR(mesh.vertices.at(2).position, glm::vec3(0.0f, 1.0f, 0.0f), kEpsilon);
}

TEST_CASE(missingNormalsUseFaceNormalFromWinding)
{
    const MeshData front = parse(kTrianglePositions + "f 1 2 3\n");
    const MeshData back = parse(kTrianglePositions + "f 3 2 1\n");
    CHECK_VEC3_NEAR(front.vertices.at(0).normal, glm::vec3(0.0f, 0.0f, 1.0f), kEpsilon);
    CHECK_VEC3_NEAR(back.vertices.at(0).normal, glm::vec3(0.0f, 0.0f, -1.0f), kEpsilon);
    CHECK(test::trianglesWindTowardTheirNormals(front));
    CHECK(test::trianglesWindTowardTheirNormals(back));
}

TEST_CASE(identicalCornersAreMergedOnlyWhenNormalsMatch)
{
    const std::string square = "v 0 0 0\nv 1 0 0\nv 1 1 0\nv 0 1 0\nvn 0 0 1\nvn 0 0 -1\n";
    // 앞 두 면은 꼭짓점 1, 3 을 공유한다. 세 번째 면은 같은 위치지만 노멀이 달라 새 정점이 된다.
    const MeshData withNormals = parse(square + "f 1//1 2//1 3//1\nf 1//1 3//1 4//1\nf 1//2 4//2 3//2\n");
    CHECK(withNormals.vertices.size() == 7);
    CHECK(withNormals.indices.size() == 9);

    const MeshData withoutNormals = parse(square + "f 1 2 3\nf 1 3 4\n");
    CHECK(withoutNormals.vertices.size() == 6);
}

TEST_CASE(crlfCommentsAndUnusedKeywordsAreIgnored)
{
    const MeshData mesh = parse("# Blender\r\nmtllib part.mtl\r\no Part\r\nv 0 0 0\r\nv 1 0 0  # x\r\n"
                                "v 0 1 0\r\nvn 0 0 1\r\ns 1\r\nusemtl Steel\r\ng body\r\n\r\n"
                                "f 1//1 2//1 3//1\r\nl 1 2\r\n");
    CHECK(mesh.vertices.size() == 3);
    CHECK(mesh.indices.size() == 3);
    CHECK_VEC3_NEAR(mesh.vertices.at(1).position, glm::vec3(1.0f, 0.0f, 0.0f), kEpsilon);
}

TEST_CASE(malformedInputThrows)
{
    CHECK_THROWS(parse(kTrianglePositions + "f 1 2 4\n"));           // 범위 밖
    CHECK_THROWS(parse(kTrianglePositions + "f 0 1 2\n"));           // OBJ 인덱스에 0 은 없다
    CHECK_THROWS(parse(kTrianglePositions + "f -4 1 2\n"));          // 음수도 범위 밖
    CHECK_THROWS(parse(kTrianglePositions + "f 1 2\n"));             // 꼭짓점 2개
    CHECK_THROWS(parse(kTrianglePositions + "f 1/1 2/1 3/1\n"));     // vt 가 없다
    CHECK_THROWS(parse(kTrianglePositions + "f 1//1 2//1 3//1\n"));  // vn 이 없다
    CHECK_THROWS(parse(kTrianglePositions + "f 1 2 3a\n"));
    CHECK_THROWS(parse("v 1 x 2\n"));
    CHECK_THROWS(parse("v 1 2\n"));
}

TEST_CASE(errorMessageNamesFileAndLine)
{
    std::string message;
    try {
        static_cast<void>(parse("v 0 0 0\nv 1 0 0\nf 1 2 3\n"));
    } catch (const std::runtime_error& error) {
        message = error.what();
    }
    CHECK(message.rfind("test.obj:3: ", 0) == 0);
}

TEST_CASE(missingFileThrows)
{
    CHECK_THROWS(loadObj(assetPath("models/no_such_part.obj")));
}

// assets/models 의 모든 OBJ 가 읽히는지 확인한다. 모델링 담당이 내보낸 파일의 형식 문제를 여기서 잡는다.
TEST_CASE(assetModelsLoadAsValidMeshes)
{
    int modelCount = 0;
    for (const std::filesystem::directory_entry& entry :
         std::filesystem::directory_iterator(assetPath("models"))) {
        if (entry.path().extension() != ".obj") {
            continue;
        }
        ++modelCount;
        const MeshData mesh = loadObj(entry.path().string());
        if (mesh.indices.empty() || !test::indicesAreValidTriangles(mesh) ||
            !test::normalsAreUnitLength(mesh)) {
            test::reportFailure(__FILE__, __LINE__,
                                entry.path().filename().string() + " is not a valid mesh");
        }
    }
    CHECK(modelCount > 0);
}
