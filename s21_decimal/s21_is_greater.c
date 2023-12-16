#include "../s21_decimal.h"

int s21_is_greater(s21_decimal a, s21_decimal b) {
  s21_decimal zero = ZERO();
  if (get_sign(a) > get_sign(b))
    return true;
  else if (get_sign(a) < get_sign(b))
    return false;

  if (get_sign(a)) {
    if (get_exp(a) < get_exp(b))
      return true;
    else if (get_exp(a) > get_exp(b))
      return false;

    for (int i = 2; i >= 0; i--) {
      if (a.mantis[i] > b.mantis[i])
        return true;
      else if (a.mantis[i] < b.mantis[i])
        return false;
    }
  } else {
    if (get_exp(a) < get_exp(b))
      return false;
    else if (get_exp(a) > get_exp(b))
      return true;

    for (int i = 2; i >= 0; i--) {
      if (a.mantis[i] > b.mantis[i])
        return false;
      else if (a.mantis[i] < b.mantis[i])
        return true;
    }
  }
  return false;
}
