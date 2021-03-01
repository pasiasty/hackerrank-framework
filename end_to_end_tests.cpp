#include <gtest/gtest.h>
#include <experimental/filesystem>
#include <fstream>
#include <string>

#include "solution.hpp"

namespace fs = std::experimental::filesystem;

class EndToEndTestsFixture : public testing::TestWithParam<std::string>
{
};

TEST_P(EndToEndTestsFixture, OutputAsExpected)
{
    testing::PrintToString<int>(1);

    std::string test_name = GetParam();
    fs::path dir("test_cases");
    fs::path input_file(test_name + "_in.txt");
    fs::path output_file(test_name + "_out.txt");

    fs::path full_input_file_path = dir / input_file;
    fs::path full_output_file_path = dir / output_file;

    std::ostringstream out;
    std::ifstream in(full_input_file_path.string());

    solution(in, out);

    std::string res = out.str();
    std::ifstream output_file_stream(full_output_file_path.string());
    std::string exp_res;

    std::ostringstream exp_res_ss;
    exp_res_ss << output_file_stream.rdbuf(); // reading data
    exp_res = exp_res_ss.str();

    if (exp_res != res)
    {
        std::cout << "Want:" << std::endl
                  << exp_res << std::endl;
        std::cout << "Got:" << std::endl
                  << res << std::endl;
    }

    EXPECT_EQ(exp_res, res);
}

std::set<std::string> GetTests()
{
    std::set<std::string> res;

    for (const auto &entry : fs::directory_iterator("test_cases"))
    {
        std::string path = entry.path();
        auto beg_idx = path.find('/') + 1;
        auto end_idx = path.rfind('_');

        std::string name = path.substr(beg_idx, end_idx - beg_idx);
        res.insert(name);
    }

    return res;
}

INSTANTIATE_TEST_CASE_P(
    EndToEndTests,
    EndToEndTestsFixture,
    testing::ValuesIn(GetTests()));
