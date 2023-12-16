#include "../s21_decimal.h"

// DATA GETTERS AND SETTERS //
void set_sign(s21_decimal *decimal, bool sign) {
  // Find the sign position in "decimal" and set it to "sign"
  set_bit(decimal, SIGN_EXP_BITE_POS, sign);
}

bool get_sign(s21_decimal decimal) {
  // Find the sign in "decimal" and return it
  return get_bit(decimal, SIGN_EXP_BITE_POS);
}

unsigned get_exp(s21_decimal decimal) {
  // Find the exp in "decimal" and return it
  set_sign(&decimal, 0);
  return decimal.data;
}

void set_exp(s21_decimal *decimal, unsigned exp) {
  // Find the exp in "decimal" and set it to "exp"
  if (exp < MAX_EXP) {
    bool sign = get_sign(*decimal);

    decimal->data = exp;

    set_sign(decimal, sign);
  }
}
