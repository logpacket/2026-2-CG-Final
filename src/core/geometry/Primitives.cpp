#include "core/geometry/Primitives.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>

#include <array>
#include <cstddef>

namespace {

constexpr std::size_t kBoxFaceCount = 6;
constexpr std::size_t kQuadVertexCount = 4;
constexpr std::size_t kQuadIndexCount = 6;

// 사각형 하나를 삼각형 두 개로 나눠 추가한다.
// right × up 이 normal 과 같은 방향이어야 앞면에서 볼 때 정점이 반시계 순서(CCW)가 된다.
void appendQuad(MeshData& mesh, const glm::vec3& center, const glm::vec3& halfRight, const glm::vec3& halfUp,
                const glm::vec3& normal)
{
    const auto base = static_cast<std::uint32_t>(mesh.vertices.size());
    mesh.vertices.push_back({center - halfRight - halfUp, normal, {0.0f, 0.0f}});
    mesh.vertices.push_back({center + halfRight - halfUp, normal, {1.0f, 0.0f}});
    mesh.vertices.push_back({center + halfRight + halfUp, normal, {1.0f, 1.0f}});
    mesh.vertices.push_back({center - halfRight + halfUp, normal, {0.0f, 1.0f}});
    for (const std::uint32_t corner : {0U, 1U, 2U, 0U, 2U, 3U}) {
        mesh.indices.push_back(base + corner);
    }
}

}  // namespace

MeshData makePlane(float size)
{
    const float half = size * 0.5f;
    MeshData mesh;
    appendQuad(mesh, glm::vec3(0.0f), glm::vec3(half, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -half),
               glm::vec3(0.0f, 1.0f, 0.0f));
    return mesh;
}

MeshData makeBox(const glm::vec3& size)
{
    struct Face {
        glm::vec3 normal;
        glm::vec3 right;
        glm::vec3 up;
    };
    // 면마다 right × up = normal 이 되도록 고른 축
    const std::array<Face, kBoxFaceCount> faces{{
        {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 0.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    }};

    const glm::vec3 half = size * 0.5f;
    MeshData mesh;
    mesh.vertices.reserve(kBoxFaceCount * kQuadVertexCount);
    mesh.indices.reserve(kBoxFaceCount * kQuadIndexCount);
    for (const Face& face : faces) {
        // 축 방향 단위 벡터와 half 를 내적하면 그 축 길이의 절반이 나온다
        const float rightHalf = glm::dot(glm::abs(face.right), half);
        const float upHalf = glm::dot(glm::abs(face.up), half);
        appendQuad(mesh, face.normal * half, face.right * rightHalf, face.up * upHalf, face.normal);
    }
    return mesh;
}
