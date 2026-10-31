#include "TestFramework.h"

#include <cstdio>
#include <cstring>
#include <exception>

namespace test {

namespace {

int failuresInCurrentTest = 0;

}  // namespace

std::vector<TestCase>& registry()
{
    static std::vector<TestCase> tests;
    return tests;
}

void reportFailure(const char* file, int line, const std::string& message)
{
    ++failuresInCurrentTest;
    std::printf("    FAILED %s:%d: %s\n", file, line, message.c_str());
}

}  // namespace test

int main(int argc, char** argv)
{
    const char* filter = argc > 1 ? argv[1] : nullptr;
    int run = 0;
    int failed = 0;

    for (const test::TestCase& testCase : test::registry()) {
        if (filter != nullptr && std::strstr(testCase.name, filter) == nullptr) {
            continue;
        }
        ++run;
        test::failuresInCurrentTest = 0;
        try {
            testCase.function();
        } catch (const std::exception& e) {
            test::reportFailure(__FILE__, __LINE__, std::string("unexpected exception: ") + e.what());
        }
        const bool passed = test::failuresInCurrentTest == 0;
        std::printf("[%s] %s\n", passed ? " OK " : "FAIL", testCase.name);
        if (!passed) {
            ++failed;
        }
    }

    std::printf("\n%d test(s) run, %d failed\n", run, failed);
    if (run == 0) {
        std::printf("no test matched the filter\n");
        return 1;
    }
    return failed == 0 ? 0 : 1;
}
