// 장면 조명: 방향광 하나와 앰비언트. 값만 담은 구조체라 ImGui 패널에 바로 연결할 수 있다.
#pragma once

#include <glm/vec3.hpp>

// 해처럼 아주 멀리 있어 모든 곳을 같은 방향으로 비추는 빛
struct DirectionalLight {
    glm::vec3 direction{-0.5f, -1.0f, -0.35f};  // 빛이 나아가는 방향(월드). 길이는 상관없다
    glm::vec3 color{1.0f, 0.97f, 0.92f};        // sRGB, 0 ~ 1
    float intensity = 1.0f;
};

struct Lighting {
    DirectionalLight directional;
    glm::vec3 ambientColor{0.35f, 0.37f, 0.42f};  // 빛이 닿지 않는 면도 이만큼은 보인다 (sRGB)
};
