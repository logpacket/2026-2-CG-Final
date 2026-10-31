// 메시 데이터: GPU 에 올리기 전 CPU 메모리에 있는 정점과 삼각형 목록. OBJ 로더와 기본 도형 함수가 만든다.
#pragma once

#include "core/math/Aabb.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>
#include <vector>

// 정점 하나. 멤버 순서(position → normal → uv)가 셰이더 입력 location 0/1/2 와 맞물리므로 바꾸지 않는다.
struct Vertex {
    glm::vec3 position{0.0f};
    glm::vec3 normal{0.0f, 1.0f, 0.0f};  // 단위 벡터
    glm::vec2 uv{0.0f};                  // 텍스처 좌표. 파일에 없으면 (0, 0)
};

// GPU 버퍼에 그대로 복사하므로 float 8개가 빈틈없이 붙어 있어야 한다.
static_assert(sizeof(Vertex) == 8 * sizeof(float), "Vertex must be tightly packed for GPU upload");

// 무엇을: 정점 배열과 삼각형 인덱스(3개가 삼각형 하나)를 담는다. GL 과 무관해서 단위 테스트로 검사할 수 있다.
// 확장:   노멀맵처럼 정점마다 값이 더 필요하면 Vertex 에 필드를 추가하고, Mesh 업로드와 셰이더도 함께 바꾼다.
// 주의:   삼각형은 앞면에서 볼 때 반시계 방향(CCW)으로 감긴다. 렌더러의 뒷면 컬링이 이 규칙에 의존한다.
//         좌표계는 오른손, Y-up, 단위 1 = 1 m 이다.
struct MeshData {
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
};

// 모든 정점 위치를 감싸는 상자. 정점이 없으면 빈 상자.
Aabb computeBounds(const MeshData& mesh);
