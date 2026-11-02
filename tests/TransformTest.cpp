#include "engine/scene/Transform.h"

#include "core/math/Aabb.h"
#include "TestFramework.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec4.hpp>

namespace {

constexpr double kEpsilon = 1e-5;
constexpr float kQuarterTurnRadians = 1.5707963f;

glm::vec3 apply(const glm::mat4& matrix, const glm::vec3& point)
{
    return glm::vec3(matrix * glm::vec4(point, 1.0f));
}

Aabb boxFrom(const glm::vec3& min, const glm::vec3& max)
{
    Aabb box;
    box.expand(min);
    box.expand(max);
    return box;
}

}  // namespace

TEST_CASE(defaultTransformIsIdentity)
{
    const Transform transform;
    CHECK_VEC3_NEAR(apply(transform.matrix(), glm::vec3(1.0f, 2.0f, 3.0f)), glm::vec3(1.0f, 2.0f, 3.0f),
                    kEpsilon);
}

TEST_CASE(matrixScalesThenRotatesThenTranslates)
{
    Transform transform;
    transform.scale = glm::vec3(2.0f);
    transform.rotation = glm::angleAxis(kQuarterTurnRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    transform.position = glm::vec3(1.0f, 2.0f, 3.0f);
    // (1, 0, 0) → 크기 2배 (2, 0, 0) → y축 90도 (0, 0, -2) → 이동 (1, 2, 1)
    CHECK_VEC3_NEAR(apply(transform.matrix(), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 2.0f, 1.0f),
                    kEpsilon);
}

TEST_CASE(placeOnFloorPutsLowestPointOnFloorAtRequestedSpot)
{
    // 원점에서 떨어져 공중에 뜬 모델 (assets 의 점화 플러그처럼)
    const Aabb local = boxFrom(glm::vec3(4.0f, 2.0f, -1.0f), glm::vec3(6.0f, 5.0f, 1.0f));
    Transform transform;
    placeOnFloor(transform, local, glm::vec2(-3.0f, 7.0f));

    const Aabb placed = local.transformed(transform.matrix());
    CHECK_NEAR(placed.min.y, 0.0, kEpsilon);
    CHECK_NEAR(placed.center().x, -3.0, kEpsilon);
    CHECK_NEAR(placed.center().z, 7.0, kEpsilon);
}

TEST_CASE(placeOnFloorKeepsRotationAndScale)
{
    // x축으로 긴 막대를 z축을 중심으로 90도 세우고 절반으로 줄이면 높이 2 m 가 된다.
    const Aabb local = boxFrom(glm::vec3(-2.0f, -0.5f, -0.5f), glm::vec3(2.0f, 0.5f, 0.5f));
    Transform transform;
    transform.rotation = glm::angleAxis(kQuarterTurnRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    transform.scale = glm::vec3(0.5f);
    placeOnFloor(transform, local, glm::vec2(0.0f));

    const Aabb placed = local.transformed(transform.matrix());
    CHECK_NEAR(placed.min.y, 0.0, kEpsilon);
    CHECK_NEAR(placed.max.y, 2.0, kEpsilon);
    CHECK_VEC3_NEAR(transform.scale, glm::vec3(0.5f), kEpsilon);
}

TEST_CASE(placeOnFloorWithEmptyBoundsUsesFloorSpot)
{
    Transform transform;
    placeOnFloor(transform, Aabb{}, glm::vec2(2.0f, -1.0f));
    CHECK_VEC3_NEAR(transform.position, glm::vec3(2.0f, 0.0f, -1.0f), kEpsilon);
}
