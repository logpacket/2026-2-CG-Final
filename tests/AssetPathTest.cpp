#include "core/AssetPath.h"

#include "TestFramework.h"

#include <filesystem>

TEST_CASE(assetPathPointsIntoAssetsFolder)
{
    CHECK(std::filesystem::is_directory(assetPath("models")));
    CHECK(std::filesystem::is_directory(assetPath("shaders")));
}
