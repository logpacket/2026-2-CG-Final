#include "engine/scene/Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 Transform::matrix() const
{
    const glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
    const glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    return translation * rotationMatrix * scaleMatrix;
}

void placeOnFloor(Transform& transform, const Aabb& localBounds, const glm::vec2& floorPositionXZ)
{
    transform.position = glm::vec3(0.0f);
    const Aabb bounds = localBounds.transformed(transform.matrix());
    if (bounds.isEmpty()) {
        transform.position = glm::vec3(floorPositionXZ.x, 0.0f, floorPositionXZ.y);
        return;
    }
    const glm::vec3 center = bounds.center();
    transform.position = glm::vec3(floorPositionXZ.x - center.x, -bounds.min.y, floorPositionXZ.y - center.z);
}
