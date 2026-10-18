# =============================================================================
# 모듈 도우미. src/<모듈>/CMakeLists.txt 에서 ve_module(...) 한 번으로 모듈(정적 라이브러리)을 만든다.
#
#   ve_module(<이름>
#     DEPENDS   <프로젝트 모듈 ...>     (선택) 이 모듈이 쓰는 아래 모듈
#     LIBRARIES <외부 라이브러리 ...>   (선택) glm, glad, glfw, imgui 중 이 모듈이 직접 쓰는 것
#   )
#
# - 타깃 이름은 ve_<이름> 이다. 그 폴더 아래(하위 폴더 포함)의 .cpp/.h 가 모두 들어간다.
#   새 파일은 다음 빌드 때 자동으로 포함된다.
# - 의존은 PUBLIC 이다: 이 모듈을 쓰는 쪽도 의존 모듈의 헤더를 쓸 수 있다.
# - 의존 규칙: DEPENDS 에는 src/CMakeLists.txt 에서 **먼저 추가된** 모듈만 올 수 있다. 어기면 configure 가 실패한다.
#   그래서 위 모듈 → 아래 모듈 한 방향으로만 의존하고, 서로 맞물린(순환) 의존이 생기지 않는다.
# - 외부 라이브러리 헤더는 LIBRARIES 로 연결한 모듈(과 그 위)에서만 보인다.
#   예: core 에서 <glad/glad.h> 나 <GLFW/glfw3.h> 를 include 하면 컴파일 에러가 난다.
# =============================================================================

# 실행 파일과 모듈에 공통으로 적용하는 설정: 경고, MinGW 정적 링크, clang-tidy
function(ve_apply_defaults target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  get_target_property(type ${target} TYPE)
  if(MINGW AND type STREQUAL "EXECUTABLE")
    # MinGW 런타임 DLL 없이 exe 하나로 실행되도록 정적 링크한다.
    target_link_options(${target} PRIVATE -static)
  endif()
  ve_enable_clang_tidy(${target})  # cmake/Lint.cmake (VE_CLANG_TIDY=ON 일 때만 동작)
endfunction()

function(ve_module name)
  cmake_parse_arguments(PARSE_ARGV 1 arg "" "" "DEPENDS;LIBRARIES")

  # 의존 규칙 검사: 먼저 추가된 모듈에만 의존할 수 있다.
  get_property(defined_modules GLOBAL PROPERTY VE_MODULES)
  foreach(dep IN LISTS arg_DEPENDS)
    if(NOT ve_${dep} IN_LIST defined_modules)
      message(FATAL_ERROR "Module '${name}' cannot depend on '${dep}': only modules added before it "
                          "in src/CMakeLists.txt are allowed (lower modules must not use upper ones).")
    endif()
  endforeach()

  set(target ve_${name})
  file(GLOB_RECURSE sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
  list(TRANSFORM arg_DEPENDS PREPEND ve_ OUTPUT_VARIABLE dep_targets)

  add_library(${target} STATIC ${sources})
  # include 는 항상 모듈 이름부터 쓴다: #include "engine/platform/Window.h"
  target_include_directories(${target} PUBLIC ${PROJECT_SOURCE_DIR}/src)
  target_link_libraries(${target} PUBLIC ${dep_targets} ${arg_LIBRARIES})
  target_compile_definitions(${target} PUBLIC GLFW_INCLUDE_NONE)  # GL 헤더는 GLAD 가 담당
  if(MSVC)
    target_compile_definitions(${target} PUBLIC _CRT_SECURE_NO_WARNINGS NOMINMAX)
  endif()
  ve_apply_defaults(${target})

  set_property(GLOBAL APPEND PROPERTY VE_MODULES ${target})
endfunction()
