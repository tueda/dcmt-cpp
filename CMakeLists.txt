cmake_minimum_required(VERSION 3.14)

# Detect if the project is the root or being bundled.

if(NOT DEFINED PROJECT_NAME)
  set(IS_ROOT_PROJECT ON)
endif()

# Project information.

project(
  dcmt-cpp
  VERSION 0.0.1
  LANGUAGES CXX)

# CPM (https://github.com/TheLartians/CPM.cmake).

set(CPM_DOWNLOAD_VERSION 0.27.3)
include(cmake/get_cpm.cmake)

# Root project settings.

if(IS_ROOT_PROJECT)
  include(CTest)

  set(PROJECT_CONFIG_LIB "${PROJECT_NAME}_private_config")
  add_library(${PROJECT_CONFIG_LIB} INTERFACE)

  # Additional compiler flags for testing.

  set(CMAKE_CXX_EXTENSIONS OFF)

  include(CheckCXXCompilerFlag)

  macro(TRY_CXX_COMPILER_FLAG flag var)
    check_cxx_compiler_flag(${flag} ${var})
    if(${var})
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
    endif()
  endmacro()

  function(check_cxx_compiler_and_linker_flag flag var)
    set(separated_flags ${flag})
    # cmake-lint: disable=E1120
    separate_arguments(separated_flags)
    # cmake-lint: disable=C0103
    set(CMAKE_REQUIRED_LINK_OPTIONS ${separated_flags})
    check_cxx_compiler_flag(${flag} ${var})
  endfunction()

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    try_cxx_compiler_flag("-Wall -Wextra" CXX_HAS_WARN_ALL_AND_EXTRA_FLAG)
    try_cxx_compiler_flag("-Wshadow" CXX_HAS_WARN_SHADOW_FLAG)
  endif()

  # Benchmarking.

  option(BUILD_BENCHMARKING "Build benchmarks" OFF)

  # Document generation.

  option(BUILD_DOCUMENTS "Build documents" ON)

  if(BUILD_DOCUMENTS)
    find_package(Doxygen)

    if(DOXYGEN_FOUND)
      add_subdirectory(docs)
    else()
      message(WARNING "Building documents requires Doxygen")
    endif()
  endif()

  # Code coverage.

  option(USE_CODE_COVERAGE "Enable coverage reporting" OFF)

  if(USE_CODE_COVERAGE)
    check_cxx_compiler_and_linker_flag("-O0 -g -coverage" CXX_HAS_COVERAGE_FLAG)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      if(CXX_HAS_COVERAGE_FLAG)
        target_compile_options(${PROJECT_CONFIG_LIB} INTERFACE -O0 -g -coverage)
        target_link_options(${PROJECT_CONFIG_LIB} INTERFACE -coverage)
      else()
        message(WARNING "Coverage reporting is not supported by the compiler")
      endif()
    else()
      message(WARNING "Coverage reporting requires GCC or LLVM/Clang")
    endif()
  endif()

  # Compiler sanitizers.

  option(USE_SANITIZER "Enable compiler sanitizers" OFF)

  if(USE_SANITIZER)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      check_cxx_compiler_and_linker_flag(
        "-O0 -g -fsanitize=address,undefined -fno-omit-frame-pointer"
        CXX_HAS_SANITIZER_FLAG)
      if(CXX_HAS_SANITIZER_FLAG)
        target_compile_options(
          ${PROJECT_CONFIG_LIB} INTERFACE -O0 -g -fsanitize=address,undefined
                                          -fno-omit-frame-pointer)
        target_link_options(${PROJECT_CONFIG_LIB} INTERFACE
                            -fsanitize=address,undefined)
      else()
        message(WARNING "Compiler sanitizers are not supported by the compiler")
      endif()
    else()
      message(WARNING "Compiler sanitizers require GCC or LLVM/Clang")
    endif()
  endif()

  # Clang-Tidy.

  option(USE_CLANG_TIDY "Enable Clang-Tidy" OFF)

  if(USE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
  endif()
endif()

# Source code.

add_subdirectory(src)

if(IS_ROOT_PROJECT AND BUILD_TESTING)
  add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()

if(IS_ROOT_PROJECT AND BUILD_BENCHMARKING)
  add_subdirectory(benches)
endif()
