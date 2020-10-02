#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <dcmt/dcmt.h>
#include <doctest/doctest.h>

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>

using dcmt::dcmt521;
using std::array;
using std::generate;
using std::stringstream;

const int n = 100;

const array<dcmt521::result_type, n> expected_32_521_0_4172_3241{
    0x4def2d53U, 0x4284dd16U, 0x97e05113U, 0x3180fe45U, 0xef6dccc5U,
    0xfa19d824U, 0xcb3393d4U, 0xe88a7e32U, 0x8815ac11U, 0x7326765bU,
    0x8f639a75U, 0xdd2a0752U, 0xa97518b0U, 0x3cec73dfU, 0xed4ed842U,
    0xc30b947cU, 0x86faecbaU, 0x06787d11U, 0x495698f2U, 0x49e68a66U,
    0x93867996U, 0xe80d33bfU, 0xb91c64cfU, 0x2fde66eeU, 0x3fb734b9U,
    0x5904020cU, 0x967baf87U, 0x29490af9U, 0xc44dfadaU, 0x1422660dU,
    0xcd03cfd4U, 0x7bea482dU, 0x1c278092U, 0x33c2cba3U, 0x35455c0fU,
    0xf1bb44d8U, 0xd2436ec0U, 0xb363bd2fU, 0x7d68ff1bU, 0x0fd0c3acU,
    0xfd3fc8d2U, 0x67347ad0U, 0x3d8b8f7dU, 0x4399434dU, 0x6e9e50b5U,
    0x1e3bc81cU, 0x8abedd12U, 0x7528fe20U, 0xb3057114U, 0xeffb376aU,
    0x3c233abaU, 0x5bf7fe59U, 0xbba87469U, 0xa91599ecU, 0x9fe1380aU,
    0x89f14b0dU, 0x186e70ffU, 0x09d1b88fU, 0x11ec92a0U, 0x3320fe0fU,
    0xc44e20deU, 0x5bc7cb5bU, 0xc59a1f51U, 0xf0c09c97U, 0xda55d9aeU,
    0xe3e73cb7U, 0x9ca0fb0aU, 0xc8f538b4U, 0xdee6807fU, 0xdc14bdbcU,
    0xea3cdcdcU, 0x9545b37eU, 0xa1e4874cU, 0x042cededU, 0x05128c27U,
    0x01c91173U, 0x62f29c93U, 0x844f62e0U, 0xd5fb909cU, 0xe9eebc3bU,
    0xffc8c4e0U, 0xaf96a417U, 0xc977acaaU, 0x07fff398U, 0x6e50a176U,
    0x0b93f137U, 0x792a3ed0U, 0x9781a1b5U, 0xd55804ccU, 0x85af48eaU,
    0x595313b7U, 0x34321bdbU, 0x9ddb5d46U, 0xd66ee620U, 0x342f0829U,
    0xbda834d0U, 0x387ee323U, 0x3ddf69f4U, 0x06fc97e5U, 0x7a4862e2U,
};

TEST_CASE("properties") {
  CHECK_EQ(dcmt521::word_size, 32);
  CHECK_EQ(dcmt521::prime_exponent, 521);
  CHECK_EQ(dcmt521::min(), 0);
  CHECK_EQ(dcmt521::max(), 0xffffffffU);
}

TEST_CASE("default ctor") {
  dcmt521 rng{};
  array<dcmt521::result_type, n> a;
  generate(begin(a), end(a), rng);

  for (int i = 0; i < n; i++) {
    CHECK_MESSAGE(a[i] == expected_32_521_0_4172_3241[i], "i == " << i);
  }
}

TEST_CASE("ctor(lvalue-param)") {
  dcmt521::param_type param;

  {
    dcmt521 rng{param};
    array<dcmt521::result_type, n> a;
    generate(begin(a), end(a), rng);

    for (int i = 0; i < n; i++) {
      CHECK_MESSAGE(a[i] == expected_32_521_0_4172_3241[i], "i == " << i);
    }
  }

  // discard, comparison operators
  {
    dcmt521 rng1{param};
    dcmt521 rng2{param};

    CHECK_EQ(rng1, rng2);

    rng1.discard(17);  // n = p/w + 1 = 17

    CHECK_NE(rng1, rng2);

    rng1.discard(1);

    CHECK_NE(rng1, rng2);

    for (int i = 0; i < 18; i++) {
      rng2();
    }

    CHECK_EQ(rng1, rng2);
  }

  // serialization
  {
    stringstream buf;

    dcmt521 rng1{param};
    rng1.discard(10);
    buf << rng1;
    rng1.discard(10);

    dcmt521 rng2{param};
    buf >> rng2;
    rng2.discard(10);

    CHECK_EQ(rng1, rng2);
  }

  // self-assignment should be no problem
  {
    dcmt521 rng{param};
    rng = rng;

    array<dcmt521::result_type, n> a;
    generate(begin(a), end(a), rng);

    for (int i = 0; i < n; i++) {
      CHECK_MESSAGE(a[i] == expected_32_521_0_4172_3241[i], "i == " << i);
    }
  }

  // id must be <= 65535
  { CHECK_THROWS_AS(dcmt521::param_type{65536}, std::invalid_argument); }
}
