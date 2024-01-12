#include "s21_decimal_test.h"

START_TEST(s21_dec_test) {
  int a = 1;
  int b = -1;
  int c = 0;
  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;
  char list[MAX_DECIMAL_EXP + 1] = {0};

  s21_from_int_to_decimal(a, &_a);
  s21_from_int_to_decimal(b, &_b);

  print_decimal_binary(_a);
  print_dec(&_a);
  print_dec(&_b);
  form_list_from_dec(list, _a);
  print_list(list, 0);
}
END_TEST

Suite *suite_s21_decimal_test(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_dec");
  tc = tcase_create("case_dec");

  tcase_add_test(tc, s21_dec_test);

  suite_add_tcase(s, tc);
  return s;
}
