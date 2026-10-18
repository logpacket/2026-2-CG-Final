# =============================================================================
# 외부 라이브러리
#   GLFW 3.4, GLM 1.0.1, Dear ImGui 1.91.8 : FetchContent 로 처음 configure 할 때 내려받는다.
#                                            (SHA256 으로 고정해 누가 받아도 같은 파일이 된다)
#   GLAD (OpenGL 4.1 Core)                 : external/glad 에 생성된 소스를 포함 (external/glad/README.md)
#
# 버전을 올릴 때는 URL 과 URL_HASH 를 함께 바꾼다. 해시는 내려받은 zip 으로 계산한다:
#   cmake -E sha256sum <파일.zip>
# =============================================================================
include(FetchContent)

# 내려받은 압축 파일의 시간 정보를 무시한다 (CMake 3.24+ 경고 방지).
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

# --- GLFW: 창, OpenGL 컨텍스트, 키보드/마우스 입력 ---
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
# Linux: X11 백엔드만 빌드해 필요한 개발 패키지를 줄인다 (Wayland 세션에서도 XWayland 로 동작).
set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "" FORCE)
FetchContent_Declare(glfw
  URL      https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
  URL_HASH SHA256=b5ec004b2712fd08e8861dc271428f048775200a2df719ccf575143ba749a3e9
)

# --- GLM: 벡터/행렬/쿼터니언 (헤더만 쓰므로 GLM 자체 CMakeLists 는 실행하지 않는다) ---
FetchContent_Declare(glm
  URL      https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
  URL_HASH SHA256=09c5716296787e1f7fcb87b1cbdbf26814ec1288ed6259ccd30d5d9795809fa5
  SOURCE_SUBDIR cmake-not-used
)

# --- Dear ImGui: 디버그 UI (CMake 파일이 없으므로 아래에서 직접 라이브러리를 만든다) ---
FetchContent_Declare(imgui
  URL      https://github.com/ocornut/imgui/archive/refs/tags/v1.91.8.zip
  URL_HASH SHA256=c17aa6abe51dda39b0ea503b855b4a819a30d3affa6d53fb887541c29d686b98
)

FetchContent_MakeAvailable(glfw glm imgui)

add_library(glm INTERFACE)
target_include_directories(glm SYSTEM INTERFACE ${glm_SOURCE_DIR})

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui SYSTEM PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC glfw)

# --- GLAD: OpenGL 4.1 Core 함수 로더 ---
add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c)
target_include_directories(glad SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
