#include "s21_decimal_test.h"

START_TEST(s21_dec_test_1) {
  float a = Rand_R(1, 1000000000);
  float b = Rand_R(-1000000000, -1);
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  ck_assert_float_eq_tol(a / b, c, 1e-1);
}
END_TEST

START_TEST(s21_dec_test_2) {
  float a = Rand_R(1, 10000000000000000);
  float b = Rand_R(-10000000000000, -1);
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  ck_assert_float_eq_tol(a / b, c, 1e-1);
}
END_TEST

START_TEST(s21_dec_test_3) {
  float a = Rand_R(1, 10000000000000000);
  float b = Rand_R(-10000000000000, -1);
  float res = 0;

  s21_decimal _a;
  s21_decimal _b;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  res = s21_is_less_or_equal(_a, _b);

  ck_assert_int_eq(a <= b, res);
}
END_TEST



Suite *suite_s21_decimal_test(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_dec");
  tc = tcase_create("case_dec");

  tcase_add_loop_test(tc, s21_dec_test_1, 0, 300);
  tcase_add_loop_test(tc, s21_dec_test_2, 0, 5);
  tcase_add_loop_test(tc, s21_dec_test_3, 0, 30);

  suite_add_tcase(s, tc);
  return s;
}
