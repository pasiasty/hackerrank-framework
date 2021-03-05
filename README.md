# Hackerrank C++ Framework

This simple framework will allow you to focus only on coding part while solving [Hackerrank](https://www.hackerrank.com/) problems (or ones coming from similar platforms). It gives you following functionalities:

* create test cases for your program in a very simple way
* temporarily whitelist/blacklist selected test cases while focusing your debugging on certain part of your project
* convenient scripts for simple building/testing your software
* enforcing timeouts on your tests to make sure that your program is performing well
* since all tests are invoked as single binary (without necessity of piping STDIN/STDOUT) you'll be able to debug
  your project in a very straightforward way.

## Prerequisites

This framework should be portable on both Windows and MacOS, however it was tested mostly on Ubuntu. In order to get it working
on your machine you'll need:
* cmake - version 3.16 or newer
* clang - version 10.0.0 or newer
* [GTest](https://github.com/google/googletest) - version 1.10 or newer

It may be useful for you to also install:
* [Visual Studio Code](https://code.visualstudio.com/) - the project contains certain configuration files that may make execution simpler
* clang-format - version 10.0.0 or newer - project contains clang-format style file that provides the used formatting.

## How do I use it?

### Coding

You just need to implement your solution of the problem into [solution.hpp](solution.hpp) file. While developing the solution be sure to use parameters `in` and `out` instead of `std::cin` and `std::cout` - this will allow the framework to test your code properly. While copying your solution back to hackerrank you can just copy the contents of your [solution.hpp](solution.hpp) file and simplistic [main.cpp](main.cpp). That's the only price you'll have to pay for all the features that framework provides you :) .

### Testing

You can run all your tests by launching [scripts/run_e2e.sh](scripts/run_e2e.sh) - this script just configures cmake, builds test target and runs it.

#### Creating test cases

In order to create a test case you just need to provide two files in [test_cases](test_cases) directory. One that would end with `_in.txt` and `_out.txt`. `*_in.txt`  will be directed into `in` stream in your solution, whereas `*_out.txt` will be compared against the data that you'll put into `out` stream. Every test case willl have a name extracted from the files names (e.g. [test_cases/example_in.txt](test_cases/example_in.txt) will result with creation of test case named `example`).

#### Whitelisting testcases

In case you want to execute only selected test cases you can add their names (the part preceeding `_(in|out).txt` suffix) into [tests_whitelist.txt](tests_whitelist.txt).

#### Blacklisting testcases

In case you want to exclude selected test cases from execution you can add their names (the part preceeding `_(in|out).txt` suffix) into [tests_blacklist.txt](tests_blacklist.txt).

### Building

If you want to build all targets you can run [scripts/build_all.sh](scripts/build_all.sh). This will result with confguring cmake and building all targets (one containing tests and main).

### Customizing timeout

Timeout for each test case is defined in [CMakeLists.txt](CMakeLists.txt) by `TESTS_TIMEOUT_IN_SECONDS` parameter. You can change it to any value that you find convenient (however keep in mind that Hackerrank allows only 2 seconds for C++ solutions).