#include "../s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  return (a.data == b.data) && (a.mantis[0] == b.mantis[0]) &&
         (a.mantis[1] == b.mantis[1]) && (a.mantis[2] == b.mantis[2]);
}
