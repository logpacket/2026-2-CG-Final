// 표면의 색과 질감. 지금은 Blinn-Phong 이 쓰고, PBR 로 바꿔도 같은 값을 쓴다.
#pragma once

#include <glm/vec3.hpp>

struct Material {
    glm::vec3 baseColor{0.8f};  // 화면에 보이는 색(sRGB), 0 ~ 1
    float metallic = 0.0f;      // 0 = 비금속, 1 = 금속. 금속일수록 반사광이 baseColor 를 띤다
    float roughness = 0.5f;     // 0 ~ 1. 거칠수록 하이라이트가 넓고 흐려진다
};
