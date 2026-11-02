#include "core/math/Aabb.h"

#include "TestFramework.h"

#include <glm/gtc/matrix_transform.hpp>

namespace {

constexpr double kEpsilon = 1e-5;

}  // namespace

TEST_CASE(defaultAabbIsEmpty)
{
    const Aabb box;
    CHECK(box.isEmpty());
}

TEST_CASE(singlePointMakesZeroSizeBox)
{
    Aabb box;
    box.expand(glm::vec3(1.0f, 2.0f, 3.0f));
    CHECK(!box.isEmpty());
    CHECK_VEC3_NEAR(box.size(), glm::vec3(0.0f), kEpsilon);
    CHECK_VEC3_NEAR(box.center(), glm::vec3(1.0f, 2.0f, 3.0f), kEpsilon);
}

TEST_CASE(expandGrowsToContainAllPoints)
{
    Aabb box;
    box.expand(glm::vec3(1.0f, 2.0f, 3.0f));
    box.expand(glm::vec3(-1.0f, 0.0f, 5.0f));
    CHECK_VEC3_NEAR(box.min, glm::vec3(-1.0f, 0.0f, 3.0f), kEpsilon);
    CHECK_VEC3_NEAR(box.max, glm::vec3(1.0f, 2.0f, 5.0f), kEpsilon);
    CHECK_VEC3_NEAR(box.center(), glm::vec3(0.0f, 1.0f, 4.0f), kEpsilon);
    CHECK_VEC3_NEAR(box.size(), glm::vec3(2.0f, 2.0f, 2.0f), kEpsilon);
}

TEST_CASE(transformedByTranslationMovesBox)
{
    Aabb box;
    box.expand(glm::vec3(-1.0f));
    box.expand(glm::vec3(1.0f));
    const Aabb moved = box.transformed(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    CHECK_VEC3_NEAR(moved.min, glm::vec3(-1.0f, 0.0f, -1.0f), kEpsilon);
    CHECK_VEC3_NEAR(moved.max, glm::vec3(1.0f, 2.0f, 1.0f), kEpsilon);
}

TEST_CASE(transformedByRotationRecomputesBounds)
{
    // x축으로 긴 상자를 y축을 중심으로 90도 돌리면 z축으로 긴 상자가 된다.
    Aabb box;
    box.expand(glm::vec3(-2.0f, -0.5f, -0.5f));
    box.expand(glm::vec3(2.0f, 0.5f, 0.5f));
    const Aabb rotated =
        box.transformed(glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    CHECK_VEC3_NEAR(rotated.min, glm::vec3(-0.5f, -0.5f, -2.0f), kEpsilon);
    CHECK_VEC3_NEAR(rotated.max, glm::vec3(0.5f, 0.5f, 2.0f), kEpsilon);
}

TEST_CASE(transformedEmptyBoxStaysEmpty)
{
    const Aabb box;
    CHECK(box.transformed(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f))).isEmpty());
}
