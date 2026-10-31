// MeshData 도우미 함수 구현.
#include "core/geometry/MeshData.h"

Aabb computeBounds(const MeshData& mesh)
{
    Aabb bounds;
    for (const Vertex& vertex : mesh.vertices) {
        bounds.expand(vertex.position);
    }
    return bounds;
}
