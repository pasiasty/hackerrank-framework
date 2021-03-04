#include <experimental/filesystem>
#include <fstream>
#include <future>
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <unordered_set>

#include "solution.hpp"

#ifndef TESTS_TIMEOUT_IN_SECONDS
#define TESTS_TIMEOUT_IN_SECONDS 10
#endif

#ifndef ROOT_DIR
#define ROOT_DIR "."
#endif

namespace fs = std::experimental::filesystem;

class EndToEndTestsFixture : public testing::TestWithParam<std::string> {};

TEST_P(EndToEndTestsFixture, OutputAsExpected) {
  std::string test_name = GetParam();
  auto dir = fs::path(ROOT_DIR) / fs::path("test_cases");
  fs::path input_file(test_name + "_in.txt");
  fs::path output_file(test_name + "_out.txt");

  fs::path full_input_file_path = dir / input_file;
  fs::path full_output_file_path = dir / output_file;

  if (!fs::exists(full_input_file_path)) {
    FAIL() << "Full input path (" << full_input_file_path << ") does not exist";
  }
  if (!fs::exists(full_output_file_path)) {
    FAIL() << "Full output path (" << full_output_file_path << ") does not exist";
  }

  std::ifstream in(full_input_file_path.string());

  std::promise<std::string> p;

  std::thread t([&in, &p, &test_name]() {
#if defined(__APPLE__) || defined(__linux) || defined(__unix) || defined(__posix)
    pthread_setname_np(pthread_self(), ("test_thread_" + test_name).c_str());
#endif

    std::ostringstream out;
    solution(in, out);
    p.set_value(out.str());
  });

  auto f = p.get_future();
  auto st = f.wait_for(std::chrono::seconds(TESTS_TIMEOUT_IN_SECONDS));
  if (st != std::future_status::ready) {
#if defined(__APPLE__) || defined(__linux) || defined(__unix) || defined(__posix)
    pthread_cancel(t.native_handle());
    t.join();
#else
    t.detach();
#endif
    FAIL() << "Test timed out";
  }

  t.join();
  auto res = f.get();

  std::ifstream output_file_stream(full_output_file_path.string());
  std::string exp_res;

  std::ostringstream exp_res_ss;
  exp_res_ss << output_file_stream.rdbuf();
  exp_res = exp_res_ss.str();

  if (exp_res != res) {
    std::cout << "Want:" << std::endl << exp_res << std::endl;
    std::cout << "Got:" << std::endl << res << std::endl;
  }

  EXPECT_EQ(exp_res, res);
}

std::unordered_set<std::string> ReadLinesFromFile(std::string fname) {
  std::ifstream in(fname);
  std::unordered_set<std::string> res;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty())
      continue;
    res.insert(line);
  }
  return res;
}

std::unordered_set<std::string> Whitelist() {
  return ReadLinesFromFile("tests_whitelist.txt");
}

std::unordered_set<std::string> Blacklist() {
  return ReadLinesFromFile("tests_blacklist.txt");
}

std::set<std::string> GetTests() {
  std::set<std::string> res;

  auto whitelist = Whitelist();
  auto blacklist = Blacklist();

  for (const auto &entry : fs::directory_iterator(fs::path(ROOT_DIR) / fs::path("test_cases"))) {
    std::string path = entry.path();
    auto beg_idx = path.rfind('/') + 1;
    auto end_idx = path.rfind('_');

    std::string name = path.substr(beg_idx, end_idx - beg_idx);
    if (name.starts_with("disabled_"))
      continue;

    if ((whitelist.empty() || whitelist.contains(name)) && !blacklist.contains(name))
      res.insert(name);
  }

  return res;
}

INSTANTIATE_TEST_SUITE_P(EndToEndTests, EndToEndTestsFixture, testing::ValuesIn(GetTests()));
