// MeshData 를 GPU 버퍼(VAO · VBO · EBO)에 올리고 그린다.
#pragma once

#include "core/geometry/MeshData.h"
#include "core/math/Aabb.h"

#include <glad/glad.h>

// 정점 셰이더의 layout(location = N) 과 같아야 한다 (assets/shaders/*.vert).
constexpr GLuint kPositionAttribute = 0;
constexpr GLuint kNormalAttribute = 1;
constexpr GLuint kUvAttribute = 2;

// 주의: GL 객체를 소유하므로 이동만 할 수 있다. GL 컨텍스트가 살아 있는 동안 만들고 소멸시킨다.
class Mesh {
public:
    explicit Mesh(const MeshData& data);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // 셰이더를 bind 한 뒤 부른다.
    void draw() const;

    // 모델 좌표계의 경계 상자
    const Aabb& bounds() const { return bounds_; }

private:
    void release();

    GLuint vao_ = 0;
    GLuint vbo_ = 0;
    GLuint ebo_ = 0;
    GLsizei indexCount_ = 0;
    Aabb bounds_;
};
