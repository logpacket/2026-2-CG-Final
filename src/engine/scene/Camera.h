// 카메라: 어디서 어디를 보는지와 원근 투영.
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// 확장: 시점 조작(궤도 회전, 확대)은 position 과 target 을 바꾸는 컨트롤러를 따로 만들어 붙인다.
struct Camera {
    glm::vec3 position{0.0f, 7.0f, 15.0f};  // 월드 좌표, m
    glm::vec3 target{0.0f, 1.0f, 0.0f};
    float fovYRadians = 0.785398f;  // 45도
    float nearMeters = 0.1f;
    float farMeters = 100.0f;

    glm::mat4 view() const;
    // aspect 는 프레임버퍼의 폭 / 높이다.
    glm::mat4 projection(float aspect) const;
};
