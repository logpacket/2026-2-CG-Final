#include "engine/gfx/GlCheck.h"

#include <glad/glad.h>

#include <cstdio>
#include <set>
#include <string>
#include <utility>

namespace {

const char* errorName(GLenum error)
{
    switch (error) {
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "unknown GL error";
    }
}

// 로그에는 경로 없이 파일 이름만 쓴다.
const char* fileName(const char* path)
{
    const char* name = path;
    for (const char* c = path; *c != '\0'; ++c) {
        if (*c == '/' || *c == '\\') {
            name = c + 1;
        }
    }
    return name;
}

}  // namespace

void checkGlErrors(const char* file, int line)
{
    // 이미 출력한 위치. 프로그램 내내 남아야 해서 함수 안 static 으로 둔다 (CODING_CONVENTION 9.4 의 예외).
    static std::set<std::pair<std::string, int>> reported;
    for (GLenum error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
        if (reported.emplace(file, line).second) {
            std::fprintf(stderr, "[GL] %s at %s:%d\n", errorName(error), fileName(file), line);
        }
    }
}
