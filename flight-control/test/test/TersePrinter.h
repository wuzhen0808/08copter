
#include <fstream>
#include <gtest/gtest.h>
#include <stdio.h>

using ::std::ofstream;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
namespace test {

class TersePrinter : public EmptyTestEventListener {
public:
    TersePrinter() {
        out = fopen("test.log", "w");
    }
    FILE *out;

private:
    // Called before any test activity starts.
    void OnTestProgramStart(const UnitTest & /* unit_test */) override {}

    // Called after all test activities have ended.
    void OnTestProgramEnd(const UnitTest &unit_test) override {
        fprintf(out, "TEST %s\n", unit_test.Passed() ? "PASSED" : "FAILED");
        fflush(out);
    }

    // Called before a test starts.
    void OnTestStart(const TestInfo &test_info) override {
        fprintf(out, "*** Test %s.%s starting.\n", test_info.test_suite_name(),
                test_info.name());
        fflush(out);
    }

    // Called after a failed assertion or a SUCCEED() invocation.
    void OnTestPartResult(const TestPartResult &test_part_result) override {
        fprintf(out, "%s in %s:%d\n%s\n%s\n",
                test_part_result.failed() ? "*** Failure" : "Success",
                test_part_result.file_name(), test_part_result.line_number(),
                test_part_result.summary(),
                test_part_result.message());
        fflush(out);
    }

    // Called after a test ends.
    void OnTestEnd(const TestInfo &test_info) override {
        fprintf(stdout, "*** Test %s.%s ending.\n", test_info.test_suite_name(),
                test_info.name());
        fflush(stdout);
    }
}; // class TersePrinter

} // namespace test