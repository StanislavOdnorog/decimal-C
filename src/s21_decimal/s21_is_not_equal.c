#include "../s21_decimal.h"

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return (a.bits[0] != b.bits[0]) || (a.bits[1] != b.bits[1]) ||
         (a.bits[2] != b.bits[2]) || (a.bits[3] != b.bits[3]);
}
