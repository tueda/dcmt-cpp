# Dependencies.

CPMAddPackage(
  NAME dcmt
  VERSION 0.6.2
  GITHUB_REPOSITORY MersenneTwister-Lab/dcmt
  GIT_TAG 214aaca2425fd834256d7c0bf9f7eabf25ea211b
  DOWNLOAD_ONLY True)

# Library.

add_library(dcmt dcmt.cpp)
target_compile_features(dcmt PUBLIC cxx_std_11)
target_include_directories(dcmt PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(dcmt PRIVATE ${dcmt_SOURCE_DIR}/include)
target_include_directories(dcmt PRIVATE ${dcmt_SOURCE_DIR}/lib)
target_link_libraries(dcmt PRIVATE ${PROJECT_CONFIG_LIB})
