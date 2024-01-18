#include "s21_decimal_test.h"

START_TEST(s21_mul_tests_1) {
  float a = Rand_R(-1e2, 1e2);
  float b = Rand_R(-1e2, 1e2);

  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_mul(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  ck_assert_float_eq_tol(a * b, c, 1e-3);
}
END_TEST

Suite *suite_s21_mul_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_mul");
  tc = tcase_create("case_mul");

  tcase_add_loop_test(tc, s21_mul_tests_1, 0, 300);

  suite_add_tcase(s, tc);
  return s;
}
