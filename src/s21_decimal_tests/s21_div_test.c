#include "s21_decimal_test.h"

START_TEST(s21_div_tests_1) {
  int a = (int)Rand_R(-100, 100);
  int b = (int)Rand_R(-100, 100);

  int c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_int_to_decimal(a, &_a);
  s21_from_int_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_int(res, &c);

  // puts("-------");
  // print_dec(&_a);
  // print_dec(&_b);
  // print_dec(&res);
  // puts("-------");

  ck_assert_int_eq(a / b, c);
}
END_TEST

START_TEST(s21_div_tests_2) {
  float a = Rand_R(1, 1000);
  float b = Rand_R(-1000, -1);
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  // puts("-------");
  // print_dec(&_a);
  // print_dec(&_b);
  // print_dec(&res);
  // puts("-------");

  ck_assert_float_eq_tol(a / b, c, 1e-2);
}
END_TEST

START_TEST(s21_div_tests_3) {
  float a = 1.0;
  float b = 1.0;
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  // puts("-------");
  // print_dec(&_a);
  // print_dec(&_b);
  // print_dec(&res);
  // puts("-------");

  ck_assert_float_eq_tol(a / b, c, 1e-2);
}
END_TEST

START_TEST(s21_div_tests_4) {
  float a = 1.0;
  float b = 0.0;
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  // puts("-------");
  // print_dec(&_a);
  // print_dec(&_b);
  // print_dec(&res);
  // puts("-------");

  ck_assert_float_eq_tol(0, c, 1e-2);
}
END_TEST

START_TEST(s21_div_tests_5) {
  float a = -0xFFFFFF;
  float b = 1.0;
  float c = 0;

  s21_decimal _a;
  s21_decimal _b;
  s21_decimal res;

  s21_from_float_to_decimal(a, &_a);
  s21_from_float_to_decimal(b, &_b);
  s21_div(_a, _b, &res);
  s21_from_decimal_to_float(res, &c);

  // puts("-------");
  // print_dec(&_a);
  // print_dec(&_b);
  // print_dec(&res);
  // puts("-------");

  ck_assert_float_eq_tol(a / b, c, 1e-2);
}
END_TEST

Suite *suite_s21_div_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_div");
  tc = tcase_create("case_div");

  tcase_add_loop_test(tc, s21_div_tests_1, 0, 20);
  tcase_add_loop_test(tc, s21_div_tests_2, 0, 20);
  tcase_add_test(tc, s21_div_tests_3);
  tcase_add_test(tc, s21_div_tests_4);
  tcase_add_test(tc, s21_div_tests_5);

  suite_add_tcase(s, tc);
  return s;
}
