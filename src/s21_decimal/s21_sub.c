#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();

  int error_code = 0;
  if (s21_is_greater(value_1, inf) || s21_is_greater(value_2, inf))
    error_code = 1;
  if (s21_is_less(value_1, neg_inf) || s21_is_less(value_2, neg_inf))
    error_code = 2;

  set_sign(&value_2, !get_sign(value_2));
  s21_add(value_1, value_2, result);
  return error_code;
}
