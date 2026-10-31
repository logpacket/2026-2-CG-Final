// 프로그램 진입점: App 을 만들어 실행한다.
#include "app/App.h"

#include <cstdio>
#include <exception>

int main()
{
    try {
        App app;
        app.run();
    } catch (const std::exception& e) {
        // 창 생성 실패 같은 치명적인 오류는 여기서 출력하고 끝낸다.
        std::fprintf(stderr, "[Fatal] %s\n", e.what());
        return 1;
    }
    return 0;
}
