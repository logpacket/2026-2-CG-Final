# C++ 코딩 규칙 (초안)

> 팀 합의 전 초안이다. 안건별 결정과 이유는 맨 아래 [11. 결정 기록](#11-결정-기록)에 있다. 팀이 확인하면 "초안" 표시를 뗀다.

## 0. 원칙

1. **도구가 잡는 것은 도구에 맡긴다.** 서식과 이름 규칙은 외우지 말고 `format` 타깃과 clang-tidy 를 돌린다.
   이 문서는 주로 도구가 잡지 못하는 것을 다룬다.
2. **주변 코드를 따른다.** 이 문서에 없는 경우는 같은 폴더의 기존 코드처럼 쓴다.
3. **영리한 코드보다 읽기 쉬운 코드.** 성능 최적화는 측정한 뒤에 한다.
4. **macOS 에서도 돌아야 한다.** OpenGL 4.1 Core / GLSL 410 을 넘는 기능은 쓰지 않는다 ([9.1](#91-버전-경계)).

| 무엇을 | 도구 | 설정 파일 |
|---|---|---|
| 서식 (들여쓰기, 줄바꿈, include 순서) | clang-format 23 | `.clang-format` |
| 이름 규칙, 버그가 되기 쉬운 코드 | clang-tidy 23 | `.clang-tidy` |
| 컴파일러 경고 | MSVC `/W4 /permissive-`, GCC·Clang `-Wall -Wextra -Wpedantic` | `cmake/ProjectModules.cmake` |
| 모듈 의존 방향 | CMake configure 단계 검사 | `cmake/ProjectModules.cmake` |
| 인코딩, 줄 끝 공백, 셰이더 들여쓰기 | EditorConfig | `.editorconfig` |

언어는 **C++17** 이다. `std::optional`, `std::string_view`, 구조적 바인딩, `if constexpr`, `[[nodiscard]]` 는 쓸 수 있다.
C++20 기능(`std::span`, `std::format`, designated initializer `{.x = 1}`, concepts)은 쓰지 않는다.
특히 designated initializer 는 GCC 에서는 경고만 나고 MSVC C++17 모드에서는 컴파일 에러라서 실수하기 쉽다.

## 1. 레퍼런스

### C++ 일반

| 자료 | 쓰는 곳 |
|---|---|
| [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) | 이 문서의 기본 근거. 자주 볼 절: R (자원 관리), C.20–C.22 (특수 멤버 함수), F.15–F.21 (매개변수 전달), ES.20 (변수 초기화) |
| [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) | 이름 규칙(`kConstant`)과 헤더 규칙의 출처. 예외 금지 같은 다른 규칙은 따르지 않는다 |
| [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html) | `.clang-format` 의 바탕 스타일 |
| [cppreference](https://en.cppreference.com) | 표준 라이브러리 레퍼런스. 기능마다 표시된 "(since C++20)" 을 확인한다 |
| *Effective Modern C++* (Scott Meyers) | 이동, `unique_ptr`, `auto` 를 깊게 이해하고 싶을 때 |

### 그래픽스

| 자료 | 쓰는 곳 |
|---|---|
| [docs.gl](https://docs.gl) | GL 함수 레퍼런스. 함수마다 지원 버전 표가 있어 **4.1 에 있는 함수인지** 확인할 때 쓴다 |
| [Khronos OpenGL Registry](https://registry.khronos.org/OpenGL/index_gl.php) | OpenGL 4.1 Core / GLSL 4.10 사양서. 동작이 애매할 때의 최종 근거 |
| [OpenGL Wiki: Common Mistakes](https://www.khronos.org/opengl/wiki/Common_Mistakes) | GL 객체를 C++ 클래스로 감쌀 때의 함정, 자주 하는 실수 모음 |
| [LearnOpenGL](https://learnopengl.com) | 개념 학습(조명, 그림자, PBR). OpenGL 3.3 기준이고, 전역 변수와 한 파일에 몰아 쓰는 코드 구조는 따라 하지 않는다 |
| [GLM 매뉴얼](https://github.com/g-truc/glm/blob/master/manual.md) | GLM 함수와 설정 매크로 |
| [Dear ImGui FAQ](https://github.com/ocornut/imgui/blob/master/docs/FAQ.md), `imgui_demo.cpp` | 위젯 사용법(F1 패널의 "Show ImGui demo"), ID 충돌 문제 |
| [Game Programming Patterns](https://gameprogrammingpatterns.com) | 웹에서 무료. Game Loop, Update Method, Component 장이 이 프로젝트 구조와 맞닿아 있다 |
| *Real-Time Rendering* 4판 | 렌더링 이론 레퍼런스 |
| [RenderDoc](https://renderdoc.org) | 프레임 캡처 디버거. 화면이 이상할 때 어떤 draw call 이 무엇을 그렸는지 본다 (Windows/Linux) |

## 2. 파일

- 파일 이름은 그 파일의 주 클래스 이름과 같은 PascalCase 다: `FrameTimer.h` / `FrameTimer.cpp`.
  함수 묶음도 PascalCase 로 짓는다: `Primitives.h`, `AssetPath.h`.
- 헤더는 `.h`, 소스는 `.cpp`, 테스트는 `tests/<이름>Test.cpp` 다.
- 파일 첫 줄은 **무엇을 하는 파일인지 한 줄 설명**이다. `// Owner:` 줄은 쓰지 않는다. 헤더는 그다음 줄이 `#pragma once` 다.
- `.cpp` 안에서만 쓰는 함수, 타입, 상수는 익명 namespace 에 넣는다.

```cpp
// 축 정렬 경계 상자(AABB): 좌표가 가장 작은 꼭짓점(min)과 가장 큰 꼭짓점(max)으로 물체를 감싼다.
#pragma once

#include <glm/vec3.hpp>
```

### include

- 프로젝트 헤더는 모듈 이름부터 쓴다: `#include "engine/platform/Window.h"`. `../` 같은 상대 경로는 쓰지 않는다.
- 순서는 clang-format 이 정리한다: 자기 헤더 → 프로젝트 헤더 → 외부 라이브러리 → 표준 라이브러리.
- 헤더에는 꼭 필요한 것만 include 한다.
  - 포인터나 참조로만 쓰는 타입은 전방 선언한다: `struct GLFWwindow;`
  - GLM 은 `<glm/glm.hpp>` 대신 쓰는 헤더만 include 한다: `<glm/vec3.hpp>`, `<glm/mat4x4.hpp>`.
    `<glm/gtc/matrix_transform.hpp>` 같은 함수 헤더는 `.cpp` 에서 include 한다.
  - `<glad/glad.h>` 는 `.cpp` 와 `engine/gfx/` 헤더에서만 include 한다. 그 밖의 헤더는 GL 타입을 드러내지 않는다.
- `using namespace` 는 헤더에서 쓰지 않는다. `.cpp` 에서도 `using namespace std;` 는 쓰지 않는다.

## 3. 이름

| 대상 | 규칙 | 예 | clang-tidy 검사 |
|---|---|---|---|
| 클래스, 구조체, enum | PascalCase | `FrameTimer`, `DebugPanelSettings` | ✓ |
| enum 값 | PascalCase, `enum class` 로 | `RenderMode::Wireframe` | ✓ |
| 함수, 메서드 | camelCase. 동작은 동사로 시작 (getter 는 아래) | `tick()`, `drawDebugPanel()` | ✓ |
| 지역 변수, 매개변수 | camelCase | `deltaSeconds`, `nowSeconds` | ✓ |
| private 멤버 변수 | camelCase + 끝에 `_` | `window_`, `showUi_` | ✓ |
| public 멤버 변수 (struct) | camelCase, `_` 없음 | `settings.vsync`, `aabb.min` | |
| 상수 (`constexpr`) | `k` + PascalCase | `kMaxDeltaSeconds`, `kMsaaSamples` | ✓ |
| namespace | lower_case | `ui` | ✓ |
| 매크로 (되도록 쓰지 않는다) | `VE_` + UPPER_SNAKE_CASE. 테스트 매크로(`CHECK` 등)는 예외 | `VE_ASSET_DIR` | |
| 테스트 | 확인하는 내용을 camelCase 문장으로 | `longPauseIsClampedToMaxDelta` | |

- bool 을 돌려주는 함수와 bool 변수는 참/거짓으로 읽히게 짓는다: `isMinimized()`, `wantsKeyboard()`, `showUi_`.
- getter 는 `get` 없이 명사로 쓰고(`fps()`, `handle()`), setter 는 `set` 으로 시작한다(`setVSync()`).
- 약어도 한 단어처럼 쓴다: `drawUi`, `kMsaaSamples`, `Aabb`, `Fbo`. 외부 라이브러리 이름(`ImGui`)은 그대로 둔다.
  기존의 `kGLMajor`, `setVSync` 는 그대로 두고 새 이름부터 맞춘다.
- **단위가 헷갈리는 값은 이름에 단위를 넣는다**: `deltaSeconds`, `frameMs`, `fovYRadians`, `angleDegrees`.
- **좌표 공간이 헷갈리는 값은 이름에 공간을 넣는다**: `positionWorld`, `normalView`.
  변환 행렬은 `model` / `view` / `projection` 을 쓰고, 그 밖의 변환은 `worldFromLocal` 처럼 "도착From출발" 로 짓는다.
  그러면 `worldFromLocal * positionLocal` 처럼 곱하는 순서대로 읽힌다.

### namespace

- 클래스, 구조체, 자유 함수는 전역 namespace 에 둔다. ImGui 패널 함수(`ui::`)와 테스트 도구(`test::`)만 namespace 로 묶는다.
- 이름 충돌은 namespace 대신 아래 두 규칙으로 막는다.
  - `.cpp` 안에서만 쓰는 타입과 함수는 익명 namespace 에 넣는다 ([2. 파일](#2-파일)).
  - 헤더에 새 타입이나 함수를 만들기 전에 `src/` 에서 같은 이름이 있는지 검색한다.
    흔한 이름은 구체적으로 짓는다: `Light` → `PointLight`.
- 같은 이름의 클래스가 서로 다른 `.cpp` 에서만 쓰이면 컴파일러도 링커도 잡지 못하고, 실행할 때 한쪽 정의가 조용히 쓰인다(ODR 위반).

## 4. 클래스와 소유권

- 멤버 변수는 선언할 때 초기값을 준다 (`float delta_ = 0.0f;`, `glm::vec3 position{0.0f};`). 초기화하지 않은 멤버를 두지 않는다 (clang-tidy 검사).
  `glm::vec3` 같은 GLM 타입도 기본 생성자가 값을 초기화하지 않으므로 반드시 초기값을 준다.
- 특수 멤버 함수는 **하나도 쓰지 않는 것(rule of zero)** 을 먼저 노린다.
  소멸자를 직접 쓰면 복사·이동 생성자와 대입 연산자도 모두 `= default` 나 `= delete` 로 밝힌다 (clang-tidy 검사).
- `new` / `delete` 를 직접 쓰지 않는다. 값 멤버 → 컨테이너 → `std::unique_ptr` 순으로 고른다.
  - raw 포인터와 참조는 **빌려 쓰는 것**이다. 받은 쪽이 지우지 않는다.
  - `std::shared_ptr` 는 수명을 정말로 여럿이 나눠 가질 때만 쓰고, 왜 그런지 주석을 단다.
- 멤버는 선언 순서대로 생성되고 **역순으로 소멸한다**. GL 리소스를 가진 멤버는 `window_` 보다 아래에 선언한다 (`App.h` 의 주의 주석).
- 데이터만 묶는 타입은 public 멤버를 가진 `struct` 로 쓴다 (`Vertex`, `Aabb`, `DebugPanelInfo`).
  지켜야 할 조건(불변식)이 있으면 `class` 와 private 멤버로 감싼다.
- 상속은 인터페이스(순수 가상 함수)가 필요할 때만 쓰고 깊게 쌓지 않는다. 구현을 물려받기보다 멤버로 갖는다(합성).
  - 재정의에는 `override` 를 붙인다 (clang-tidy 검사).
  - 다형적으로 쓰는 기반 클래스는 `virtual` 소멸자를 둔다.
- 값을 바꿀 수 있는 전역 변수와 싱글턴은 쓰지 않는다. 여러 곳에서 써야 하면 `App` 이 소유하고 참조로 넘긴다.

## 5. 함수

| 받는 것 | 받는 법 | 예 |
|---|---|---|
| `int`, `float`, `bool`, enum, 포인터 | 값 | `setVSync(bool enabled)` |
| GLM 벡터·행렬, `std::string`, 컨테이너, 큰 struct | `const&` | `makeBox(const glm::vec3& size)` |
| 함수가 고쳐야 하는 것 | `&` | `drawDebugPanel(..., DebugPanelSettings& settings)` |
| 소유권을 넘겨받는 것 | `std::unique_ptr<T>` 값 | `addExhibit(std::unique_ptr<Exhibit> exhibit)` |
| 없을 수도 있는 것 | 포인터 (`nullptr` 허용) | |

- 결과는 출력 매개변수 대신 반환값으로 돌려준다. 여러 개면 struct 로 묶는다. 실패할 수 있으면 `std::optional` 을 쓴다.
- 반환값을 무시하면 안 되는 함수(성공 여부 `bool` 등)에는 `[[nodiscard]]` 를 붙인다.
- 상태를 바꾸지 않는 메서드는 `const` 로 선언한다. 다시 대입하지 않는 지역 변수도 `const` 로 선언한다.
- 함수가 한 화면(40줄 정도)을 넘으면 나눈다.
- 중첩을 줄이려면 조건이 맞지 않을 때 일찍 `return` 한다.
- `std::string_view` 는 끝에 `\0` 이 있다는 보장이 없으므로 GL·GLFW 같은 C API 에 넘기지 않는다. C API 로 가는 문자열은 `const std::string&` 로 받는다.

## 6. 타입과 표현식

- 지역 변수는 선언할 때 초기화한다: `GLint success = 0;` (clang-tidy 검사).
  GL 함수는 실패하면 출력 인자를 건드리지 않으므로, 초기화하지 않으면 쓰레기 값으로 성공 여부를 판단하게 된다.
- `auto` 는 오른쪽에서 타입이 보일 때만 쓴다: `std::make_unique<Mesh>()`, 반복자, 범위 for.
  숫자에는 쓰지 않는다 (`auto x = 1.0;` 은 `float` 이 아니라 `double` 이다).
- 실수 리터럴에는 `f` 를 붙인다: `0.5f`. GPU 로 가는 값과 렌더링 계산은 `float` 이다.
  계속 누적되는 시각만 `double` 로 두고(`glfwGetTime()`), 프레임 사이 간격을 구한 뒤 `float` 로 바꾼다 (`FrameTimer`).
- 실수는 `==` 로 비교하지 않고 오차 범위로 비교한다. 테스트에서는 `CHECK_NEAR` 를 쓴다.
- 캐스트는 `static_cast` 를 쓴다.
  - `reinterpret_cast` 는 C API 경계(GLAD 로더, `glGetString`)에서만 쓴다. 버퍼 오프셋은 `bufferOffset()` 을 쓴다 ([9.6](#96-정점과-버퍼)).
  - C 스타일 캐스트 `(float)x` 는 쓰지 않는다 (clang-tidy 검사).
- 크기 타입이 바뀌는 곳은 명시적으로 바꾼다: `static_cast<GLsizei>(indices.size())`.
- 의미 있는 숫자는 이름 있는 상수로 둔다: `kMsaaSamples = 4`. `0`, `1`, `0.5f` 처럼 식에서 뜻이 분명한 값은 그대로 쓴다.
- bool 로 바꾸는 비교는 드러나게 쓴다: `window_ == nullptr`, `glfwInit() == GLFW_FALSE`, `glfwWindowShouldClose(window_) != 0` (clang-tidy 검사).
- `if`, `for`, `while` 의 본문은 한 줄이어도 중괄호로 감싼다 (clang-tidy 검사).

## 7. 오류 처리와 로그

| 상황 | 처리 | 예 |
|---|---|---|
| 계속할 수 없는 초기화 실패 | `std::runtime_error` 를 던지고 `main` 에서 출력 후 종료 | 창 생성, GLAD 로딩, 필수 셰이더 |
| 계속할 수 있는 실패 | 로그를 남기고 대체물로 진행. `bool` 이나 `std::optional` 반환 | 모델 파일 없음 → 기본 상자, 텍스처 없음 → 단색 |
| 프로그래머 실수 (불변식 위반) | `assert` (Debug 빌드에서만 검사) | 인덱스 범위, `nullptr` 이면 안 되는 포인터 |

- 프레임 루프 안에서는 예외를 던지지 않는다.
- 로그는 `[태그] 메시지` 형식이다. 태그는 클래스나 모듈 이름이다. 정보는 `std::printf`(stdout), 오류는 `std::fprintf(stderr, ...)` 로 쓴다.
- 매 프레임 찍히는 로그는 남기지 않는다. 반복될 수 있는 경고는 한 번만 출력한다.
- 납득되지 않는 clang-tidy 경고는 그 줄에 `// NOLINT(검사이름)` 을 달고 이유를 적는다. 검사 전체를 끄지 않는다.

## 8. 주석과 문자열

- 주석은 한국어로 쓴다. **화면이나 콘솔에 나오는 문자열(UI, 로그, 예외 메시지)은 영어로 쓴다.** ImGui 기본 글꼴에는 한글이 없다.
- 클래스 위에는 필요할 때만 아래 두 항목을 쓴다.
  - `확장:` 기능을 더하려면 어디를 어떻게 고치는지
  - `주의:` 지키지 않으면 깨지는 것
- 코드가 **무엇을** 하는지는 코드로 보이게 하고, 주석에는 **왜** 그런지를 쓴다. 코드를 그대로 다시 말하는 주석은 지운다.
- 단위, 좌표계, 값의 범위처럼 타입에 드러나지 않는 정보는 주석에 적는다: `// 단위 벡터`, `// 0 ~ 1`.
- 할 일은 이슈 번호와 함께 남긴다: `// TODO(#12): 그림자 바이어스 조정`.
- 용어는 통일한다.

| 쓰는 말 | 쓰지 않는 말 |
|---|---|
| 소멸 | 파괴 |
| 상위 / 하위 모듈 | 위 / 아래 모듈 |
| 정점 순서 (winding order) | 감김 방향 |
| 서식 (format) | 코드 모양 |
| 링크 (link) | 연결 |

## 9. 그래픽스

### 9.1 버전 경계

GLAD 에는 OpenGL 4.1 Core 함수만 들어 있어서 4.2 이상의 C 함수를 부르면 **컴파일 에러**로 바로 드러난다.
그런데 GLSL 은 실행할 때 드라이버가 컴파일한다. Windows 의 NVIDIA·AMD 드라이버는 `#version 410` 셰이더 안의
4.2+ 문법을 그냥 받아 주는 경우가 많다. 그래서 **Windows 에서는 되고 macOS 에서만 셰이더 컴파일이 실패한다.**
아래 표의 기능은 쓰지 않는다.

| 쓰고 싶은 것 | 필요한 버전 | 4.1 에서는 |
|---|---|---|
| `layout(binding = N)` (sampler, uniform block) | GLSL 420 | `glUniform1i(location, unit)`, `glUniformBlockBinding` |
| `layout(location = N) uniform ...` | GLSL 430 | `glGetUniformLocation` 으로 찾아 캐시 |
| `glTexStorage2D` | 4.2 | `glTexImage2D`, 밉맵은 `glGenerateMipmap` |
| DSA (`glCreateBuffers`, `glNamedBufferData` …) | 4.5 | `glGen*` 후 `glBind*` |
| `glDebugMessageCallback`, `glObjectLabel` | 4.3 | `glGetError` 확인 ([9.4](#94-오류-확인)) |
| 컴퓨트 셰이더, SSBO, image load/store | 4.3 / 4.2 | 프래그먼트 셰이더 + FBO, 또는 CPU 계산 |
| `glClipControl` (깊이 0 ~ 1) | 4.5 | 기본값 -1 ~ 1 그대로. `GLM_FORCE_DEPTH_ZERO_TO_ONE` 를 정의하지 않는다 |
| `glLineWidth` 1 보다 큰 값 | forward-compatible 컨텍스트(macOS 필수)에서 오류 | 굵은 선은 사각형 메시로 그린다 |

쓸 수 있는 것: VAO, UBO (`std140`), 인스턴싱(`glVertexAttribDivisor`), FBO 와 다중 렌더 타깃, sRGB 텍스처,
정점 입력·프래그먼트 출력의 `layout(location = N) in/out`.
core profile 에서는 **VAO 가 바인딩되어 있지 않으면 그릴 수 없다.**

### 9.2 GL 객체는 RAII 로 감싼다

- `glGen*` / `glDelete*` 는 `engine/gfx/` 의 래퍼 클래스 안에서만 부른다.
- 래퍼는 **이동만 되고 복사는 막는다.** 복사하면 같은 id 를 두 번 지운다.
  이동한 뒤 원본의 id 는 0 으로 만든다. `glDelete*(0)` 은 아무 일도 하지 않으므로 소멸자는 그대로 둬도 된다.
- GL 객체를 전역 변수나 `static` 으로 두지 않는다. GL 컨텍스트가 사라진 뒤에 소멸해서 잘못된 호출이 된다.
- GL 함수는 컨텍스트를 만든 메인 스레드에서만 부른다.

```cpp
// GL 버퍼 객체 하나를 소유한다.
// 주의: 복사하면 같은 버퍼를 두 번 지우게 되므로 이동만 허용한다.
class Buffer {
public:
    Buffer();
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&& other) noexcept : id_(std::exchange(other.id_, 0)) {}
    Buffer& operator=(Buffer&& other) noexcept;

    GLuint id() const { return id_; }

private:
    GLuint id_ = 0;
};

Buffer& Buffer::operator=(Buffer&& other) noexcept
{
    if (this != &other) {
        glDeleteBuffers(1, &id_);
        id_ = std::exchange(other.id_, 0);
    }
    return *this;
}
```

### 9.3 GL 상태

- GL 은 전역 상태 기계다. 그리기 전에 필요한 상태(프로그램, VAO, 텍스처 유닛, 깊이·블렌드·컬링)를 직접 설정하고,
  앞에서 누가 무엇을 해 뒀을 거라고 가정하지 않는다.
- 패스 하나에서만 바꾸는 상태(반투명 패스의 블렌드, 그림자 패스의 FBO·뷰포트)는 패스가 끝날 때 되돌린다.
- VAO 가 바인딩된 동안 `GL_ELEMENT_ARRAY_BUFFER` 를 0 으로 바인딩하지 않는다. 그 바인딩은 VAO 에 기록된다.
  설정이 끝나면 VAO 를 먼저 해제한다.
- 텍스처 유닛 번호는 한 곳에 상수로 둔다 (`kAlbedoUnit = 0` …). 셰이더의 sampler 에는 `glUniform1i` 로 알려 준다.
- FBO 를 바꾸면 `glViewport` 도 그 크기로 바꾼다. 화면으로 돌아올 때는 `window.framebufferSize()` 를 쓴다.
  macOS Retina 에서는 창 크기의 2배다.

### 9.4 오류 확인

- 셰이더 컴파일·링크 결과는 **항상** 확인하고, 실패하면 info log 를 셰이더 파일 이름과 함께 출력한다.
- 4.1 에는 디버그 콜백(`glDebugMessageCallback`)이 없으므로 `glGetError` 로 확인한다.
- GL 오류 플래그는 누가 꺼낼 때까지 남아 있다. 그래서 호출마다 감싸지 않고 **구간 끝에서 한 번** `VE_GL_CHECK()` 를 부른다.
  - gfx 래퍼에서 리소스를 만들거나 데이터를 올리는 함수의 끝 (셰이더 링크, 메시·텍스처 업로드)
  - 렌더 패스마다 끝에서
  - `App::render` 끝에서 매 프레임 한 번 (어디서 났는지 모르는 오류를 잡는 그물)
- 오류가 잡히면 그 구간 안에 `VE_GL_CHECK()` 를 더 넣어 범위를 좁히거나 RenderDoc 으로 본다.
- Release 빌드는 CMake 가 `NDEBUG` 를 정의하므로 비용이 없다 (`assert` 와 같은 스위치).
- `checkGlErrors()` 는 `glGetError()` 가 `GL_NO_ERROR` 를 돌려줄 때까지 오류를 모두 꺼내 `[GL] GL_INVALID_OPERATION at Mesh.cpp:42` 형식으로 출력한다.
  같은 위치를 한 번만 출력하려고 함수 안에 두는 `static` 컨테이너는 4절 "전역 변수 금지" 의 예외다.

`engine/gfx/GlCheck.h`:

```cpp
// Debug 빌드에서 쌓인 GL 오류를 꺼내 출력한다. Release 에서는 아무 일도 하지 않는다.
#pragma once

#ifdef NDEBUG
#define VE_GL_CHECK() static_cast<void>(0)
#else
#define VE_GL_CHECK() checkGlErrors(__FILE__, __LINE__)
#endif

// 같은 위치의 오류는 한 번만 출력한다 (매 프레임 반복되는 오류로 콘솔이 넘치지 않게).
void checkGlErrors(const char* file, int line);
```

- `glGetUniformLocation` 이 -1 을 돌려주면 셰이더가 그 uniform 을 쓰지 않아 컴파일러가 지운 것이다.
  오류는 아니지만 이름 오타일 수 있으니 경고를 한 번 출력한다.
- 화면이 까맣거나 이상하면 RenderDoc 으로 프레임을 캡처해 draw call 마다 입력·출력을 본다.

### 9.5 좌표계, 단위, 행렬

- **오른손 좌표계, +Y 가 위, 길이 1 = 1 m** 다. 카메라는 기본적으로 -Z 를 본다 (`glm::lookAt`).
- 삼각형은 앞면에서 볼 때 **반시계 방향(CCW)** 이고, 뒷면은 그리지 않는다. GL 기본값(`GL_CCW`)과 같다.
- 각도는 코드 안에서 **라디안**이다. GLM 함수가 라디안을 받는다.
  UI 에서 도(degree)로 보여 주려면 UI 경계에서 `glm::radians` / `glm::degrees` 로 바꾼다.
- GLM 과 GL 은 둘 다 열 우선(column-major)이다. `glUniformMatrix4fv` 의 transpose 인자는 `GL_FALSE`, 포인터는 `glm::value_ptr` 로 넘긴다.
- 곱하는 순서는 `projection * view * model * position` 이다. 오른쪽 행렬부터 적용된다.
- 모델 행렬에 비균등 스케일이 있으면 노멀은 `transpose(inverse(mat3(model)))` 로 변환한다.
  이 행렬은 CPU 에서 한 번 계산해 uniform 으로 넘긴다. 셰이더에서 `inverse` 를 부르면 정점마다 계산한다.
- 투영 행렬의 near 를 너무 작게 잡지 않는다 (0.1 m 정도). far / near 비가 크면 깊이 정밀도가 모자라 면이 깜빡인다(z-fighting).
- `GLM_FORCE_*` 매크로를 파일마다 정의하지 않는다. 파일마다 GLM 타입이 달라져 링크 후 알기 힘든 버그가 된다. 필요하면 CMake 에서 전체에 정의한다.

### 9.6 정점과 버퍼

- 정점 struct 는 `static_assert` 로 크기를 고정한다 (`Vertex` 참고).
- attribute 오프셋은 `bufferOffset(offsetof(Vertex, normal))` 로 넘긴다.
  정수를 포인터로 바꾸는 캐스트는 이미 켜져 있는 clang-tidy 검사(`performance-no-int-to-ptr`)에 걸리므로, 아래 도우미 한 곳에만 NOLINT 를 둔다.

```cpp
// glVertexAttribPointer 등은 버퍼 안의 바이트 위치를 포인터 타입으로 받는다 (옛 API 의 흔적).
inline const void* bufferOffset(std::size_t bytes)
{
    return reinterpret_cast<const void*>(bytes);  // NOLINT(performance-no-int-to-ptr)
}
```

- attribute location 은 C++ 쪽 상수 한 곳과 셰이더의 `layout(location = N)` 이 같아야 한다 (position 0, normal 1, uv 2).
- 인덱스는 `std::uint32_t` 이고 `GL_UNSIGNED_INT` 로 그린다.
- UBO 는 `std140` 이다. `vec3` 는 16바이트로 정렬되므로 UBO 안에서는 `vec4` 를 쓰거나 패딩을 넣고,
  C++ struct 는 `static_assert(sizeof(...) == ...)` 로 크기를 확인한다.
- 버퍼는 로드할 때 `glBufferData` 로 한 번 만들고, 매 프레임 바뀌는 데이터는 같은 크기로 `glBufferSubData` 한다.

### 9.7 프레임 루프 성능

- 프레임 루프 안에서 하지 않는 것: 파일 읽기, 셰이더 컴파일, `glGen*` / `glDelete*`, 큰 메모리 할당, 문자열을 조합하는 로그.
- GPU 가 끝나기를 기다리게 하는 호출(`glGet*`, `glReadPixels`, `glFinish`)은 매 프레임 쓰지 않는다. 클릭할 때 한 번(피킹)은 괜찮다.
- uniform location 은 셰이더를 로드할 때 한 번 찾아 저장한다. `glGetUniformLocation` 은 문자열 검색이다.
- 움직임은 항상 `속도 × deltaSeconds()` 로 계산한다. 그래야 FPS 와 상관없이 같은 속도로 움직인다.
- 성능은 **Release 빌드**로 잰다. Debug 빌드(특히 MSVC)의 GLM 은 매우 느려서 판단 근거가 되지 않는다. FPS 는 F1 패널에서 본다.

### 9.8 색과 감마

- 조명 계산은 **선형(linear) 공간**에서 한다.
- 색 텍스처(albedo, base color)는 `GL_SRGB8_ALPHA8` 로 올린다. 샘플링할 때 GPU 가 선형으로 바꿔 준다.
  노멀맵, roughness, metallic 같은 데이터 텍스처는 선형(`GL_RGBA8`)으로 올린다.
- 최종 출력의 감마 보정은 **셰이더에서, 한 곳에서만** 한다. 두 번 하면 화면이 허옇게 뜨고, 안 하면 어둡게 나온다.
  - 렌더러 초기: 조명 셰이더 마지막에서 `outColor = vec4(pow(color, vec3(1.0 / 2.2)), alpha);`
  - HDR 을 쓰면: 장면을 `GL_RGBA16F` FBO 에 그리고, 마지막 전체 화면 패스에서 톤 매핑 다음에 감마를 적용한다. 장면 셰이더에서는 감마를 뺀다.
- `GL_FRAMEBUFFER_SRGB` 는 켜지 않는다. 켜면 ImGui 와 `glClearColor` 까지 변환되어 색이 연해지고,
  기본 프레임버퍼가 sRGB 를 지원하는지도 플랫폼마다 다르다.
- HDR FBO 로 바꿀 때 함께 챙긴다.
  - 배경색: 디버그 패널의 `clearColor` 는 화면에 보이는 색(sRGB)이다. 선형인 HDR FBO 를 지울 때는 `glm::pow(color, glm::vec3(2.2f))` 로 바꿔서 지운다.
  - MSAA: `Window` 가 요청한 4x MSAA 는 기본 프레임버퍼에만 적용된다. 장면을 FBO 에 그리면 멀티샘플 렌더버퍼(`glRenderbufferStorageMultisample`)에
    그린 뒤 `glBlitFramebuffer` 로 합친다.

### 9.9 셰이더 (GLSL)

- 위치는 `assets/shaders/<이름>.vert`, `<이름>.frag` 쌍이다. 지오메트리 셰이더는 `.geom` 이다.
  경로는 `assetPath("shaders/basic.vert")` 로 만든다.
- 첫 줄은 `#version 410 core` 다.
- 이름 규칙 (C++ 와 같은 camelCase 에 역할 접두사):

| 대상 | 규칙 | 예 |
|---|---|---|
| uniform | `u` | `uModel`, `uLightColor` |
| 텍스처 sampler | `u` + `...Map` | `uAlbedoMap`, `uShadowMap` |
| uniform 블록 | 블록 이름은 PascalCase + `Block`, 멤버에는 `u` | `uniform CameraBlock { mat4 uView; mat4 uProjection; };` |
| 정점 입력 | `a` | `aPosition`, `aNormal` |
| 셰이더 단계 사이 (out → in) | `v` | `vNormalWorld`, `vUv` |
| 프래그먼트 출력 | `out`, 여러 개면 `out` + 대상 | `outColor` / `outAlbedo`, `outNormal` |
| 상수 | `k` | `const float kPi = 3.14159265;` |

- 좌표 공간은 C++ 처럼 이름 끝에 붙인다: `vPositionWorld`.
- 단계 사이 변수는 location 을 따로 주지 않으면 이름으로 연결된다. 정점 셰이더의 `out` 과 프래그먼트 셰이더의 `in` 은 이름이 같아야 한다.
- 들여쓰기는 4칸(`.editorconfig`)이고 중괄호는 C++ 와 같게 쓴다.
  clang-format 은 GLSL 을 공식 지원하지 않아 `layout`, `in`/`out` 선언을 망가뜨릴 수 있으므로 셰이더에는 돌리지 않는다.
- 모든 정점·픽셀에서 같은 값이 나오는 계산(MVP 행렬 곱, `inverse`)은 CPU 에서 한 번 하고 uniform 으로 넘긴다.
- 보간된 노멀은 길이가 1 이 아니므로 프래그먼트 셰이더에서 다시 `normalize` 한다.

### 9.10 Dear ImGui

- ImGui 호출은 `beginFrame()` 과 `endFrame()` 사이(`App::drawUi`)에서만 한다.
- 패널 함수는 `exhibition/ui/` 에 두고 상태를 직접 갖지 않는다.
  보여 줄 값(`...Info`)과 사용자가 바꿀 값(`...Settings`)을 인자로 받는다 (`DebugPanel` 참고).
- 같은 창 안에 같은 라벨이 둘 이상이면 ID 가 겹쳐 위젯이 같이 움직인다. `"Speed##gear1"` 처럼 `##` 뒤에 구분자를 붙이거나,
  반복문에서는 `ImGui::PushID(i)` / `ImGui::PopID()` 로 감싼다.
- 3D 장면 입력을 처리하기 전에 `wantsMouse()` / `wantsKeyboard()` 를 확인한다. ImGui 창 위의 클릭이 장면까지 가지 않게 한다.

## 10. 테스트

- `core` 로직을 바꾸면 `tests/<이름>Test.cpp` 에 테스트를 추가한다. 테스트는 창과 GPU 없이 돌아야 한다.
- GL 을 쓰는 기능은 계산 부분을 `core` 의 순수 함수로 떼어 내 그 함수를 테스트한다.
  예: 피킹의 광선-AABB 교차 검사는 `core/math/` 에 두고 테스트한다.
- 실수는 `CHECK_NEAR` 로 비교한다.
- 사용법은 `tests/TestFramework.h` 맨 위 주석에 있다.

## 11. 결정 기록

| # | 안건 | 결정 | 이유 | 관련 절 |
|---|---|---|---|---|
| 1 | namespace | 전역 유지. `ui::`, `test::` 만 namespace. 충돌은 익명 namespace 와 이름 검색으로 막는다 | 외부 라이브러리와는 겹칠 일이 없다(GLFW·GLAD 는 접두사, ImGui·GLM 은 자기 namespace). 모듈별 namespace 는 이름만 길어진다. 나중에 이름이 실제로 겹치면 전체를 `ve::` 하나로 감싼다 | [3](#namespace) |
| 2 | 감마 보정 위치 | 셰이더. HDR 을 쓰면 마지막 패스 한 곳 | `GL_FRAMEBUFFER_SRGB` 는 ImGui 와 배경색까지 바꾸고, 기본 프레임버퍼의 sRGB 지원이 플랫폼마다 다르다. PBR 은 어차피 HDR 과 톤 매핑이 필요하다 | [9.8](#98-색과-감마) |
| 3 | GL 오류 확인 | `VE_GL_CHECK()` 를 구간 끝에 두는 방식, Debug 빌드에서만 | 호출마다 감싸면 지저분하고 빠뜨리기 쉽다. GL 오류 플래그는 꺼낼 때까지 남아 있어 구간 단위로 충분하다 | [9.4](#94-오류-확인) |
| 4 | clang-tidy 검사 추가 | `readability-implicit-bool-conversion`, `cppcoreguidelines-init-variables`, `cppcoreguidelines-pro-type-member-init`, `google-readability-casting` 을 켠다. `readability-magic-numbers`, `misc-const-correctness` 는 켜지 않는다 | 기존 코드에 걸리는 패턴이 없어 지금 켜면 경고 0개에서 시작한다. 꺼 둔 두 검사는 그래픽스 코드(`0.5f`, `2.0f` …)에서 경고만 쌓인다 | [4](#4-클래스와-소유권), [6](#6-타입과-표현식) |
| 5 | 셰이더 파일 서식 | `.editorconfig` 에 4칸 들여쓰기 추가. clang-format 은 돌리지 않는다 | clang-format 은 GLSL 을 공식 지원하지 않는다 | [9.9](#99-셰이더-glsl) |
| 6 | 셰이더 이름 접두사 | `u` / `a` / `v` / `out`, sampler 와 uniform 블록 규칙 추가 | 셰이더 안에서 값이 어디서 왔는지 보이고, C++ 의 `glGetUniformLocation(program, "uModel")` 과 문자열이 같아 한 번에 검색된다 | [9.9](#99-셰이더-glsl) |

### 적용 상태

| 대상 | 상태 |
|---|---|
| `.editorconfig` | 5번 셰이더 항목 추가함 |
| `.clang-tidy` | 4번 검사 4개 추가함. 지금 있는 소스·테스트 15개 파일에서 경고 없음을 확인했다 (clang-tidy 22.1.3, 팀 버전 23.1.3 으로 다시 확인 필요) |
| `engine/gfx/` | 3번의 `VE_GL_CHECK()` / `checkGlErrors()` 와 [9.6](#96-정점과-버퍼) 의 `bufferOffset()` 은 아직 없다. GL 래퍼를 만들 때 함께 추가한다 |

### 후속 제안

- glslangValidator(Khronos 공식 GLSL 검사기)로 셰이더를 검사하는 `shader-check` 타깃.
  `#version 410` 에서 안 되는 문법을 Windows 에서도 에러로 잡아 [9.1](#91-버전-경계) 의 문제를 PR 전에 막는다.
  코드에 들어가는 라이브러리가 아니라 개발 도구지만, 과제 규칙상 괜찮은지 확인한 뒤 설치 스크립트에 추가한다.
