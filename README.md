dcmt-cpp
========

A C++11 wrapper library for
[Dynamic Creator of Mersenne Twisters](https://github.com/MersenneTwister-Lab/dcmt).


Example
-------

```cpp
#include <iostream>
#include <dcmt/dcmt.h>

int main() {
  // Create 32-bit Mersenne Twister pseudo-random number generator
  // engines with a period 2^521-1. Their characteristic polynomials are
  // determined with id = 0, 1, 999 and generated sequences are highly
  // independent. The generator engines are initialized with seed =
  // 1234, 4567, 8901, respectively.
  dcmt::dcmt521 rng1{dcmt::dcmt521::param_type{0}, 1234};
  dcmt::dcmt521 rng2{dcmt::dcmt521::param_type{1}, 4567};
  dcmt::dcmt521 rng3{dcmt::dcmt521::param_type{999}, 8901};

  // Generate a number by using each generator engine.
  std::cout << rng1() << std::endl;
  std::cout << rng2() << std::endl;
  std::cout << rng3() << std::endl;

  return 0;
}
```


### CMake (3.14 or later)

```cmake
add_subdirectory(dcmt-cpp)
target_link_libraries(your_app PRIVATE dcmt)
```


Development
-----------

```bash
# Install prerequisites (minimal).
brew install cmake gcc git

# Install prerequisites (including optional ones).
brew install cmake doxygen gcc git lcov llvm pre-commit

# Install pre-commit hooks.
pre-commit install
pre-commit install --hook-type commit-msg

# Set CPM source cache.
export CPM_SOURCE_CACHE=$HOME/.cache/CPM  # for Linux
export CPM_SOURCE_CACHE=$HOME/Library/Caches/CPM  # for macOS

# Run linters.
pre-commit run --all-files

# Tests.
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug --target check

# Benchmarking.
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release -DBUILD_BENCHMARKING=ON
NANOBENCH_SUPPRESS_WARNINGS=1 cmake --build build/release --target bench

# Documents.
cmake -S . -B build/docs -DBUILD_TESTING=OFF
cmake --build build/docs --target doc

# Code coverage.
cmake -S . -B build/coverage -DCMAKE_BUILD_TYPE=Debug -DUSE_CODE_COVERAGE=ON
lcov -z -d build/coverage
cmake --build build/coverage --target check
lcov -c -d build/coverage -o build/coverage/coverage.info
lcov -r build/coverage/coverage.info '*/c++/*' -o build/coverage/coverage.info
lcov -r build/coverage/coverage.info '*/lib/*' -o build/coverage/coverage.info
lcov -r build/coverage/coverage.info '*/doctest/*' -o build/coverage/coverage.info
lcov -r build/coverage/coverage.info '*/tests/*' -o build/coverage/coverage.info
genhtml -o build/coverage/html build/coverage/coverage.info

# Compiler sanitizers.
cmake -S . -B build/sanitizer -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=ON
cmake --build build/sanitizer --target check

# Clang-Tidy.
cmake -S . -B build/clang-tidy -DCMAKE_BUILD_TYPE=Debug -DUSE_CLANG_TIDY=ON
cmake --build build/clang-tidy --target check
```


License
-------

[MIT](https://github.com/tueda/dcmt-cpp/blob/master/LICENSE)
