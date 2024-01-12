#include "../s21_decimal.h"

// COMPARATORS //
int s21_is_zero(s21_decimal a) {
  return a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0;
}

int s21_is_not_zero(s21_decimal a) {
  return a.bits[0] != 0 || a.bits[1] != 0 || a.bits[2] != 0;
}

int s21_is_greater_module(s21_decimal a, s21_decimal b) {
  set_sign(&a, 1);
  set_sign(&b, 1);
  return s21_is_greater(a, b);
}
