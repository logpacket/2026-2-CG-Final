# =============================================================================
# Interactive 3D Virtual Exhibition - 빌드 스크립트
#
#   cmake --preset <프리셋>          (프리셋 목록: cmake --list-presets)
#   cmake --build --preset <프리셋>
#   ctest --preset <프리셋>
#
# 프리셋 없이:  cmake -S . -B build  →  cmake --build build  →  ctest --test-dir build
#
# 구성
#   cmake/Dependencies.cmake    외부 라이브러리 (GLFW, GLM, Dear ImGui: FetchContent / GLAD: external/glad)
#   cmake/Lint.cmake            clang-format / clang-tidy
#   cmake/ProjectModules.cmake  모듈을 만드는 ve_module(...) 와 계층 규칙 검사
#   src/CMakeLists.txt          모듈 목록과 계층 구조 (모듈마다 src/<모듈>/CMakeLists.txt)
#   tests/CMakeLists.txt        단위 테스트
#
# 허용된 라이브러리(GLFW, GLAD, GLM, Dear ImGui) 외에는 추가하지 않는다.
# =============================================================================
cmake_minimum_required(VERSION 3.20)
project(VirtualExhibition LANGUAGES C CXX)

option(VE_BUILD_TESTS "단위 테스트(unit_tests)를 빌드한다" ON)
option(VE_CLANG_TIDY "컴파일할 때 clang-tidy 를 함께 실행한다 (Ninja/Makefile 제너레이터만)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # clang-tidy, clangd 가 읽는 compile_commands.json

# 단일 구성(single-config) 제너레이터(Ninja, Makefiles)에서 빌드 타입을 정하지 않으면 Debug 로 빌드한다.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# 실행 파일은 <빌드 폴더>/bin 에 모인다. Visual Studio 같은 다중 구성(multi-config) 제너레이터는 bin/<Config> 이다.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# MSVC 는 한국어 Windows 에서 소스를 CP949 로 읽는다. 주석이 한글이므로 UTF-8 로 읽게 한다.
if(MSVC)
  add_compile_options(/utf-8)
endif()

include(cmake/Dependencies.cmake)
include(cmake/Lint.cmake)
include(cmake/ProjectModules.cmake)

add_subdirectory(src)

if(VE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# Visual Studio 에서 F5 를 누르면 바로 이 프로그램이 실행되도록 한다.
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VirtualExhibition)
