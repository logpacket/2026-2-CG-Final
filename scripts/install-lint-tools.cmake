# =============================================================================
# clang-format / clang-tidy 23.1.3 설치 스크립트 — pip, 관리자 권한, PATH 수정 없이 CMake 하나로 동작한다.
#
#   cmake -P scripts/install-lint-tools.cmake               (FinalProject 폴더에서 실행)
#   cmake -DFORCE=ON -P scripts/install-lint-tools.cmake    (이미 있어도 다시 설치)
#
# 하는 일
#   1) OS 와 CPU 를 알아내 맞는 파일을 고른다 (Windows x64, Linux x86_64/aarch64, macOS arm64/x86_64).
#   2) PyPI 에 올라와 있는 공식 LLVM 바이너리 휠(.whl = zip)을 내려받고 SHA256 으로 확인한다.
#   3) 실행 파일과 clang 내장 헤더만 .tools/llvm/ 에 푼다. 시스템에는 아무것도 설치하지 않는다.
#
# 설치 후 configure 를 다시 하면 cmake/Lint.cmake 가 .tools/llvm/bin 의 도구를 먼저 쓴다.
# 지우려면 .tools 폴더를 지운다. (.gitignore 에 들어 있어 git 에는 올라가지 않는다)
#
# 버전을 올릴 때: 아래 LLVM_VERSION 과 플랫폼별 경로·해시를 함께 바꾼다.
#   https://pypi.org/project/clang-format/<버전>/#files , https://pypi.org/project/clang-tidy/<버전>/#files
#   (cmake/Lint.cmake 의 VE_LLVM_VERSION, CI 도 같은 버전을 쓴다)
# =============================================================================
cmake_minimum_required(VERSION 3.20)

set(LLVM_VERSION 23.1.3)
set(PYPI_FILES https://files.pythonhosted.org/packages)

get_filename_component(PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(TOOLS_DIR "${PROJECT_ROOT}/.tools")
set(INSTALL_DIR "${TOOLS_DIR}/llvm")

# --- 플랫폼별 휠: <PyPI 파일 경로> <SHA256> ---
set(FORMAT_win_amd64
    23/a6/90148f91544e3c056e78c514f0e764c1b44d50a33344c83fd03011ea97c8/clang_format-23.1.3-py2.py3-none-win_amd64.whl
    6c2b74c3a3e942ec172d768e7b945f8e136db05a7b735d49ecfc69bda97643ad)
set(TIDY_win_amd64
    ac/f0/cd5021e635cebaf42c696c46ecc583d77b4e3a396a1a58cdadb8b99c4ef2/clang_tidy-23.1.3-py2.py3-none-win_amd64.whl
    cf0d0e671f707b5a668c035382b964056067076445e87b736b19ff482229010a)

set(FORMAT_linux_x86_64
    11/d1/658d3da4dcbca98620956868c7cf6f4ba19fccecbdfc30cc055e19c9d4a7/clang_format-23.1.3-py2.py3-none-manylinux_2_27_x86_64.manylinux_2_28_x86_64.whl
    58bf1804216501669832a39f2415e14284337438a062ff564729792adec664ac)
set(TIDY_linux_x86_64
    7a/f0/5d684465b0727e3d5364de38cead2775f73ff35f63694176a0694c2ab943/clang_tidy-23.1.3-py2.py3-none-manylinux_2_27_x86_64.manylinux_2_28_x86_64.whl
    6b2558bc396a6e792190700134609e041f657d7f8d15385e8b0aa66f5ffe21b5)

set(FORMAT_linux_aarch64
    1d/00/a0f99e24dc4892fb120e82fa9cf6e8fcb92714181ffd1165ab7eaf5ba1fd/clang_format-23.1.3-py2.py3-none-manylinux_2_26_aarch64.manylinux_2_28_aarch64.whl
    072113527b49a9c0ab9058006d9fe20e583823e6482608a34e1883dd2c49ae7d)
set(TIDY_linux_aarch64
    3d/6f/1c50dacbc9ae537e0e40be27fa7b0a56e60dcc0a1fffd0d9b280b1ed6749/clang_tidy-23.1.3-py2.py3-none-manylinux_2_26_aarch64.manylinux_2_28_aarch64.whl
    5639a8e65df6701ac144ee6b9e73aef4bdd3706cdc5849de1a5147c5aeaddf24)

set(FORMAT_macos_arm64
    47/a5/2007b7a7a53dfe417e66d0a6b38b2f87f16391bfe6fe522d436d1e50be1c/clang_format-23.1.3-py2.py3-none-macosx_11_0_arm64.whl
    79f53ac55434af6dda88eb90eeb65d903ff920c910f78041de41e3ca666dfba5)
set(TIDY_macos_arm64
    26/42/65a2c5ee9be88b81d3890df15fd1af7f5bbab72a16ccf4d302454385e067/clang_tidy-23.1.3-py2.py3-none-macosx_11_0_arm64.whl
    cbd31dea271cee7ed5b5048847e73b8208c8dde90e321e09c20d9146011f8ff2)

set(FORMAT_macos_x86_64
    05/0f/a1813ede1e149e700ee810baf22dd9c863ca0bd719d73f86e8c10b67a6bb/clang_format-23.1.3-py2.py3-none-macosx_10_9_x86_64.whl
    0b8e86cc5a870ca42989bd1ccfe5220fbab96e386922d0a8f0ca350db30593df)
set(TIDY_macos_x86_64
    f0/a6/b5b34b1770cc50aa216d33a45456c1a69375dee40a0abad91ef2656d570c/clang_tidy-23.1.3-py2.py3-none-macosx_10_9_x86_64.whl
    6d745b16e375bfc2219bca5f6697dc17348313f6b1fc58f8fa7321172a59ecdf)

# --- 1) 플랫폼 판별 ---
cmake_host_system_information(RESULT host_os QUERY OS_NAME)        # Windows / Linux / macOS(Darwin)
cmake_host_system_information(RESULT host_arch QUERY OS_PLATFORM)  # AMD64 / x86_64 / aarch64 / arm64
string(TOLOWER "${host_arch}" host_arch)

if(host_os STREQUAL "Windows" AND host_arch MATCHES "^(amd64|x86_64)$")
  set(platform win_amd64)
elseif(host_os STREQUAL "Linux" AND host_arch MATCHES "^(x86_64|amd64)$")
  set(platform linux_x86_64)
elseif(host_os STREQUAL "Linux" AND host_arch MATCHES "^(aarch64|arm64)$")
  set(platform linux_aarch64)
elseif(host_os MATCHES "^(macOS|Darwin)$" AND host_arch STREQUAL "arm64")
  set(platform macos_arm64)
elseif(host_os MATCHES "^(macOS|Darwin)$" AND host_arch STREQUAL "x86_64")
  set(platform macos_x86_64)
else()
  message(FATAL_ERROR
    "No prebuilt clang-format/clang-tidy ${LLVM_VERSION} for ${host_os} ${host_arch}.\n"
    "Install LLVM ${LLVM_VERSION} another way (e.g. https://github.com/llvm/llvm-project/releases).")
endif()

if(platform STREQUAL "win_amd64")
  set(exe_suffix ".exe")
else()
  set(exe_suffix "")
endif()

# 이미 같은 버전이 있으면 건너뛴다.
function(installed_version tool out_var)
  set(exe "${INSTALL_DIR}/bin/${tool}${exe_suffix}")
  set(version "")
  if(EXISTS "${exe}")
    execute_process(COMMAND "${exe}" --version OUTPUT_VARIABLE text ERROR_QUIET RESULT_VARIABLE result)
    if(result EQUAL 0 AND text MATCHES "version ([0-9.]+)")
      set(version "${CMAKE_MATCH_1}")
    endif()
  endif()
  set(${out_var} "${version}" PARENT_SCOPE)
endfunction()

installed_version(clang-format format_version)
installed_version(clang-tidy tidy_version)
if(NOT FORCE AND format_version STREQUAL LLVM_VERSION AND tidy_version STREQUAL LLVM_VERSION)
  message(STATUS "clang-format / clang-tidy ${LLVM_VERSION} are already installed in ${INSTALL_DIR}")
  message(STATUS "(use -DFORCE=ON to reinstall)")
  return()
endif()

# --- 2), 3) 내려받기 → 해시 확인 → 풀기 ---
function(install_wheel tool wheel_info)
  list(GET wheel_info 0 path)
  list(GET wheel_info 1 sha256)
  get_filename_component(file_name "${path}" NAME)
  set(wheel "${TOOLS_DIR}/downloads/${file_name}")
  set(unpack "${TOOLS_DIR}/unpack-${tool}")

  message(STATUS "Downloading ${tool} ${LLVM_VERSION} (${platform}) ...")
  file(DOWNLOAD "${PYPI_FILES}/${path}" "${wheel}"
       EXPECTED_HASH SHA256=${sha256} TLS_VERIFY ON SHOW_PROGRESS STATUS status)
  list(GET status 0 code)
  if(NOT code EQUAL 0)
    list(GET status 1 reason)
    file(REMOVE "${wheel}")
    message(FATAL_ERROR "Download of ${tool} failed: ${reason}")
  endif()

  # 휠 안의 <모듈>/data/ 아래에 bin/(실행 파일)과 lib/clang/<버전>/include(내장 헤더)가 있다.
  file(REMOVE_RECURSE "${unpack}")
  file(ARCHIVE_EXTRACT INPUT "${wheel}" DESTINATION "${unpack}")
  file(GLOB data_dir LIST_DIRECTORIES true "${unpack}/*/data")
  if(NOT IS_DIRECTORY "${data_dir}")
    message(FATAL_ERROR "Unexpected wheel layout: ${file_name}")
  endif()
  file(COPY "${data_dir}/" DESTINATION "${INSTALL_DIR}")

  # zip 에서 실행 권한이 빠질 수 있으므로 다시 준다 (Linux/macOS).
  if(NOT exe_suffix)
    file(CHMOD "${INSTALL_DIR}/bin/${tool}"
         PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  endif()

  file(REMOVE_RECURSE "${unpack}")
  file(REMOVE "${wheel}")
endfunction()

install_wheel(clang-format "${FORMAT_${platform}}")
install_wheel(clang-tidy "${TIDY_${platform}}")
file(REMOVE_RECURSE "${TOOLS_DIR}/downloads")

installed_version(clang-format format_version)
installed_version(clang-tidy tidy_version)
if(NOT format_version STREQUAL LLVM_VERSION OR NOT tidy_version STREQUAL LLVM_VERSION)
  message(FATAL_ERROR "Installed tools do not run (clang-format '${format_version}', clang-tidy '${tidy_version}')")
endif()

message(STATUS "Installed clang-format ${format_version} and clang-tidy ${tidy_version} into ${INSTALL_DIR}/bin")
message(STATUS "Next: configure again (cmake --preset <preset>) so CMake picks them up.")
