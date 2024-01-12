#include "../s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  s21_decimal compare_pos = {0xFFFFF, 0, 0, SIGN_HEX_POS};
  s21_decimal ten = TEN();
  s21_decimal one = ONE();
  *dst = 0;

  int error_code = 0;
  if (s21_is_greater_module(src, compare_pos)) error_code = 1;

  unsigned new_sign = get_sign(src);
  set_sign(&src, 1);

  float temp = 0;
  normalize_decs(&src, &one);
  float mult = 1;

  for (unsigned i = 0; i <= get_exp(src); i++) {
    mult /= 10.0;
  }

  char result[MAX_DECIMAL_EXP + 1] = {0};
  form_list_from_dec(result, src);

  for (int i = MAX_DECIMAL_EXP; i >= 0; i--) {
    temp += result[i] * mult;
    mult *= 10.;
  }

  *dst = temp;

  if (!new_sign) *dst = -*dst;

  return error_code;
}
