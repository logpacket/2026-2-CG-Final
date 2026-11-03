#include "engine/gfx/Shader.h"

#include "engine/gfx/GlCheck.h"

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {

std::string readFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error(path + ": cannot open shader file");
    }
    std::ostringstream text;
    text << file.rdbuf();
    return text.str();
}

std::string fileName(const std::string& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

std::string shaderLog(GLuint shader)
{
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length), '\0');
    if (length > 0) {
        glGetShaderInfoLog(shader, length, nullptr, log.data());
    }
    return log;
}

std::string programLog(GLuint program)
{
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length), '\0');
    if (length > 0) {
        glGetProgramInfoLog(program, length, nullptr, log.data());
    }
    return log;
}

GLuint compileStage(GLenum stage, const std::string& path)
{
    const std::string source = readFile(path);
    const char* text = source.c_str();
    const GLuint shader = glCreateShader(stage);
    glShaderSource(shader, 1, &text, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        const std::string log = shaderLog(shader);
        glDeleteShader(shader);
        throw std::runtime_error(path + ": shader compile failed\n" + log);
    }
    return shader;
}

GLuint linkProgram(GLuint vertex, GLuint fragment, const std::string& name)
{
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    // 링크가 끝나면 셰이더 객체는 필요 없다.
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        const std::string log = programLog(program);
        glDeleteProgram(program);
        throw std::runtime_error(name + ": shader link failed\n" + log);
    }
    return program;
}

}  // namespace

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : name_(fileName(vertexPath) + " + " + fileName(fragmentPath))
{
    const GLuint vertex = compileStage(GL_VERTEX_SHADER, vertexPath);
    GLuint fragment = 0;
    try {
        fragment = compileStage(GL_FRAGMENT_SHADER, fragmentPath);
    } catch (...) {
        glDeleteShader(vertex);
        throw;
    }
    program_ = linkProgram(vertex, fragment, name_);
    findUniforms();
    VE_GL_CHECK();
}

Shader::~Shader()
{
    glDeleteProgram(program_);
}

Shader::Shader(Shader&& other) noexcept
    : program_(std::exchange(other.program_, 0)), name_(std::move(other.name_)),
      uniformLocations_(std::move(other.uniformLocations_)), warnedNames_(std::move(other.warnedNames_))
{}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other) {
        glDeleteProgram(program_);
        program_ = std::exchange(other.program_, 0);
        name_ = std::move(other.name_);
        uniformLocations_ = std::move(other.uniformLocations_);
        warnedNames_ = std::move(other.warnedNames_);
    }
    return *this;
}

void Shader::bind() const
{
    glUseProgram(program_);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    glProgramUniformMatrix4fv(program_, location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
    glProgramUniformMatrix3fv(program_, location(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glProgramUniform3fv(program_, location(name), 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, float value) const
{
    glProgramUniform1f(program_, location(name), value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glProgramUniform1i(program_, location(name), value);
}

// glGetUniformLocation 은 문자열 검색이라 매 프레임 부르지 않도록 링크 직후 모두 찾아 둔다.
void Shader::findUniforms()
{
    GLint count = 0;
    glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &count);
    GLint maxLength = 0;
    glGetProgramiv(program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
    std::string buffer(static_cast<std::size_t>(maxLength), '\0');
    for (GLint i = 0; i < count; ++i) {
        GLsizei length = 0;
        GLint size = 0;
        GLenum type = 0;
        glGetActiveUniform(program_, static_cast<GLuint>(i), maxLength, &length, &size, &type, buffer.data());
        const std::string uniformName = buffer.substr(0, static_cast<std::size_t>(length));
        uniformLocations_.emplace_back(uniformName, glGetUniformLocation(program_, uniformName.c_str()));
    }
}

GLint Shader::location(const std::string& name) const
{
    for (const auto& [uniformName, uniformLocation] : uniformLocations_) {
        if (uniformName == name) {
            return uniformLocation;
        }
    }
    // 셰이더가 쓰지 않는 uniform 은 컴파일러가 지운다. 오류는 아니지만 이름 오타일 수 있다.
    if (std::find(warnedNames_.begin(), warnedNames_.end(), name) == warnedNames_.end()) {
        warnedNames_.push_back(name);
        std::fprintf(stderr, "[Shader] %s: uniform '%s' is not used\n", name_.c_str(), name.c_str());
    }
    return -1;  // glProgramUniform* 은 위치 -1 을 받으면 아무것도 하지 않는다
}
