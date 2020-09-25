#define ANKERL_NANOBENCH_IMPLEMENT
#include <dcmt/dcmt.h>
#include <nanobench.h>

#include <random>

using ankerl::nanobench::Bench;
using ankerl::nanobench::doNotOptimizeAway;
using dcmt::dcmt521;
using std::mt19937;
using std::mt19937_64;
using std::normal_distribution;
using std::uniform_real_distribution;

#define bench_init(bench, rng, name) \
  do {                               \
    bench.run(name, [&]() {          \
      auto r = rng;                  \
      doNotOptimizeAway(r);          \
    });                              \
  } while (false)

template <typename Rng>
void bench_gen_raw(Bench& bench, const Rng& rng, const char* name) {
  Rng r(rng);
  bench.run(name, [&]() {
    auto x = r();
    doNotOptimizeAway(x);
  });
}

template <typename Rng>
void bench_gen_double(Bench& bench, const Rng& rng, const char* name) {
  Rng r(rng);
  auto dist = uniform_real_distribution<>();
  bench.run(name, [&]() {
    auto x = dist(r);
    doNotOptimizeAway(x);
  });
}

template <typename Rng>
void bench_gen_gauss(Bench& bench, const Rng& rng, const char* name) {
  Rng r(rng);
  auto dist = normal_distribution<>();
  bench.run(name, [&]() {
    auto x = dist(r);
    doNotOptimizeAway(x);
  });
}

#define bench_set(b, bench_func)                    \
  do {                                              \
    bench_func(b, dcmt521(), "dcmt::dcmt521");      \
    bench_func(b, mt19937(), "std::mt19937");       \
    bench_func(b, mt19937_64(), "std::mt19937_64"); \
  } while (false)

int main() {
  Bench b;

  b.title("init");
  bench_set(b, bench_init);

  b.relative(true).warmup(1000).clockResolutionMultiple(100000);

  b.title("gen_raw");
  bench_set(b, bench_gen_raw);

  b.title("gen_double");
  bench_set(b, bench_gen_double);

  b.title("gen_gauss");
  bench_set(b, bench_gen_gauss);

  return 0;
}
