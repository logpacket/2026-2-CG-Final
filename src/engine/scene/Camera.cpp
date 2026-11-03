#include "engine/scene/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::view() const
{
    return glm::lookAt(position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::projection(float aspect) const
{
    return glm::perspective(fovYRadians, aspect, nearMeters, farMeters);
}
