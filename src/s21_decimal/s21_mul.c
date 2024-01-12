#include "../s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal res = ZERO();
  s21_decimal zero = ZERO();
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();
  *result = res;

  int error_code = 0;
  if (s21_is_greater(value_1, inf) || s21_is_greater(value_2, inf))
    error_code = 1;
  if (s21_is_less(value_1, neg_inf) || s21_is_less(value_2, neg_inf))
    error_code = 2;

  int new_exp = get_exp(value_1) + get_exp(value_2);
  new_exp = new_exp > MAX_DECIMAL_EXP ? MAX_DECIMAL_EXP : new_exp;
  bool new_sign = get_sign(value_1) == get_sign(value_2);

  while (s21_is_not_zero(value_2)) {
    res = add_helper(res,
                     ((get_bit(value_2, MANTIS_1_PART) & 1)) ? value_1 : zero);
    value_1 = shift_decimal_left(value_1, 1);
    value_2 = shift_decimal_right(value_2, 1);
  }

  set_exp(&res, new_exp);
  set_sign(&res, new_sign);

  *result = res;
  return error_code;
}
