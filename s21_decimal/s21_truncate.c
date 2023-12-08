#include "../s21_decimal.h"
 
int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();

  int error_code = 0;
  if(s21_is_greater(value, inf))
    error_code = 1;
  if(s21_is_less(value, neg_inf))
    error_code = 1;
  int res = 0;
  *result = value;
  s21_from_decimal_to_int(value, &res);
  s21_from_int_to_decimal(res, result);
  return error_code;
}
