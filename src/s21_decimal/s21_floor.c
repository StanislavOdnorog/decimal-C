#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_other_result code = S21_OTHER_OK;

  if (!result) {
    code = S21_OTHER_ERROR;
  } else if (!s21_is_correct_decimal(value)) {
    code = S21_OTHER_ERROR;
    *result = s21_decimal_get_inf();
  } else {
    *result = s21_decimal_get_zero();
    int sign = s21_decimal_get_sign(value);
    s21_decimal fractional;
    s21_decimal value_unsigned_truncated;
    s21_decimal value_unsigned = s21_abs(value);

    s21_truncate(value_unsigned, &value_unsigned_truncated);

    s21_sub(value_unsigned, value_unsigned_truncated, &fractional);

    if (sign == S21_NEGATIVE &&
        s21_is_greater(fractional, s21_decimal_get_zero())) {
      s21_add(value_unsigned_truncated, s21_decimal_get_one(),
              &value_unsigned_truncated);
    }

    *result = value_unsigned_truncated;
    s21_decimal_set_sign(result, sign);
  }

  return code;
}
