// 메시 데이터: GPU 에 올리기 전 CPU 메모리에 있는 정점과 삼각형 목록. OBJ 로더와 기본 도형 함수가 만든다.
#pragma once

#include "core/math/Aabb.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>
#include <vector>

// 멤버 순서(position → normal → uv)가 셰이더 입력 location 0/1/2 에 대응하므로 바꾸지 않는다.
struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f, 1.0f, 0.0f};  // 단위 벡터
    glm::vec2 uv{0.0f};                  // 파일에 없으면 (0, 0)
};

static_assert(sizeof(Vertex) == 8 * sizeof(float), "Vertex must be tightly packed for GPU upload");

// 확장: 노멀맵용 탄젠트처럼 정점마다 값이 더 필요하면 Vertex 에 필드를 추가한다.
//       Mesh 업로드와 셰이더도 함께 바꾼다.
// 주의: indices 는 3개씩 삼각형 하나를 이루고, 앞면에서 볼 때 정점이 반시계 순서(CCW)로 놓인다.
//       렌더러는 이 순서로 앞뒷면을 구분해 뒷면을 그리지 않는다.
//       좌표계는 오른손 좌표계이고 Y축이 위, 길이 1 = 1 m 이다.
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
};

// 정점이 없으면 빈 상자를 돌려준다.
Aabb computeBounds(const MeshData& mesh);
