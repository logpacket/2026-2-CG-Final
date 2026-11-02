#include "core/AssetPath.h"

std::string assetPath(const std::string& relativePath)
{
    return std::string(VE_ASSET_DIR) + "/" + relativePath;
}
