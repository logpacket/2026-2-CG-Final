// 물체의 위치 · 회전 · 크기와, 그것을 합친 모델 행렬.
#pragma once

#include "core/math/Aabb.h"

#include <glm/ext/quaternion_float.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Transform {
    glm::vec3 position{0.0f};                    // 월드 좌표, m
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};  // 단위 쿼터니언 (w, x, y, z)
    glm::vec3 scale{1.0f};

    // T · R · S: 크기 → 회전 → 이동 순서로 적용된다.
    glm::mat4 matrix() const;
};

// 회전과 크기는 그대로 두고 위치만 바꿔 물체를 바닥(y = 0)에 놓는다.
// 월드 경계 상자의 가장 낮은 점이 y = 0 에, 가로 중심이 floorPositionXZ (x, z) 에 온다.
// 모델의 원점이 물체 가운데에 있지 않아도 된다. localBounds 는 모델 좌표계의 경계 상자다.
void placeOnFloor(Transform& transform, const Aabb& localBounds, const glm::vec2& floorPositionXZ);
