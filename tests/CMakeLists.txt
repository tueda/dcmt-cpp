# Define the "check" target. Use "make check" instead of "make test"; the latter
# is known not to update executables.
# https://gitlab.kitware.com/cmake/cmake/-/issues/8774
add_custom_target(
  check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  COMMENT "Build and run tests")

# Dependencies.

CPMAddPackage(
  NAME doctest
  GITHUB_REPOSITORY onqtam/doctest
  GIT_TAG 2.4.0)

# Add a test.
function(do_test testfile)
  string(REPLACE ".cpp" "" testname ${testfile})
  set(testexe "${testname}.exe")
  add_executable(${testexe} ${ARGV})
  target_link_libraries(${testexe} PRIVATE dcmt)
  target_link_libraries(${testexe} PRIVATE doctest)
  target_link_libraries(${testexe} PRIVATE ${PROJECT_CONFIG_LIB})
  add_test(${testname} ${testexe})
  add_dependencies(check ${testexe})
  # Tests (except test-expand.cpp) are in general dirty. Excluded from static
  # code analysis.
  if(NOT (${testname} STREQUAL "test-expand"))
    set_target_properties(${testexe} PROPERTIES CXX_CLANG_TIDY "")
  endif()
endfunction()

# Tests.

do_test(test-expand.cpp)
do_test(test-dcmt521.cpp)
do_test(test-dcmt521_31.cpp)
do_test(test-readme.cpp)
do_test(test-new_example2.cpp)
