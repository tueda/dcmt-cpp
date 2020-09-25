# Define the "bench" target.
add_custom_target(
  bench
  COMMAND test-bench.exe
  COMMENT "Running benchmark tests")

# Dependencies.

CPMAddPackage(
  NAME nanobench
  VERSION 4.2.0
  GITHUB_REPOSITORY martinus/nanobench)
target_compile_features(nanobench PUBLIC cxx_std_11)

# Add a benchmark.
function(do_bench testfile)
  string(REPLACE ".cpp" "" testname ${testfile})
  set(testexe "${testname}.exe")
  add_executable(${testexe} ${ARGV})
  target_link_libraries(${testexe} PRIVATE dcmt)
  target_link_libraries(${testexe} PRIVATE nanobench)
  add_dependencies(bench ${testexe})
endfunction()

# Benchmarking.

do_bench(test-bench.cpp)
do_bench(test-complexity.cpp) # NOTE: running this takes time!
