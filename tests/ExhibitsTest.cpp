#include "exhibition/exhibits/Exhibits.h"

#include "core/AssetPath.h"
#include "TestFramework.h"

#include <filesystem>

// 목록의 경로 오타는 실행해도 상자로 대신 그려져 놓치기 쉬우므로 여기서 잡는다.
TEST_CASE(exhibitSpecsPointToExistingModels)
{
    for (const ExhibitSpec& spec : exhibitSpecs()) {
        if (!std::filesystem::is_regular_file(assetPath(spec.objPath))) {
            test::reportFailure(__FILE__, __LINE__, spec.objPath + " does not exist");
        }
    }
}
