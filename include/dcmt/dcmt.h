#ifndef DCMT_CPP_DCMT_H
#define DCMT_CPP_DCMT_H

#include <cassert>
#include <cstddef>  // std::size_t
#include <cstdint>
#include <istream>
#include <limits>
#include <memory>  // std::unique_ptr
#include <ostream>
#include <type_traits>
#include <utility>  // std::move, std::swap

/**
 * Namespace for classes that wraps the DCMT library.
 */
namespace dcmt {

/**
 * Internal details.
 */
namespace detail {

struct mt_struct {
  std::uint32_t aaa;
  int mm, nn, rr, ww;
  std::uint32_t wmask, umask, lmask;
  int shift0, shift1, shiftB, shiftC;
  std::uint32_t maskB, maskC;
  int i;
  std::uint32_t* state;
};

void* alloc_memory(std::size_t size);
void free_memory(void* ptr);
void free_mt_struct(mt_struct* mts);

inline bool operator==(const mt_struct& a, const mt_struct& b) {
  if (a.aaa != b.aaa || a.mm != b.mm || a.nn != b.nn || a.rr != b.rr ||
      a.ww != b.ww || a.wmask != b.wmask || a.umask != b.umask ||
      a.lmask != b.lmask || a.shift0 != b.shift0 || a.shift1 != b.shift1 ||
      a.shiftB != b.shiftB || a.shiftC != b.shiftC || a.maskB != b.maskB ||
      a.maskC != b.maskC || a.i != b.i) {
    return false;
  }
  for (int i = 0; i < a.nn; i++) {
    if (a.state[i] != b.state[i]) {
      return false;
    }
  }
  return true;
}

template <class CharT, class Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os, const mt_struct& mt) {
  os << mt.aaa << ' ' << mt.mm << ' ' << mt.nn << ' ' << mt.rr << ' ' << mt.ww
     << ' ' << mt.wmask << ' ' << mt.umask << ' ' << mt.lmask << ' '
     << mt.shift0 << ' ' << mt.shift1 << ' ' << mt.shiftB << ' ' << mt.shiftC
     << ' ' << mt.maskB << ' ' << mt.maskC << ' ' << mt.i;
  for (int i = 0; i < mt.nn; i++) {
    os << ' ' << mt.state[i];
  }
  return os;
}

template <class CharT, class Traits>
inline std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is, mt_struct& mt) {
  mt_struct tmp;  // NOLINT(cppcoreguidelines-pro-type-member-init)

  is >> tmp.aaa >> std::ws >> tmp.mm >> std::ws >> tmp.nn >> std::ws >>
      tmp.rr >> std::ws >> tmp.ww >> std::ws >> tmp.wmask >> std::ws >>
      tmp.umask >> std::ws >> tmp.lmask >> std::ws >> tmp.shift0 >> std::ws >>
      tmp.shift1 >> std::ws >> tmp.shiftB >> std::ws >> tmp.shiftC >> std::ws >>
      tmp.maskB >> std::ws >> tmp.maskC >> std::ws >> tmp.i;

  tmp.state =
      static_cast<std::uint32_t*>(alloc_memory(sizeof(std::uint32_t) * tmp.nn));

  for (int i = 0; i < mt.nn; i++) {
    is >> std::ws >> tmp.state[i];
  }

  std::swap(tmp, mt);

  free_memory(tmp.state);

  return is;
}

struct mt_struct_deleter {
  void operator()(mt_struct* ptr) { free_mt_struct(ptr); }
};

class dcmt_engine_impl {
 public:
  dcmt_engine_impl(int w, int p, int id, std::uint32_t seed);
  dcmt_engine_impl(const dcmt_engine_impl& impl);  // deep copy
  dcmt_engine_impl(dcmt_engine_impl&&) noexcept = default;
  ~dcmt_engine_impl() = default;
  dcmt_engine_impl& operator=(const dcmt_engine_impl& impl);  // deep copy
  dcmt_engine_impl& operator=(dcmt_engine_impl&&) noexcept = default;

  void sgenrand(std::uint32_t seed);
  std::uint32_t genrand();
  void discard(unsigned long long z);  // NOLINT(google-runtime-int)

  friend bool operator==(const dcmt_engine_impl& a, const dcmt_engine_impl& b) {
    // See https://bugs.llvm.org/show_bug.cgi?id=28480
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(a.ptr_);
    // See https://bugs.llvm.org/show_bug.cgi?id=28480
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(b.ptr_);
    return *a.ptr_ == *b.ptr_;
  }

  template <class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits>& operator<<(
      std::basic_ostream<CharT, Traits>& os, const dcmt_engine_impl& impl) {
    // See https://bugs.llvm.org/show_bug.cgi?id=28480
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(impl.ptr_);
    return os << *impl.ptr_;
  }

  template <class CharT, class Traits>
  friend std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& is, dcmt_engine_impl& impl) {
    // See https://bugs.llvm.org/show_bug.cgi?id=28480
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(impl.ptr_);
    return is >> *impl.ptr_;
  }

 private:
  std::unique_ptr<mt_struct, mt_struct_deleter> ptr_;
};

}  // namespace detail

/**
 * Set of parameters for Mersenne Twister pseudo-random number generators.
 *
 * @tparam UIntType unsigned integral type for generators
 * @tparam WordSize word size for pseudo-random numbers
 * @tparam p Mersenne prime exponent
 */
template <class UIntType, std::size_t WordSize, std::size_t p>
class dcmt_param {
  static_assert(std::is_integral<UIntType>::value &&
                    std::is_unsigned<UIntType>::value,
                "UIntType must be an unsigned integral type");

  static_assert(WordSize == 31 || WordSize == 32, "WordSize must be 31 or 32");

  static_assert((WordSize == 31 &&
                 std::numeric_limits<UIntType>::max() >= 0x7fffffffU) ||
                    (WordSize == 32 &&
                     std::numeric_limits<UIntType>::max() >= 0xffffffffU),
                "size of UIntType must be equal to or larger than WordSize");

  static_assert(
      p == 521 || p == 607 || p == 1279 || p == 2203 || p == 2281 ||
          p == 3217 || p == 4253 || p == 4423 || p == 9689 || p == 9941 ||
          p == 11213 || p == 19937 || p == 21701 || p == 23209 || p == 44497,
      "p must be in the range of 521 to 44497 and 2^p-1 must be a prime");

  using impl = detail::dcmt_engine_impl;

 public:
  /**
   * Integral type for generators.
   */
  using result_type = UIntType;

  /**
   * Word size that determines the range of generated numbers.
   */
  static constexpr std::size_t word_size = WordSize;

  /**
   * Mersenne prime exponent.
   */
  static constexpr std::size_t prime_exponent = p;

  /**
   * Default id to search for the characteristic polynomial.
   */
  static constexpr result_type default_id = 0;

  /**
   * Default random seed to search for the characteristic polynomial.
   */
  static constexpr result_type default_seed = 4172;

  /**
   * Searches for a set of parameters for pseudo-random number generators.
   *
   * @param id to generate the characteristic polynomial. Must be <= 65535
   * @param seed random seed to generate the characteristic polynomial
   */
  // NOTE: currently, only lower 32 bits are used for the seed.
  explicit dcmt_param(result_type id = default_id,
                      result_type seed = default_seed)
      : impl_(word_size, prime_exponent, id, seed) {}

 private:
  impl impl_;

  template <class U, std::size_t W, std::size_t P>
  friend class dcmt_engine;
};

/**
 * Mersenne Twister pseudo-random number generator engine.
 *
 * @tparam UIntType unsigned integral type for the engine
 * @tparam WordSize word size for the generated numbers
 * @tparam p Mersenne prime exponent
 */
template <class UIntType, std::size_t WordSize, std::size_t p>
class dcmt_engine {
  using impl = detail::dcmt_engine_impl;

 public:
  /**
   * Parameter type for the engine.
   */
  using param_type = dcmt_param<UIntType, WordSize, p>;

  /**
   * Integral type generated by the engine.
   */
  using result_type = UIntType;

  /**
   * Word size that determines the range of numbers generated by the engine.
   */
  static constexpr std::size_t word_size = WordSize;

  /**
   * Mersenne prime exponent.
   */
  static constexpr std::size_t prime_exponent = p;

  /**
   * Default random seed.
   */
  static constexpr result_type default_seed = 3241;

  /**
   * Constructs the engine.
   *
   * @param seed random seed
   */
  explicit dcmt_engine(result_type seed = default_seed)
      : dcmt_engine(param_type(), seed) {}

  /**
   * Constructs the engine.
   *
   * @param param set of parameters
   * @param seed random seed
   */
  explicit dcmt_engine(const param_type& param, result_type seed = default_seed)
      : impl_(param.impl_) {
    this->seed(seed);
  }

  /**
   * Constructs the engine.
   *
   * @param param set of parameters
   * @param seed random seed
   */
  explicit dcmt_engine(param_type&& param, result_type seed = default_seed)
      : impl_(std::move(param.impl_)) {
    this->seed(seed);
  }

  /**
   * Reinitializes the engine by a new seed value.
   *
   * @param value random seed
   */
  // NOTE: currently, only lower 32 bits are used for the seed.
  void seed(result_type value = default_seed) { impl_.sgenrand(value); }

  /**
   * Returns the next pseudo-random number.
   *
   * @return generated value
   */
  result_type operator()() { return impl_.genrand(); }

  /**
   * Advances the internal state.
   *
   * @param z number of advances
   */
  // NOTE: intentionally use "unsigned long long" for consistency with the
  // standard <random> library.
  // NOLINTNEXTLINE(google-runtime-int)
  void discard(unsigned long long z) { impl_.discard(z); }

  /**
   * Returns the miminum value potentially generated by the engine, which is 0.
   *
   * @return minimum value
   */
  static constexpr result_type min() { return 0U; }

  /**
   * Returns the maximum value potentially generated by the engine.
   *
   * @return maximum value
   */
  static constexpr result_type max() { return 0xffffffffU >> (32 - word_size); }

  /**
   * Compares two engines.
   *
   * @param a first engine
   * @param b second engine
   * @return `true` if the engines are equivalent including their internal
   * states, `false` otherwise
   */
  friend bool operator==(const dcmt_engine& a, const dcmt_engine& b) {
    return a.impl_ == b.impl_;
  }

  /**
   * Compares two engines.
   *
   * @param a first engine
   * @param b second engine
   * @return `true` if the engines are not equivalent including their internal
   * states, `false` otherwise
   */
  friend bool operator!=(const dcmt_engine& a, const dcmt_engine& b) {
    return !(a == b);
  }

  template <class CharT, class Traits>
  friend std::basic_ostream<CharT, Traits>& operator<<(
      std::basic_ostream<CharT, Traits>& os, const dcmt_engine& e) {
    return os << e.impl_;
  }

  template <class CharT, class Traits>
  friend std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& is, dcmt_engine& e) {
    return is >> e.impl_;
  }

 private:
  impl impl_;
};

// Constant members.

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr std::size_t dcmt_param<UIntType, WordSize, p>::word_size;

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr std::size_t dcmt_param<UIntType, WordSize, p>::prime_exponent;

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr UIntType dcmt_param<UIntType, WordSize, p>::default_id;

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr UIntType dcmt_param<UIntType, WordSize, p>::default_seed;

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr std::size_t dcmt_engine<UIntType, WordSize, p>::word_size;

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr std::size_t dcmt_engine<UIntType, WordSize, p>::prime_exponent;

template <class UIntType, std::size_t WordSize, std::size_t p>
constexpr UIntType dcmt_engine<UIntType, WordSize, p>::default_seed;

// Predefined engine.

/**
 * 32-bit Mersenne Twister pseudo-random number generator engine with a period
 * 2^521-1.
 */
using dcmt521 = dcmt_engine<std::uint_fast32_t, 32, 521>;

}  // namespace dcmt

#endif  // DCMT_CPP_DCMT_H
