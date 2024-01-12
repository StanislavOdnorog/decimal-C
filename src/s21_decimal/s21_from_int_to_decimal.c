#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_decimal zero = ZERO();
  bool sign = src >= 0;
  *dst = zero;
  dst->bits[3] = SIGN_HEX_POS;
  set_sign(dst, sign);
  if (!sign) src = -src;
  dst->bits[0] = src;
  return 1;
}
