// MeshData 가 지켜야 할 조건을 검사하는 테스트 도구. 기본 도형과 OBJ 로더 테스트가 함께 쓴다.
#pragma once

#include "core/geometry/MeshData.h"

#include <glm/geometric.hpp>

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace test {

// 인덱스 개수가 3의 배수이고 모든 인덱스가 정점 배열 안을 가리키는지
inline bool indicesAreValidTriangles(const MeshData& mesh)
{
    if (mesh.indices.size() % 3 != 0) {
        return false;
    }
    for (const std::uint32_t index : mesh.indices) {
        if (index >= mesh.vertices.size()) {
            return false;
        }
    }
    return true;
}

// 앞면에서 볼 때 정점이 반시계 순서(CCW)인지: 정점 순서로 구한 면 노멀이 정점 노멀과 같은 쪽을 향해야 한다.
inline bool trianglesWindTowardTheirNormals(const MeshData& mesh)
{
    for (std::size_t i = 0; i + 2 < mesh.indices.size(); i += 3) {
        const Vertex& a = mesh.vertices[mesh.indices[i]];
        const Vertex& b = mesh.vertices[mesh.indices[i + 1]];
        const Vertex& c = mesh.vertices[mesh.indices[i + 2]];
        const glm::vec3 faceNormal = glm::cross(b.position - a.position, c.position - a.position);
        if (glm::dot(faceNormal, a.normal) <= 0.0f) {
            return false;
        }
    }
    return true;
}

inline bool normalsAreUnitLength(const MeshData& mesh)
{
    constexpr float kTolerance = 1e-5f;
    for (const Vertex& vertex : mesh.vertices) {
        if (std::abs(glm::length(vertex.normal) - 1.0f) > kTolerance) {
            return false;
        }
    }
    return true;
}

}  // namespace test
