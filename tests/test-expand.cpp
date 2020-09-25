// Instantiate template classes; expand them as much as possible.

#include <dcmt/dcmt.h>

template class dcmt::dcmt_engine<std::uint_fast32_t, 32, 521>;

void foo(std::istream& is, std::ostream& os) {
  dcmt::dcmt521 a;
  dcmt::dcmt521 b;
  os << (a != b);
  os << a;
  is >> b;
}

int main() { return 0; }
