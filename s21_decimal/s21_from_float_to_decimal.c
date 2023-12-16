#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  s21_decimal ten = TEN();
  s21_decimal temp = ZERO();
  *dst = temp;

  bool sign = src > 0;
  dst->data = 0;
  set_sign(dst, 1);
  if (!sign) src = -src;

  unsigned *ptr = (unsigned *)&src;
  dst->mantis[0] = trunc(src);

  unsigned exp = 0;
  while ((float)src != (float)trunc(src) && exp < 6) {
    src -= trunc(src);
    src *= 10.0;

    s21_from_int_to_decimal((int)src, &temp);

    s21_mul(*dst, ten, dst);
    s21_add(*dst, temp, dst);
    exp++;
  }

  set_exp(dst, exp);
  set_sign(dst, sign);

  return 0;
}
