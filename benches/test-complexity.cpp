#define ANKERL_NANOBENCH_IMPLEMENT
#include <dcmt/dcmt.h>
#include <nanobench.h>

#include <random>

using ankerl::nanobench::Bench;
using ankerl::nanobench::doNotOptimizeAway;
using dcmt::dcmt_param;

template <int p>
void bench_init_complexity(Bench& bench) {
  int id = 0;
  bench.complexityN(p).run("dcmt::dcmt_param<p = N>", [&] {
    dcmt::dcmt_param<std::uint_fast32_t, 32, p> param(id);
    if (++id > 65535) {
      id = 0;
    }
    doNotOptimizeAway(id);
    doNotOptimizeAway(param);
  });
}

int main() {
  Bench b;
  b.title("asymptotic complexity");
  bench_init_complexity<521>(b);
  bench_init_complexity<607>(b);
  bench_init_complexity<1279>(b);
  bench_init_complexity<2203>(b);
  bench_init_complexity<2281>(b);
  bench_init_complexity<3217>(b);
  bench_init_complexity<4253>(b);
  bench_init_complexity<4423>(b);
  std::cout << b.complexityBigO() << std::endl;  // expected to be O(p^3)

  return 0;
}
