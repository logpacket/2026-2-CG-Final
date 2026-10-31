// 아주 작은 단위 테스트 도구. 허용 라이브러리 밖의 테스트 프레임워크를 쓰지 않기 위해 직접 만들었다.
#pragma once

#include <cmath>
#include <string>
#include <vector>

// 사용법: tests/<모듈이름>Test.cpp 를 만들고 아래처럼 쓴다. (CMake 가 자동으로 포함한다)
//
//   #include "core/FrameTimer.h"   // 테스트할 대상 (clang-format 이 맨 위로 올린다)
//
//   #include "TestFramework.h"
//
//   TEST_CASE(firstTickHasZeroDelta)
//   {
//       FrameTimer timer;
//       timer.tick(5.0);
//       CHECK(timer.deltaSeconds() == 0.0f);
//       CHECK_NEAR(timer.fps(), 0.0, 1e-6);
//   }
//
// 실행: ctest --preset <프리셋>            모든 테스트
//       <빌드 폴더>/bin/unit_tests Timer    이름에 "Timer" 가 들어간 테스트만
//
// 규칙: 테스트는 창이나 GPU 없이 돌아가야 한다(CI 서버에는 화면이 없다).
//       GL 을 쓰는 코드는 계산 부분을 따로 함수로 빼서 그 함수를 테스트한다.
// 확장: 비교 매크로가 더 필요하면 아래 CHECK_NEAR 를 본떠 추가한다.

namespace test {

using TestFunction = void (*)();

struct TestCase {
    const char* name;
    TestFunction function;
};

// 등록된 모든 테스트 (TEST_CASE 가 프로그램 시작 전에 여기에 넣는다)
std::vector<TestCase>& registry();

// CHECK 가 실패하면 호출된다. 실패를 기록하고 메시지를 출력한다.
void reportFailure(const char* file, int line, const std::string& message);

struct Registrar {
    Registrar(const char* name, TestFunction function) { registry().push_back({name, function}); }
};

}  // namespace test

// 테스트 함수 하나를 정의하고 자동으로 등록한다. name 은 camelCase 식별자로 쓴다.
#define TEST_CASE(name)                                                                                      \
    static void name();                                                                                      \
    static const ::test::Registrar name##Registrar(#name, &(name));                                          \
    static void name()

// 조건이 거짓이면 실패로 기록한다. 실패해도 테스트는 끝까지 실행된다.
#define CHECK(condition)                                                                                     \
    do {                                                                                                     \
        if (!(condition)) {                                                                                  \
            ::test::reportFailure(__FILE__, __LINE__, "CHECK(" #condition ")");                              \
        }                                                                                                    \
    } while (false)

// 실수 비교: |actual - expected| <= tolerance 이어야 통과한다.
#define CHECK_NEAR(actual, expected, tolerance)                                                              \
    do {                                                                                                     \
        const double testActual = static_cast<double>(actual);                                               \
        const double testExpected = static_cast<double>(expected);                                           \
        if (std::abs(testActual - testExpected) > static_cast<double>(tolerance)) {                          \
            ::test::reportFailure(__FILE__, __LINE__,                                                        \
                                  "CHECK_NEAR(" #actual ", " #expected "): got " +                           \
                                      std::to_string(testActual) + ", expected " +                           \
                                      std::to_string(testExpected));                                         \
        }                                                                                                    \
    } while (false)
