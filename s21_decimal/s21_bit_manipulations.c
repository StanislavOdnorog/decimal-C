#include "../s21_decimal.h"

// BIT MANIPULATION //
void set_bit(s21_decimal *decimal, unsigned position, bool value) {
  // Find the bit in "position" in "decimal" and set it to "value"
  unsigned *ptr = &decimal->data + position / PART_BITS;
  if (value)
    *ptr |= (1 << (position % PART_BITS));
  else
    *ptr &= ~(1 << (position % PART_BITS));
}

bool get_bit(s21_decimal decimal, unsigned position) {
  // Find the bit in "position" in "decimal" and return it
  unsigned *ptr = &decimal.data + position / PART_BITS;
  return (*ptr >> (position % PART_BITS)) & 1;
}

void print_decimal_binary(s21_decimal decimal) {
  // Print the binary representation of "decimal"
  for (int i = MANTIS_END; i >= 0; i--) {
    // Print bite (0 or 1)
    printf("%u", get_bit(decimal, i));
    if (i % PART_BITS == 0 && i != 0)
      // Print separator
      printf(" | ");
  }
  printf("\n");
}

s21_decimal shift_decimal_left(s21_decimal decimal, int shift) {
  // Shift "decimal" "shift" times left and return it
  for (int i = MANTIS_END; i >= MANTIS_1_PART; i--) {
    if (i - shift < MANTIS_1_PART)
      set_bit(&decimal, i, 0);
    else
      set_bit(&decimal, i, get_bit(decimal, i - shift));
  }
  return decimal;
}

s21_decimal shift_decimal_right(s21_decimal decimal, int shift) {
  // Shift "decimal" "shift" times right and return it
  for (int i = MANTIS_1_PART; i <= MANTIS_END; i++) {
    if (i + shift > MANTIS_END)
      set_bit(&decimal, i, 0);
    else
      set_bit(&decimal, i, get_bit(decimal, i + shift));
  }
  return decimal;
}
