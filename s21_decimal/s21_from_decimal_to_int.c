#include "../s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_decimal compare_pos = {SIGN_HEX_POS, 0xFFFFFFF, 0, 0};
  s21_decimal one = ONE();
  s21_decimal ten = TEN();
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();

  int error_code = 0;
  if (s21_is_greater_module(src, compare_pos)) error_code = 1;
  if (s21_is_greater(src, inf)) error_code = 1;
  if (s21_is_less(src, neg_inf)) error_code = 1;

  unsigned new_sign = get_sign(src);
  set_sign(&src, 1);
  unsigned temp = 0;

  normalize_decs(&src, &one);
  if (s21_is_greater_or_equal(src, one)) {
    unsigned mult = 1;
    char result[MAX_DECIMAL_EXP + 1] = {0};
    form_list_from_dec(result, src);
    for (int i = MAX_DECIMAL_EXP - 1 - get_exp(src); i >= 0; i--) {
      temp += result[i] * mult;
      mult *= 10;
    }
  }
  *dst = temp;

  if (!new_sign) *dst = -*dst;

  return error_code;
}
