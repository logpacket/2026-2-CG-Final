#include "core/geometry/Primitives.h"

#include "core/geometry/MeshData.h"
#include "TestFramework.h"

#include <glm/geometric.hpp>

#include <cmath>
#include <cstddef>

namespace {

constexpr double kEpsilon = 1e-5;

bool indicesAreValidTriangles(const MeshData& mesh)
{
    if (mesh.indices.size() % 3 != 0) {
        return false;
    }
    for (const std::uint32_t index : mesh.indices) {
        if (index >= mesh.vertices.size()) {
            return false;
        }
    }
    return true;
}

// 앞면에서 볼 때 정점이 반시계 순서(CCW)인지: 정점 순서로 구한 면 노멀이 정점 노멀과 같은 쪽을 향해야 한다.
bool trianglesWindTowardTheirNormals(const MeshData& mesh)
{
    for (std::size_t i = 0; i + 2 < mesh.indices.size(); i += 3) {
        const Vertex& a = mesh.vertices[mesh.indices[i]];
        const Vertex& b = mesh.vertices[mesh.indices[i + 1]];
        const Vertex& c = mesh.vertices[mesh.indices[i + 2]];
        const glm::vec3 faceNormal = glm::cross(b.position - a.position, c.position - a.position);
        if (glm::dot(faceNormal, a.normal) <= 0.0f) {
            return false;
        }
    }
    return true;
}

bool normalsAreUnitLength(const MeshData& mesh)
{
    for (const Vertex& vertex : mesh.vertices) {
        if (std::abs(glm::length(vertex.normal) - 1.0f) > kEpsilon) {
            return false;
        }
    }
    return true;
}

}  // namespace

TEST_CASE(emptyMeshHasEmptyBounds)
{
    const MeshData mesh;
    CHECK(computeBounds(mesh).isEmpty());
}

TEST_CASE(planeLiesFlatOnFloorFacingUp)
{
    const MeshData plane = makePlane(4.0f);
    CHECK(plane.vertices.size() == 4);
    CHECK(plane.indices.size() == 6);

    const Aabb bounds = computeBounds(plane);
    CHECK_VEC3_NEAR(bounds.min, glm::vec3(-2.0f, 0.0f, -2.0f), kEpsilon);
    CHECK_VEC3_NEAR(bounds.max, glm::vec3(2.0f, 0.0f, 2.0f), kEpsilon);
    for (const Vertex& vertex : plane.vertices) {
        CHECK_VEC3_NEAR(vertex.normal, glm::vec3(0.0f, 1.0f, 0.0f), kEpsilon);
    }
}

TEST_CASE(boxHasRequestedSizeAroundOrigin)
{
    const MeshData box = makeBox(glm::vec3(2.0f, 1.0f, 0.5f));
    CHECK(box.vertices.size() == 24);  // 면 6개 × 정점 4개
    CHECK(box.indices.size() == 36);   // 면 6개 × 삼각형 2개 × 3

    const Aabb bounds = computeBounds(box);
    CHECK_VEC3_NEAR(bounds.min, glm::vec3(-1.0f, -0.5f, -0.25f), kEpsilon);
    CHECK_VEC3_NEAR(bounds.max, glm::vec3(1.0f, 0.5f, 0.25f), kEpsilon);
}

TEST_CASE(primitivesHaveValidCounterClockwiseTriangles)
{
    const MeshData plane = makePlane(1.0f);
    const MeshData box = makeBox(glm::vec3(1.0f, 2.0f, 3.0f));
    CHECK(indicesAreValidTriangles(plane));
    CHECK(indicesAreValidTriangles(box));
    CHECK(trianglesWindTowardTheirNormals(plane));
    CHECK(trianglesWindTowardTheirNormals(box));
    CHECK(normalsAreUnitLength(plane));
    CHECK(normalsAreUnitLength(box));
}
