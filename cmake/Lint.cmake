# =============================================================================
# C++ 린터
#   clang-format : 코드 서식(들여쓰기, 줄바꿈, include 순서). 규칙은 .clang-format
#   clang-tidy   : 버그가 되기 쉬운 코드, 이름 규칙 검사. 규칙은 .clang-tidy
#
# 설치 (팀원 모두 같은 버전을 쓴다. 버전이 다르면 포맷 결과가 달라진다):
#   cmake -P scripts/install-lint-tools.cmake     → .tools/llvm/bin 에 설치, 여기서 자동으로 찾는다
#
# 사용법
#   cmake --build --preset <프리셋> --target format        모든 소스를 규칙대로 고친다
#   cmake --build --preset <프리셋> --target format-check  규칙과 다른 곳이 있으면 실패 (CI 에서 사용)
#   cmake --preset <프리셋> -DVE_CLANG_TIDY=ON             이후 빌드할 때마다 clang-tidy 검사
# =============================================================================

set(VE_LLVM_VERSION 23.1.3)  # scripts/install-lint-tools.cmake 와 같아야 한다
set(VE_LOCAL_LLVM_BIN ${CMAKE_CURRENT_SOURCE_DIR}/.tools/llvm/bin)

# 설치 스크립트로 받은 도구가 있으면 항상 그것을 쓰고, 없으면 PATH 에서 찾는다.
function(ve_find_llvm_tool var name)
  set(local "${VE_LOCAL_LLVM_BIN}/${name}${CMAKE_EXECUTABLE_SUFFIX}")
  if(EXISTS "${local}")
    set(${var} "${local}" CACHE FILEPATH "Path to ${name}" FORCE)
  else()
    find_program(${var} NAMES ${name})
  endif()
  if(${var})
    execute_process(COMMAND "${${var}}" --version OUTPUT_VARIABLE text ERROR_QUIET)
    if(NOT text MATCHES "version ${VE_LLVM_VERSION}")
      message(WARNING "${name} at ${${var}} is not version ${VE_LLVM_VERSION}; results may differ from CI.\n"
                      "Install the team version with: cmake -P scripts/install-lint-tools.cmake")
    endif()
  endif()
endfunction()

ve_find_llvm_tool(CLANG_FORMAT_EXE clang-format)
ve_find_llvm_tool(CLANG_TIDY_EXE clang-tidy)

file(GLOB_RECURSE VE_FORMAT_FILES CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h
)

if(CLANG_FORMAT_EXE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXE} -i ${VE_FORMAT_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "clang-format: formatting sources"
    VERBATIM
  )
  add_custom_target(format-check
    COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${VE_FORMAT_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "clang-format: checking sources"
    VERBATIM
  )
else()
  message(STATUS "clang-format not found: 'format' / 'format-check' targets are disabled "
                 "(install: cmake -P scripts/install-lint-tools.cmake)")
endif()

# 지정한 타깃을 컴파일할 때 clang-tidy 를 함께 실행한다. (외부 라이브러리 타깃에는 적용하지 않는다)
function(ve_enable_clang_tidy)
  if(NOT VE_CLANG_TIDY)
    return()
  endif()
  if(NOT CLANG_TIDY_EXE)
    message(FATAL_ERROR "VE_CLANG_TIDY=ON but clang-tidy was not found (install: cmake -P scripts/install-lint-tools.cmake)")
  endif()
  if(CMAKE_GENERATOR MATCHES "Visual Studio")
    # Visual Studio 제너레이터에서는 CXX_CLANG_TIDY 가 동작하지 않는다.
    # 대신 Visual Studio 의 코드 분석으로 빌드할 때마다 clang-tidy 를 돌린다 (.clang-tidy 를 그대로 읽는다).
    get_filename_component(tidy_dir "${CLANG_TIDY_EXE}" DIRECTORY)
    foreach(target IN LISTS ARGN)
      set_target_properties(${target} PROPERTIES
        VS_GLOBAL_RunCodeAnalysis true
        VS_GLOBAL_EnableMicrosoftCodeAnalysis false
        VS_GLOBAL_EnableClangTidyCodeAnalysis true
        VS_GLOBAL_ClangTidyToolPath "${tidy_dir}"
      )
    endforeach()
    return()
  endif()
  set(tidy_command ${CLANG_TIDY_EXE})
  if(MSVC)
    # CMake 는 컴파일 명령을 -- 뒤에 넘기는데, clang-tidy 는 거기서 / 로 시작하는 옵션을 파일 이름으로 보고 버린다.
    # /EHsc 가 빠지면 예외가 꺼진 것으로 보고 throw/try 를 에러로 내므로 다시 넣어 준다.
    list(APPEND tidy_command --extra-arg=/EHsc)
  elseif(MINGW)
    # Windows 용 clang-tidy 는 기본으로 MSVC 헤더를 찾으므로, MinGW 헤더를 쓰도록 타깃 트리플을 지정한다.
    list(APPEND tidy_command --extra-arg=--target=x86_64-w64-mingw32)
  endif()
  foreach(target IN LISTS ARGN)
    set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${tidy_command}")
  endforeach()
endfunction()
