#include "s21_decimal_test.h"

START_TEST(s21_from_decimal_to_float_1) {
  s21_decimal var;
  float value = 0;
  s21_from_float_to_decimal(5.0, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(5.0, value, 10e-5);
}
END_TEST

START_TEST(s21_from_decimal_to_float_2) {
  float n = Rand_R(-3402823, 3402823);
  s21_decimal var;
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

START_TEST(s21_from_decimal_to_float_3) {
  float n = Rand_R(-1e-6 / 3, 1e-6 / 3);
  s21_decimal var;
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

START_TEST(s21_from_decimal_to_float_4) {
  float n = 1.2;
  s21_decimal *var = NULL;
  int result;

  result = s21_from_float_to_decimal(n, var);

  ck_assert_int_eq(result, S21_CONVERSION_ERROR);
}
END_TEST

START_TEST(s21_from_decimal_to_float_5) {
  float n = MIN_FLOAT_TO_CONVERT;
  s21_decimal var;
  int result;

  result = s21_from_float_to_decimal(n, &var);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_6) {
  float n = MAX_FLOAT_TO_CONVERT;
  s21_decimal var;
  int result;

  result = s21_from_float_to_decimal(n, &var);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_7) {
  float n = INFINITY;
  s21_decimal var;
  int result;

  result = s21_from_float_to_decimal(n, &var);

  ck_assert_int_eq(result, S21_CONVERSION_ERROR);
}
END_TEST

Suite *suite_s21_from_decimal_to_float_ste(void) {
  Suite *s;
  TCase *tc;
  s = suite_create("s21_float_conversion");
  tc = tcase_create("case_from_decimal_to_float");

  tcase_add_test(tc, s21_from_decimal_to_float_1);
  tcase_add_loop_test(tc, s21_from_decimal_to_float_2, 0, 100);
  tcase_add_loop_test(tc, s21_from_decimal_to_float_3, 0, 100);
  tcase_add_test(tc, s21_from_decimal_to_float_4);
  tcase_add_test(tc, s21_from_decimal_to_float_5);
  tcase_add_test(tc, s21_from_decimal_to_float_6);
  tcase_add_test(tc, s21_from_decimal_to_float_7);

  suite_add_tcase(s, tc);
  return s;
}
