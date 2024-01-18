#include "s21_decimal_test.h"

START_TEST(s21_add_tests_1) {
  int a = (float)Rand_R(-10e5, 10e1);
  int b = (float)Rand_R(-10e5, 10e1);
  int c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_int_to_decimal(a, &_a);
  s21_from_int_to_decimal(b, &_b);
  s21_add(_a, _b, &res);
  s21_from_decimal_to_int(res, &c);

  // printf("%d + %d ||||| %d\n", a, b, c);

  ck_assert_int_eq(a + b, c);
}
END_TEST

START_TEST(s21_add_tests_2) {
  int a = (int)Rand_R(0, 10e5);
  int b = (int)Rand_R(0, 10e5);
  int c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_int_to_decimal(a, &_a);
  s21_from_int_to_decimal(b, &_b);
  s21_add(_a, _b, &res);
  s21_from_decimal_to_int(res, &c);

  // printf("%d + %d ||||| %d\n", a, b, c);

  ck_assert_int_eq(a + b, c);
}
END_TEST

START_TEST(s21_add_tests_3) {
  float a = Rand_R(-10e6, 10e6);
  float b = Rand_R(-10e6, 10e6);
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_add(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  ck_assert_float_eq_tol(a + b, c, 10);
}
END_TEST

START_TEST(s21_add_tests_4) {
  float a = -0.00000002;
  float b = -4;

  s21_decimal _a;
  s21_decimal _b;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_is_greater(_a, _b);
}
END_TEST

START_TEST(s21_add_tests_5) {
  float a = -4;
  float b = -0.00000002;

  s21_decimal _a;
  s21_decimal _b;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_is_greater(_a, _b);
}
END_TEST

Suite *suite_s21_add_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_add");
  tc = tcase_create("case_add");

  tcase_add_loop_test(tc, s21_add_tests_1, 0, 100);
  tcase_add_loop_test(tc, s21_add_tests_2, 0, 100);
  tcase_add_loop_test(tc, s21_add_tests_3, 0, 100);
  tcase_add_test(tc, s21_add_tests_4);
  tcase_add_test(tc, s21_add_tests_5);

  suite_add_tcase(s, tc);
  return s;
}
