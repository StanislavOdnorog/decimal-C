#include "../s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  s21_conversion_result code = S21_CONVERSION_OK;
  if (!dst) {
    code = S21_CONVERSION_ERROR;
  } else if (src == 0.0) {
    code = S21_CONVERSION_OK;
    *dst = s21_decimal_get_zero();
    if (signbit(src) != 0) {
      s21_decimal_set_sign(dst, S21_NEGATIVE);
    }
  } else if (isinf(src) || isnan(src)) {
    code = S21_CONVERSION_ERROR;
    *dst = s21_decimal_get_inf();
    if (signbit(src) != 0) {
      s21_decimal_set_sign(dst, S21_NEGATIVE);
    }
  } else if (fabsf(src) > MAX_FLOAT_TO_CONVERT) {
    code = S21_CONVERSION_ERROR;
    *dst = s21_decimal_get_inf();
    if (signbit(src) != 0) {
      s21_decimal_set_sign(dst, S21_NEGATIVE);
    }
  } else if (fabsf(src) < MIN_FLOAT_TO_CONVERT) {
    code = S21_CONVERSION_ERROR;
    *dst = s21_decimal_get_zero();
  } else {
    *dst = s21_decimal_get_zero();
    s21_decimal result;
    char flt[64];

    sprintf(flt, "%.6E", fabsf(src));
    int exp = s21_get_float_exp_from_string(flt);
    if (exp <= -23) {
      int float_precision = exp + 28;
      sprintf(flt, "%.*E", float_precision, fabsf(src));
    }

    result = s21_float_string_to_decimal(flt);

    if (signbit(src) != 0) {
      s21_decimal_set_sign(&result, S21_NEGATIVE);
    }

    *dst = result;
  }

  return code;
}