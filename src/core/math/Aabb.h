// 축 정렬 경계 상자(AABB): 물체가 차지하는 공간을 가장 작은 점(min)과 가장 큰 점(max)으로 나타낸다.
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <limits>

// 무엇을: 점을 expand() 로 하나씩 넣으면 그 점들을 모두 감싸는 가장 작은 상자가 된다.
//         부품을 바닥에 놓을 때(min.y), 나중에 피킹(광선과 교차)할 때 쓴다.
// 확장:   광선 교차 검사는 Ray 를 만들 때 이 폴더(core/math/)에 함께 추가한다.
// 주의:   기본 생성한 상자는 비어 있다(min 이 max 보다 크다). center()/size() 는 비어 있지 않을 때만 의미가 있다.
struct Aabb {
    glm::vec3 min{std::numeric_limits<float>::max()};
    glm::vec3 max{std::numeric_limits<float>::lowest()};

    // 상자를 point 까지 늘린다. 점 하나만 넣으면 크기 0 인 상자가 된다.
    void expand(const glm::vec3& point);

    bool isEmpty() const;
    glm::vec3 center() const;
    glm::vec3 size() const;

    // 상자의 꼭짓점 8개를 matrix(모델 행렬 같은 아핀 변환)로 옮긴 뒤 다시 감싼 상자.
    // 회전하면 원래보다 커질 수 있다. 빈 상자는 빈 상자 그대로다.
    Aabb transformed(const glm::mat4& matrix) const;
};
