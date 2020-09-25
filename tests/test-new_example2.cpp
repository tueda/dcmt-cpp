// This program should reproduce the output of "new_example2.c".

#include <dcmt/dcmt.h>

#include <array>
#include <iomanip>
#include <iostream>

using dcmt::dcmt521;
using std::array;
using std::cout;
using std::endl;
using std::hex;
using std::setw;

int main() {
  dcmt521 r1{dcmt521::param_type(0), 1234};
  dcmt521 r2{dcmt521::param_type(1), 4567};
  dcmt521 r3{dcmt521::param_type(999), 8901};

  const int n = 10;

  array<dcmt521::result_type, n> x1;
  array<dcmt521::result_type, n> x2;
  array<dcmt521::result_type, n> x3;

  for (int i = 0; i < n; i++) {
    x1[i] = r1();
    x2[i] = r2();
    x3[i] = r3();
  }

  for (int i = 0; i < 10; i++) {
    cout << hex << setw(8) << x1[i] << ' ';
    cout << hex << setw(8) << x2[i] << ' ';
    cout << hex << setw(8) << x3[i] << endl;
  }

  // check

  const array<dcmt521::result_type, n> seq1{
      0x527415d6U, 0x77876c15U, 0xf09e6deaU, 0xd582955cU, 0x3bee0f14U,
      0xbbf15d5eU, 0xdb6d7f03U, 0x19fef1a1U, 0xd95d4addU, 0x17335683U,
  };

  const array<dcmt521::result_type, n> seq2{
      0x00af2259U, 0xc7d8a3a7U, 0x7ec9c866U, 0x0f3c64baU, 0xabbcbd72U,
      0xad5b6275U, 0xc060150eU, 0x0e57a0a4U, 0x2d44a12fU, 0x62913251U,
  };

  const array<dcmt521::result_type, n> seq3{
      0x25810b80U, 0xc608933fU, 0xa7277662U, 0x2315fe87U, 0x80e94e47U,
      0x5406d957U, 0xc4181b50U, 0x9b45ac86U, 0xdd75553bU, 0x1cc69638U,
  };

  if (x1 != seq1 || x2 != seq2 || x3 != seq3) {
    return EXIT_FAILURE;
  }

  return 0;
}
