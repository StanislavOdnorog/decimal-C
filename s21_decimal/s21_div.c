#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal zero = ZERO();
  s21_decimal one = ONE();
  s21_decimal res = ONE();
  s21_decimal remainder = ONE();
  s21_decimal mult = ONE();
  s21_decimal five = FIVE();
  s21_decimal ten = TEN();
  s21_decimal max = MAX();
  s21_decimal inf = INF();

  int new_exp = get_exp(value_2) - get_exp(value_2);
  unsigned new_sign = get_sign(value_1) == get_sign(value_2);
  set_sign(&value_1, 1);
  set_sign(&value_2, 1);

  int error_code = 0;
  if (s21_is_greater(value_1, inf) || s21_is_greater(value_2, inf))
    error_code = 1;

  if (s21_is_zero(value_2)) {
    set_sign(&remainder, new_sign);
    *result = zero;
  } else if (s21_is_equal(value_1, value_2)) {
    set_sign(&remainder, new_sign);
    *result = one;
  } else {
    s21_decimal b_copy = value_2;
    while (s21_is_not_zero(value_1) && s21_is_less_or_equal(res, max)) {
      div_helper(&value_1, &value_2, &remainder, one, ten, b_copy, &new_exp,
                 &res, &mult);
    }
    if (s21_is_greater(res, max)) {
      div_helper(&value_1, &value_2, &remainder, one, ten, b_copy, &new_exp,
                 &res, &mult);
    }

    normalize_decs(&value_1, &b_copy);
    set_exp(&value_1, 0);
    set_exp(&res, 0);
    set_exp(&b_copy, 0);

    if (value_1.mantis[0] * 10 / b_copy.mantis[0] >= 5) {
      s21_add(res, one, &res);
    }

    res = sub_helper(res, mult);
    set_exp(&res, new_exp);
    set_sign(&res, new_sign);
    *result = res;
  }
  return error_code;
}
