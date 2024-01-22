#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  s21_conversion_result code = S21_CONVERSION_OK;
  if (!dst) {
    code = S21_CONVERSION_ERROR;
  } else if (!s21_is_correct_decimal(src)) {
    code = S21_CONVERSION_ERROR;
    *dst = 0.0;
  } else if (s21_is_equal(src, s21_decimal_get_zero())) {
    int sign = s21_decimal_get_sign(src);
    if (sign == S21_NEGATIVE) {
      *dst = -0.0;
    } else {
      *dst = 0.0;
    }
    code = S21_CONVERSION_OK;
  } else {
    *dst = 0.0;
    double tmp = 0.0;
    int sign = s21_decimal_get_sign(src);
    int power = s21_decimal_get_power(src);

    for (int i = 0; i < MAX_BLOCK_NUMBER; i++) {
      if (s21_decimal_is_set_bit(src, i) != 0) {
        tmp += pow(2.0, i);
      }
    }

    double powerten = pow(10, power);
    tmp /= powerten;

    if (sign == S21_NEGATIVE) {
      tmp *= -1.0;
    }

    *dst = (float)tmp;
  }

  return code;
}
