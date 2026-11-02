// 에셋(셰이더, 모델) 파일 경로를 만든다.
#pragma once

#include <string>

// assets/ 폴더 기준 상대 경로를 절대 경로로 바꾼다. 예) assetPath("models/piston.obj")
// 기준 폴더는 CMake 가 VE_ASSET_DIR 로 넣어 주므로(src/core/CMakeLists.txt) 어디서 실행해도 같은 파일을 찾는다.
// 주의: 빌드한 PC 의 소스 폴더를 가리킨다. 실행 파일만 다른 PC 로 옮기면 에셋을 찾지 못한다.
std::string assetPath(const std::string& relativePath);
