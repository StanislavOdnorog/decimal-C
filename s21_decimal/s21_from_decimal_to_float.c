#include "../s21_decimal.h"
 
int s21_from_decimal_to_float(s21_decimal src, float *dst){
  s21_decimal compare_pos = {SIGN_HEX_POS, 0xFFFFFFFF, 0, 0};
  s21_decimal compare_neg = {SIGN_HEX_NEG, 0xFFFFFFFF, 0, 0};
  s21_decimal ten = TEN();
  s21_decimal one = ONE();

  if (s21_is_greater(src, compare_pos))
    return 1;
  if (s21_is_less(src, compare_neg))
    return 1;

  unsigned new_sign = get_sign(src);
  set_sign(&src, 1);

  unsigned temp = 0;
  normalize_decs(&src, &one);
  printf("FUCK\n");
  unsigned mult = 1;
  char result[MAX_DECIMAL_EXP+1] = {0};
  form_list_from_dec(result, src);
  for (int i = MAX_DECIMAL_EXP - 1; i > 0; i--){
    temp += result[i] * mult;
    mult *= 10;
  }

  float res = 0.;

  *dst = (double)temp;
  for (unsigned i = 0; i < get_exp(src); i++) {
    *dst /= (double)round(10.0);
  }

  if (!new_sign)
    *dst = -*dst;

  return 0;
}
