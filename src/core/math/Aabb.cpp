// Aabb 구현.
#include "core/math/Aabb.h"

#include <glm/common.hpp>
#include <glm/vec4.hpp>

namespace {

constexpr unsigned int kCornerCount = 8;

}  // namespace

void Aabb::expand(const glm::vec3& point)
{
    min = glm::min(min, point);
    max = glm::max(max, point);
}

bool Aabb::isEmpty() const
{
    return min.x > max.x || min.y > max.y || min.z > max.z;
}

glm::vec3 Aabb::center() const
{
    return (min + max) * 0.5f;
}

glm::vec3 Aabb::size() const
{
    return max - min;
}

Aabb Aabb::transformed(const glm::mat4& matrix) const
{
    Aabb result;
    if (isEmpty()) {
        return result;
    }
    for (unsigned int i = 0; i < kCornerCount; ++i) {
        // i 의 비트 0/1/2 가 x/y/z 에서 min 과 max 중 무엇을 고를지 정한다.
        const glm::vec3 corner((i & 1U) != 0 ? max.x : min.x, (i & 2U) != 0 ? max.y : min.y,
                               (i & 4U) != 0 ? max.z : min.z);
        result.expand(glm::vec3(matrix * glm::vec4(corner, 1.0f)));
    }
    return result;
}
