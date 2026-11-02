#include "core/geometry/Primitives.h"

#include "core/geometry/MeshData.h"
#include "MeshChecks.h"
#include "TestFramework.h"

namespace {

constexpr double kEpsilon = 1e-5;

}  // namespace

TEST_CASE(emptyMeshHasEmptyBounds)
{
    const MeshData mesh;
    CHECK(computeBounds(mesh).isEmpty());
}

TEST_CASE(planeLiesFlatOnFloorFacingUp)
{
    const MeshData plane = makePlane(4.0f);
    CHECK(plane.vertices.size() == 4);
    CHECK(plane.indices.size() == 6);

    const Aabb bounds = computeBounds(plane);
    CHECK_VEC3_NEAR(bounds.min, glm::vec3(-2.0f, 0.0f, -2.0f), kEpsilon);
    CHECK_VEC3_NEAR(bounds.max, glm::vec3(2.0f, 0.0f, 2.0f), kEpsilon);
    for (const Vertex& vertex : plane.vertices) {
        CHECK_VEC3_NEAR(vertex.normal, glm::vec3(0.0f, 1.0f, 0.0f), kEpsilon);
    }
}

TEST_CASE(boxHasRequestedSizeAroundOrigin)
{
    const MeshData box = makeBox(glm::vec3(2.0f, 1.0f, 0.5f));
    CHECK(box.vertices.size() == 24);  // 면 6개 × 정점 4개
    CHECK(box.indices.size() == 36);   // 면 6개 × 삼각형 2개 × 3

    const Aabb bounds = computeBounds(box);
    CHECK_VEC3_NEAR(bounds.min, glm::vec3(-1.0f, -0.5f, -0.25f), kEpsilon);
    CHECK_VEC3_NEAR(bounds.max, glm::vec3(1.0f, 0.5f, 0.25f), kEpsilon);
}

TEST_CASE(primitivesHaveValidCounterClockwiseTriangles)
{
    const MeshData plane = makePlane(1.0f);
    const MeshData box = makeBox(glm::vec3(1.0f, 2.0f, 3.0f));
    CHECK(test::indicesAreValidTriangles(plane));
    CHECK(test::indicesAreValidTriangles(box));
    CHECK(test::trianglesWindTowardTheirNormals(plane));
    CHECK(test::trianglesWindTowardTheirNormals(box));
    CHECK(test::normalsAreUnitLength(plane));
    CHECK(test::normalsAreUnitLength(box));
}
