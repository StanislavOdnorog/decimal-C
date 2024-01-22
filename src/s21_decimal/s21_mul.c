#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_arithmetic_result code = S21_ARITHMETIC_OK;

  if (!result) {
    code = S21_ARITHMETIC_ERROR;
  } else if (!s21_is_correct_decimal(value_1) ||
             !s21_is_correct_decimal(value_2)) {
    code = S21_ARITHMETIC_ERROR;
    *result = s21_decimal_get_inf();
  } else {
    *result = s21_decimal_get_zero();
    s21_decimal res = s21_decimal_get_zero();

    int sign1 = s21_decimal_get_sign(value_1);
    int sign2 = s21_decimal_get_sign(value_2);

    if (sign1 == S21_POSITIVE && sign2 == S21_POSITIVE) {
      code = s21_mul_handle(value_1, value_2, &res);
    } else if (sign1 == S21_POSITIVE && sign2 == S21_NEGATIVE) {
      code = s21_mul_handle(value_1, s21_abs(value_2), &res);
      s21_negate(res, &res);
    } else if (sign1 == S21_NEGATIVE && sign2 == S21_POSITIVE) {
      code = s21_mul_handle(s21_abs(value_1), value_2, &res);
      s21_negate(res, &res);
    } else if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
      code = s21_mul_handle(s21_abs(value_1), s21_abs(value_2), &res);
    }

    if (code == S21_ARITHMETIC_BIG) {
      if (s21_decimal_get_sign(res) == S21_NEGATIVE) {
        code = S21_ARITHMETIC_SMALL;
      }
    }

    if (code == S21_ARITHMETIC_OK &&
        s21_is_not_equal(value_1, s21_decimal_get_zero()) &&
        s21_is_not_equal(value_2, s21_decimal_get_zero()) &&
        s21_is_equal(res, s21_decimal_get_zero())) {
      code = S21_ARITHMETIC_SMALL;
    }

    *result = res;
  }

  return code;
}

int s21_mul_handle(s21_decimal value_1, s21_decimal value_2,
                   s21_decimal *result) {
  s21_arithmetic_result code = S21_ARITHMETIC_OK;
  int power1 = s21_decimal_get_power(value_1);
  int power2 = s21_decimal_get_power(value_2);

  s21_decimal_null_service_bits(&value_1);
  s21_decimal_null_service_bits(&value_2);

  s21_big_decimal res = s21_binary_multiplication(value_1, value_2);

  int shift = s21_big_decimal_get_shift_to_decimal(res);
  int res_power = power1 + power2 - shift;

  if (res_power < 0) {
    code = S21_ARITHMETIC_BIG;
    *result = s21_decimal_get_inf();
  } else {
    while (shift > 28) {
      res = s21_big_decimal_binary_division(
          res, s21_create_big_decimal_from_decimal(s21_decimal_get_ten()),
          NULL);
      --shift;
    }

    if (res_power > 28) {
      s21_big_decimal tmp = res;
      int tmp_power = res_power;
      while (tmp_power > 28) {
        tmp = s21_big_decimal_binary_division(
            tmp, s21_create_big_decimal_from_decimal(s21_decimal_get_ten()),
            NULL);
        --tmp_power;
      }
      shift = res_power - tmp_power + shift;
      res_power = tmp_power;
    }

    s21_big_decimal remainder =
        s21_create_big_decimal_from_decimal(s21_decimal_get_zero());
    s21_big_decimal powerten =
        s21_create_big_decimal_from_decimal(s21_get_ten_pow(shift));

    res = s21_big_decimal_binary_division(res, powerten, &remainder);
    s21_decimal_set_power(&remainder.decimals[0], shift);
    res.decimals[0] = s21_round_banking(res.decimals[0], remainder.decimals[0]);
    s21_decimal_set_power(&res.decimals[0], res_power);

    if (!s21_binary_equal_zero(res.decimals[1]) ||
        !s21_is_correct_decimal(res.decimals[0])) {
      code = S21_ARITHMETIC_BIG;
      *result = s21_decimal_get_inf();
    } else {
      *result = res.decimals[0];
    }
  }

  return code;
}
