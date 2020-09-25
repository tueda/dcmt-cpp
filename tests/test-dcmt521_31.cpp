#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <dcmt/dcmt.h>
#include <doctest/doctest.h>

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>

using dcmt521_31 = dcmt::dcmt_engine<std::uint_fast32_t, 31, 521>;

using std::array;
using std::cout;
using std::endl;
using std::generate;
using std::hex;
using std::setw;
using std::size_t;

const int n = 100;

const array<dcmt521_31::result_type, n> expected_31_521_0_4172_3241{
    0x4cfa5245U, 0x42326ae0U, 0x0034b07dU, 0x18daf0f2U, 0x3cdac286U,
    0x480158b0U, 0x179d2fffU, 0x295fe99aU, 0x010047c7U, 0x349b5d3fU,
    0x2c8455f7U, 0x7ed3521eU, 0x214eaf8eU, 0x2bdddac6U, 0x39e19114U,
    0x16a8fa3bU, 0x3b7ff9c7U, 0x3f0e5028U, 0x13cc7c57U, 0x73d2b44dU,
    0x1572bfc7U, 0x0da0e97bU, 0x2c8f609cU, 0x57a09d0dU, 0x3b7da72cU,
    0x1749de57U, 0x1160d37eU, 0x745a7f52U, 0x6b6d8865U, 0x0728fb78U,
    0x39fa9b70U, 0x5f68ae48U, 0x7a4b67e3U, 0x661ec5e4U, 0x65a870c1U,
    0x5a19e528U, 0x091a351cU, 0x3f29f920U, 0x1ed22653U, 0x24872a37U,
    0x0adbde5fU, 0x5fdd1e7cU, 0x264b3e7fU, 0x1b1eb631U, 0x2efdd8b0U,
    0x0ebadd2aU, 0x00752635U, 0x6b6af92bU, 0x596f4d0dU, 0x3450e352U,
    0x5c674ffdU, 0x60f0ae05U, 0x659c7fd9U, 0x4e2c4ca6U, 0x2bb09892U,
    0x7fb508beU, 0x38247ef6U, 0x67740d77U, 0x1b1d6495U, 0x5eef7db0U,
    0x61e0fbeaU, 0x79d00342U, 0x52770de0U, 0x17943ad8U, 0x793c5378U,
    0x45c466fbU, 0x139474ebU, 0x2f3d756eU, 0x3a2b9cdbU, 0x55f75be7U,
    0x63610620U, 0x3cabf9aeU, 0x0c7fb89eU, 0x17945935U, 0x26ec5f72U,
    0x10905088U, 0x7c657864U, 0x4405e7d8U, 0x2f9dcce8U, 0x3c7e2c9aU,
    0x1a487610U, 0x39d583e5U, 0x26becb31U, 0x44b8f4f6U, 0x42f17f08U,
    0x2782a337U, 0x107cf306U, 0x7ebf5c81U, 0x3ba3f2a6U, 0x610b8a94U,
    0x715e3aeeU, 0x27faca9aU, 0x3df56de9U, 0x35ac58f9U, 0x6fa8d894U,
    0x71552e87U, 0x7879f9f3U, 0x238de2f0U, 0x46746d12U, 0x7c037dfcU,
};

TEST_CASE("properties") {
  CHECK_EQ(dcmt521_31::word_size, 31);
  CHECK_EQ(dcmt521_31::prime_exponent, 521);
  CHECK_EQ(dcmt521_31::min(), 0);
  CHECK_EQ(dcmt521_31::max(), 0x7fffffffU);
}

TEST_CASE("default ctor") {
  dcmt521_31 rng{};
  array<dcmt521_31::result_type, n> a;
  generate(begin(a), end(a), rng);

  for (int i = 0; i < n; i++) {
    CHECK_MESSAGE(a[i] == expected_31_521_0_4172_3241[i], "i == " << i);
  }
}
