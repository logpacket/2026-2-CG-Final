// vert/frag 셰이더 파일 두 개로 GL 프로그램을 만들고 uniform 값을 넣는다.
#pragma once

#include <glad/glad.h>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <utility>
#include <vector>

// 주의: 컴파일·링크에 실패하면 GL 로그를 담아 예외를 던진다. GL 프로그램을 소유하므로 이동만 할 수 있다.
//       uniform 위치는 링크한 뒤 한 번에 찾아 둔다. 셰이더가 쓰지 않는 이름은 경고를 한 번 출력하고 무시한다.
class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    // 그리기 전에 부른다. set* 은 bind 하지 않아도 이 프로그램에 값을 넣는다 (glProgramUniform).
    void bind() const;

    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;

private:
    void findUniforms();
    GLint location(const std::string& name) const;

    GLuint program_ = 0;
    std::string name_;  // 로그용: "basic.vert + basic.frag"
    // 해시 컨테이너 대신 vector 를 쓴다. MSVC 의 unordered_map 은 이동할 때도 할당할 수 있어 noexcept 이동이 깨진다.
    // uniform 은 셰이더마다 수십 개 이하라 순서대로 찾아도 충분하다.
    std::vector<std::pair<std::string, GLint>> uniformLocations_;
    mutable std::vector<std::string> warnedNames_;  // 없는 uniform 경고를 한 번만 출력하려고 기억한다
};
