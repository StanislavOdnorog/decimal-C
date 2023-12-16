#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_decimal one = ONE();
  s21_decimal five = FIVE();
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();
  int error_code = 0;
  if (s21_is_greater(value, inf)) error_code = 1;
  if (s21_is_less(value, neg_inf)) error_code = 1;
  *result = value;
  set_exp(&five, 1);

  unsigned new_sign = get_sign(value);
  set_sign(&value, 1);

  s21_truncate(value, result);

  s21_decimal rest;
  s21_sub(value, *result, &rest);

  if (!new_sign && s21_is_not_zero(rest)) s21_add(*result, one, result);

  set_sign(result, new_sign);
  return error_code;
}
