#include <dcmt/dcmt.h>

#include <cassert>
#include <cstdlib>  // std::free, std::malloc
#include <cstring>  // std::memcpy
#include <new>      // std::bad_alloc
#include <stdexcept>

namespace dcmt {

namespace detail {

using std::uint32_t;

// include guard in dc.h

/// @cond PRIVATE
#define DYNAMIC_CREATION
/// @endcond

// include other source files

#include <check32.c>
#include <eqdeg.c>
#include <genmtrand.c>
#include <mt19937.c>
#include <prescr.c>
#include <seive.c>

// implementation

void* alloc_memory(std::size_t size) {
  // NOLINTNEXTLINE(cppcoreguidelines-no-malloc,cppcoreguidelines-owning-memory)
  void* ptr = std::malloc(size);

  if (ptr == nullptr) {
    throw std::bad_alloc();
  }

  return ptr;
}

void free_memory(void* ptr) {
  // NOLINTNEXTLINE(cppcoreguidelines-no-malloc,cppcoreguidelines-owning-memory)
  std::free(ptr);
}

dcmt_engine_impl::dcmt_engine_impl(int w, int p, int id, std::uint32_t seed) {
  if (id > 65535) {
    throw std::invalid_argument("id must be <= 65535");
  }

  mt_struct* mt = get_mt_parameter_id_st(w, p, id, seed);

  if (mt == nullptr) {
    throw std::runtime_error("parameter not found");
  }

  ptr_.reset(mt);
}

dcmt_engine_impl::dcmt_engine_impl(const dcmt_engine_impl& impl) {
  *this = impl;
}

dcmt_engine_impl& dcmt_engine_impl::operator=(const dcmt_engine_impl& impl) {
  if (this == &impl) {
    return *this;
  }

  // See https://bugs.llvm.org/show_bug.cgi?id=28480
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  assert(impl.ptr_);

  mt_struct* mt = alloc_mt_struct(impl.ptr_->nn);

  if (mt == nullptr) {
    throw std::bad_alloc();
  }

  std::uint32_t* state = mt->state;
  *mt = *impl.ptr_;
  mt->state = state;
  std::memcpy(mt->state, impl.ptr_->state, mt->nn * sizeof(std::uint32_t));

  ptr_.reset(mt);

  return *this;
}

// This method doesn't change the pointer but changes its pointed data.
// NOLINTNEXTLINE(readability-make-member-function-const)
void dcmt_engine_impl::sgenrand(std::uint32_t seed) {
  // See https://bugs.llvm.org/show_bug.cgi?id=28480
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  assert(ptr_);
  sgenrand_mt(seed, ptr_.get());
}

// This method doesn't change the pointer but changes its pointed data.
// NOLINTNEXTLINE(readability-make-member-function-const)
std::uint32_t dcmt_engine_impl::genrand() {
  // See https://bugs.llvm.org/show_bug.cgi?id=28480
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  assert(ptr_);
  return genrand_mt(ptr_.get());
}

// NOLINTNEXTLINE(google-runtime-int)
void dcmt_engine_impl::discard(unsigned long long z) {
  // See https://bugs.llvm.org/show_bug.cgi?id=28480
  // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
  assert(ptr_);
  // NOLINTNEXTLINE(google-runtime-int)
  for (unsigned long long i = 0; i < z; i++) {
    genrand_mt(ptr_.get());
  }
}

}  // namespace detail

}  // namespace dcmt
