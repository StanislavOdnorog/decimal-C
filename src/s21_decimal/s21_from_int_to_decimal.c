#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_conversion_result code = S21_CONVERSION_OK;

  if (!dst) {
    code = S21_CONVERSION_ERROR;
  } else {
    *dst = s21_decimal_get_zero();
    int sign;

    if (src < 0) {
      sign = S21_NEGATIVE;
      if (src != INT_MIN) {
        src = -src;
      }
    } else {
      sign = S21_POSITIVE;
    }

    dst->bits[0] = src;
    s21_decimal_set_sign(dst, sign);
  }

  return code;
}
