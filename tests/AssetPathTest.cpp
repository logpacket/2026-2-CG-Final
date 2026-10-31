// assetPath 단위 테스트: CMake 가 넣어 준 에셋 폴더가 실제 assets/ 를 가리키는지 확인한다.
#include "core/AssetPath.h"

#include "TestFramework.h"

#include <filesystem>

TEST_CASE(assetPathPointsIntoAssetsFolder)
{
    CHECK(std::filesystem::is_directory(assetPath("models")));
    CHECK(std::filesystem::is_directory(assetPath("shaders")));
}
