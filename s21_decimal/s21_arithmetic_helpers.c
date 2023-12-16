#include "../s21_decimal.h"

// ARITHMETICS HELPERS //
void normalize_decs(s21_decimal *a, s21_decimal *b) {
  s21_decimal ten = TEN();
  set_exp(&ten, 1);

  while (get_exp(*a) != get_exp(*b)) {
    if (get_exp(*a) < get_exp(*b)) {
      s21_mul(*a, ten, a);
    } else {
      s21_mul(*b, ten, b);
    }
  }
}

s21_decimal add_helper(s21_decimal a, s21_decimal b) {
  bool carry = 0;  // Set the carry to zero

  for (int i = MANTIS_1_PART; i <= MANTIS_END;
       i++) {                                  // Itterate through all bites
    if (carry) {                               // If there is something in carry
      carry = get_bit(a, i) || get_bit(b, i);  // Carry is equal to or operation
      set_bit(&a, i,
              !(get_bit(a, i) ^ get_bit(b, i)));  // execute XNOR operation
    } else {  // If there is nothing in carry
      carry =
          get_bit(a, i) && get_bit(b, i);  // Carry is equal to and operation
      set_bit(&a, i, get_bit(a, i) ^ get_bit(b, i));  // execute XOR operation
    }
  }
  return a;
}

s21_decimal sub_helper(s21_decimal a, s21_decimal b) {
  s21_decimal result = {s21_is_greater_module(a, b) ? a.data : b.data, 0, 0, 0};
  unsigned borrow =
      0;  // Set thee result to the lowest exp, set the borrow var to zero

  for (int i = 0; i < 3; ++i) {
    unsigned a_part = a.mantis[i];  // Make a copy of a mantis
    unsigned b_part = b.mantis[i];  // Make a copy of b mantis

    if (a_part <
        b_part + borrow) {  // If a mantis is less than b mantis + borrow
      result.mantis[i] = (1LU << PART_BITS) + a_part - b_part - borrow;
      borrow = 1;  // Set the result to 2^32 in binary + a mantis - b mantis -
                   // borrow, set borrow 1
    } else {       // If a mantis is greater or equal than b mantis + borrow
      result.mantis[i] = a_part - b_part - borrow;
      borrow =
          0;  // Set the result to a mantis - b mantis - borrow, set borrow 0
    }
  }
  return result;
}

void div_helper(s21_decimal *a, s21_decimal *b, s21_decimal *remainder,
                s21_decimal one, s21_decimal ten, s21_decimal b_copy,
                int *new_exp, s21_decimal *res, s21_decimal *mult) {
  *remainder = one;
  *b = b_copy;
  normalize_decs(a, b);
  if (s21_is_greater(*b, *a)) {
    (*new_exp)++;
    s21_mul(*a, ten, a);
    s21_mul(*res, ten, res);
    s21_mul(*mult, ten, mult);
  } else {
    while (s21_is_less_or_equal(*b, *a)) {
      *b = shift_decimal_left(*b, 1);
      *remainder = shift_decimal_left(*remainder, 1);
    }

    do {
      if (s21_is_greater_or_equal(*a, *b)) {
        *a = sub_helper(*a, *b);
        *res = add_helper(*res, *remainder);
      }
      *remainder = shift_decimal_right(*remainder, 1);
      *b = shift_decimal_right(*b, 1);
    } while (s21_is_not_zero(*remainder));
  }
}
