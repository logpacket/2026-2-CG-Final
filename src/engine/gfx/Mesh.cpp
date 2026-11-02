#include "engine/gfx/Mesh.h"

#include "engine/gfx/GlCheck.h"

#include <cstddef>
#include <cstdint>
#include <utility>

namespace {

// glVertexAttribPointer 는 버퍼 안의 바이트 위치를 포인터 타입으로 받는다 (옛 API 의 흔적).
const void* bufferOffset(std::size_t bytes)
{
    return reinterpret_cast<const void*>(bytes);  // NOLINT(performance-no-int-to-ptr)
}

void setFloatAttribute(GLuint location, GLint components, std::size_t offset)
{
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(sizeof(Vertex)),
                          bufferOffset(offset));
}

}  // namespace

Mesh::Mesh(const MeshData& data)
    : indexCount_(static_cast<GLsizei>(data.indices.size())), bounds_(computeBounds(data))
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.vertices.size() * sizeof(Vertex)),
                 data.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(data.indices.size() * sizeof(std::uint32_t)), data.indices.data(),
                 GL_STATIC_DRAW);

    setFloatAttribute(kPositionAttribute, 3, offsetof(Vertex, position));
    setFloatAttribute(kNormalAttribute, 3, offsetof(Vertex, normal));
    setFloatAttribute(kUvAttribute, 2, offsetof(Vertex, uv));

    // EBO 바인딩은 VAO 에 기록되므로 0 으로 바꾸지 않고 VAO 만 해제한다.
    glBindVertexArray(0);
    VE_GL_CHECK();
}

Mesh::~Mesh()
{
    release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_(std::exchange(other.vao_, 0)), vbo_(std::exchange(other.vbo_, 0)),
      ebo_(std::exchange(other.ebo_, 0)), indexCount_(std::exchange(other.indexCount_, 0)),
      bounds_(other.bounds_)
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other) {
        release();
        vao_ = std::exchange(other.vao_, 0);
        vbo_ = std::exchange(other.vbo_, 0);
        ebo_ = std::exchange(other.ebo_, 0);
        indexCount_ = std::exchange(other.indexCount_, 0);
        bounds_ = other.bounds_;
    }
    return *this;
}

void Mesh::draw() const
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, nullptr);
}

// glDelete*(0) 은 아무 일도 하지 않으므로 이동한 뒤의 빈 객체도 그대로 부를 수 있다.
void Mesh::release()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}
