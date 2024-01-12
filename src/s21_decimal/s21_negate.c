#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  s21_decimal inf = INF();

  int error_code = 0;
  if (s21_is_greater_module(value, inf)) error_code = 1;

  set_sign(&value, !get_sign(value));
  *result = value;
  return error_code;
}
