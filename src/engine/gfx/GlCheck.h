// Debug 빌드에서 쌓인 GL 오류를 꺼내 출력한다. Release 에서는 아무 일도 하지 않는다.
#pragma once

// GL 오류 플래그는 꺼낼 때까지 남아 있으므로 호출마다 감싸지 않고 구간 끝에서 한 번 부른다.
#ifdef NDEBUG
#define VE_GL_CHECK() static_cast<void>(0)
#else
#define VE_GL_CHECK() checkGlErrors(__FILE__, __LINE__)
#endif

// 같은 위치의 오류는 한 번만 출력한다 (매 프레임 반복되는 오류로 콘솔이 넘치지 않게).
void checkGlErrors(const char* file, int line);
