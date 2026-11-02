#include "exhibition/exhibits/Exhibits.h"

#include "core/AssetPath.h"
#include "core/geometry/ObjLoader.h"
#include "core/geometry/Primitives.h"

#include <glm/gtc/quaternion.hpp>

#include <cstdio>
#include <stdexcept>
#include <utility>

namespace {

constexpr float kFallbackBoxMeters = 1.0f;

// 모델 파일이 없거나 깨져도 전시는 계속한다 (CODING_CONVENTION 7: 계속할 수 있는 실패).
MeshData loadMeshOrBox(const std::string& objPath)
{
    try {
        return loadObj(assetPath(objPath));
    } catch (const std::runtime_error& error) {
        std::fprintf(stderr, "[Exhibits] %s (drawing a box instead)\n", error.what());
        return makeBox(glm::vec3(kFallbackBoxMeters));
    }
}

}  // namespace

std::vector<ExhibitSpec> exhibitSpecs()
{
    // 색은 sRGB. 방향광이 하나뿐이라 금속도가 높으면 확산광이 줄어 어둡게 보이므로 metallic 을 낮게 둔다.
    // 부품 사이를 1.2 m 쯤 띄워 x 축을 따라 한 줄로 놓는다.
    // 항목 순서: 경로, {색, metallic, roughness}, 바닥 위 자리 (x, z), y축 회전
    return {
        {"models/gear.obj", {{0.78f, 0.60f, 0.28f}, 0.4f, 0.35f}, {-4.8f, 0.0f}, 0.0f},
        {"models/cranksharft.obj", {{0.55f, 0.56f, 0.60f}, 0.4f, 0.30f}, {-1.4f, 0.0f}, 0.0f},
        {"models/cylinder_block.obj", {{0.42f, 0.47f, 0.55f}, 0.2f, 0.55f}, {1.6f, 0.0f}, 0.0f},
        {"models/piston_assembly.obj", {{0.75f, 0.76f, 0.78f}, 0.3f, 0.40f}, {4.5f, 0.0f}, 0.0f},
        {"models/spark_plug.obj", {{0.92f, 0.91f, 0.88f}, 0.0f, 0.30f}, {6.3f, 0.0f}, 0.0f},
    };
}

std::vector<Exhibit> loadExhibits(const std::vector<ExhibitSpec>& specs)
{
    std::vector<Exhibit> exhibits;
    exhibits.reserve(specs.size());
    for (const ExhibitSpec& spec : specs) {
        Mesh mesh(loadMeshOrBox(spec.objPath));
        Transform transform;
        transform.rotation = glm::angleAxis(spec.yawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        placeOnFloor(transform, mesh.bounds(), spec.floorPositionXZ);
        exhibits.push_back({std::move(mesh), spec.material, transform});
    }
    return exhibits;
}
