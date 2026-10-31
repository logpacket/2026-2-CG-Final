// 축 정렬 경계 상자(AABB): 좌표가 가장 작은 꼭짓점(min)과 가장 큰 꼭짓점(max)으로 물체를 감싼다.
// 부품을 바닥에 놓을 때(min.y)와 나중에 피킹(광선 교차 검사)할 때 쓴다.
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <limits>

// 확장: 광선 교차 검사는 Ray 를 만들 때 이 폴더(core/math/)에 함께 추가한다.
// 주의: 기본 생성자로 만든 상자는 비어 있다(min 이 max 보다 크다).
//       center()/size() 는 비어 있지 않을 때만 의미가 있다.
struct Aabb {
    glm::vec3 min{std::numeric_limits<float>::max()};
    glm::vec3 max{std::numeric_limits<float>::lowest()};

    // point 를 포함하도록 상자를 늘린다.
    void expand(const glm::vec3& point);

    bool isEmpty() const;
    glm::vec3 center() const;
    glm::vec3 size() const;

    // 꼭짓점 8개를 matrix(모델 행렬 같은 아핀 변환)로 변환한 뒤 다시 감싼 상자.
    // 회전이 있으면 원래보다 커질 수 있다. 빈 상자는 그대로 빈 상자다.
    Aabb transformed(const glm::mat4& matrix) const;
};
