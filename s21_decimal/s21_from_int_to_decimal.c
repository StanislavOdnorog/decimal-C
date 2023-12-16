#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_decimal zero = ZERO();
  bool sign = src > 0;
  *dst = zero;
  dst->data = 0;
  set_sign(dst, sign);
  if (!sign) src = -src;
  dst->mantis[0] = src;
  return 1;
}
