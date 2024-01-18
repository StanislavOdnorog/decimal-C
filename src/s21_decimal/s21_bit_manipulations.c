#include "../s21_decimal.h"

int s21_is_set_bit(int number, int index) {
    return !!(number & (1U << index));
}

int s21_set_bit(int number, int index) {
    return number | (1U << index);
}

int s21_reset_bit(int number, int index) {
    return number & ~(1U << index);
}

int s21_decimal_is_set_bit(s21_decimal decimal, int index) {
    return s21_is_set_bit(decimal.bits[index / MAX_BLOCK_BITS], index % MAX_BLOCK_BITS);
}

s21_decimal s21_decimal_set_bit(s21_decimal decimal, int index) {
    decimal.bits[index / MAX_BLOCK_BITS] =
        s21_set_bit(decimal.bits[index / MAX_BLOCK_BITS], index % MAX_BLOCK_BITS);
    return decimal;
}

int s21_decimal_get_not_zero_bit(s21_decimal decimal) {
    int result = -1;
    for (int i = MAX_BITS - 1; i >= 0; i--) {
        if (s21_decimal_is_set_bit(decimal, i)) {
            result = i;
            break;
        }
    }

    return result;
}

s21_decimal s21_int128_binary_addition(s21_decimal decimal1, s21_decimal decimal2) {
    s21_decimal result = decimal1;
    s21_decimal tmp = decimal2;

    while (!s21_int128_binary_equal_zero(tmp)) {
        s21_decimal overflow_bits = s21_int128_binary_and(result, tmp);
        overflow_bits = s21_int128_binary_shift_left(overflow_bits, 1);
        result = s21_int128_binary_xor(result, tmp);
        tmp = overflow_bits;
    }

    return result;
}

s21_int256 s21_int256_binary_addition(s21_int256 decimal1, s21_int256 decimal2) {
    s21_int256 result = decimal1;
    s21_int256 tmp = decimal2;

    while (!s21_int128_binary_equal_zero(tmp.decimals[0]) || !s21_int128_binary_equal_zero(tmp.decimals[1])) {
        s21_int256 overflow_bits;
        overflow_bits.decimals[0] = s21_int128_binary_and(result.decimals[0], tmp.decimals[0]);
        overflow_bits.decimals[1] = s21_int128_binary_and(result.decimals[1], tmp.decimals[1]);

        overflow_bits = s21_int256_binary_shift_left(overflow_bits, 1);
        result.decimals[0] = s21_int128_binary_xor(result.decimals[0], tmp.decimals[0]);
        result.decimals[1] = s21_int128_binary_xor(result.decimals[1], tmp.decimals[1]);

        tmp = overflow_bits;
    }

    return result;
}

int s21_int128_binary_equal_zero(s21_decimal decimal) {
    return decimal.bits[0] == 0 && decimal.bits[1] == 0 && decimal.bits[2] == 0 && decimal.bits[3] == 0;
}

int s21_int128_binary_compare(s21_decimal d1, s21_decimal d2) {
    int result = 0;

    for (int i = MAX_BITS - 1; i >= 0; i--) {
        int b1 = s21_decimal_is_set_bit(d1, i);
        int b2 = s21_decimal_is_set_bit(d2, i);

        if (b1 == 0 && b2 != 0) {
            result = -1;
        }

        if (b1 != 0 && b2 == 0) {
            result = 1;
        }

        if (result != 0) {
            break;
        }
    }

    return result;
}

int s21_int256_binary_compare(s21_int256 d1, s21_int256 d2) {
    int compare = s21_int128_binary_compare(d1.decimals[1], d2.decimals[1]);

    if (compare == 0) {
        compare = s21_int128_binary_compare(d1.decimals[0], d2.decimals[0]);
    }

    return compare;
}

s21_decimal s21_int128_binary_division(s21_decimal decimal1, s21_decimal decimal2, s21_decimal *remainder) {

    s21_decimal result;

    s21_decimal partial_remainder = s21_decimal_get_zero();
    s21_decimal quotient = s21_decimal_get_zero();

    if (s21_int128_binary_equal_zero(decimal1)) {
        quotient = s21_decimal_get_zero();
        partial_remainder = s21_decimal_get_zero();
    } else if (s21_int128_binary_compare(decimal1, decimal2) == -1) {
        quotient = s21_decimal_get_zero();
        partial_remainder = decimal1;
    } else {

        int left1 = s21_decimal_get_not_zero_bit(decimal1);
        int left2 = s21_decimal_get_not_zero_bit(decimal2);
        int shift = left1 - left2;

        s21_decimal shifted_divisor = s21_int128_binary_shift_left(decimal2, shift);
        s21_decimal dividend = decimal1;

        int need_subtraction = 1;

        while (shift >= 0) {
            if (need_subtraction == 1) {
                partial_remainder = s21_int128_binary_subtraction(dividend, shifted_divisor);
            } else {
                partial_remainder = s21_int128_binary_addition(dividend, shifted_divisor);
            }

            quotient = s21_int128_binary_shift_left(quotient, 1);
            if (s21_decimal_is_set_bit(partial_remainder, MAX_BITS - 1) == 0) {
                quotient = s21_decimal_set_bit(quotient, 0);
            }

            dividend = s21_int128_binary_shift_left(partial_remainder, 1);

            if (s21_decimal_is_set_bit(partial_remainder, MAX_BITS - 1) == 0) {
                need_subtraction = 1;
            } else {
                need_subtraction = 0;
            }
            --shift;
        }
        if (s21_decimal_is_set_bit(partial_remainder, MAX_BITS - 1)) {
            partial_remainder = s21_int128_binary_addition(partial_remainder, shifted_divisor);
        }
        partial_remainder = s21_int128_binary_shift_right(partial_remainder, left1 - left2);
    }

    result = quotient;
    if (remainder != NULL) {
        *remainder = partial_remainder;
    }

    return result;
}

s21_int256 s21_int256_binary_division(s21_int256 decimal1, s21_int256 decimal2, s21_int256 *remainder) {
    s21_int256 result;

    s21_int256 partial_remainder = s21_create_int256_from_decimal(s21_decimal_get_zero());
    s21_int256 quotient = s21_create_int256_from_decimal(s21_decimal_get_zero());

    if (s21_int128_binary_equal_zero(decimal1.decimals[0]) &&
        s21_int128_binary_equal_zero(decimal1.decimals[1])) {
        quotient = s21_create_int256_from_decimal(s21_decimal_get_zero());
        partial_remainder = s21_create_int256_from_decimal(s21_decimal_get_zero());
    } else if (s21_int256_binary_compare(decimal1, decimal2) == -1) {
        quotient = s21_create_int256_from_decimal(s21_decimal_get_zero());
        partial_remainder = decimal1;
    } else {
        int left1 = s21_decimal_get_not_zero_bit(decimal1.decimals[1]);
        if (left1 == -1) {
            left1 = s21_decimal_get_not_zero_bit(decimal1.decimals[0]);
        } else {
            left1 = MAX_BITS + left1;
        }

        int left2 = s21_decimal_get_not_zero_bit(decimal2.decimals[1]);
        if (left2 == -1) {
            left2 = s21_decimal_get_not_zero_bit(decimal2.decimals[0]);
        } else {
            left2 = MAX_BITS + left2;
        }

        int shift = left1 - left2;

        s21_int256 shifted_divisor = s21_int256_binary_shift_left(decimal2, shift);
        s21_int256 dividend = decimal1;

        int need_subtraction = 1;

        while (shift >= 0) {
            if (need_subtraction == 1) {
                partial_remainder = s21_int256_binary_subtraction(dividend, shifted_divisor);
            } else {
                partial_remainder = s21_int256_binary_addition(dividend, shifted_divisor);
            }

            quotient = s21_int256_binary_shift_left(quotient, 1);
            if (s21_decimal_is_set_bit(partial_remainder.decimals[1], MAX_BITS - 1) == 0) {
                quotient.decimals[0] = s21_decimal_set_bit(quotient.decimals[0], 0);
            }

            dividend = s21_int256_binary_shift_left(partial_remainder, 1);

            if (s21_decimal_is_set_bit(partial_remainder.decimals[1], MAX_BITS - 1) == 0) {
                need_subtraction = 1;
            } else {
                need_subtraction = 0;
            }
            --shift;
        }
        if (s21_decimal_is_set_bit(partial_remainder.decimals[1], MAX_BITS - 1)) {
            partial_remainder = s21_int256_binary_addition(partial_remainder, shifted_divisor);
        }
        partial_remainder = s21_int256_binary_shift_right(partial_remainder, left1 - left2);
    }

    result = quotient;
    if (remainder != NULL) {
        *remainder = partial_remainder;
    }

    return result;
}

s21_decimal s21_int128_binary_and(s21_decimal decimal1, s21_decimal decimal2) {
    s21_decimal result = s21_decimal_get_zero();
    result.bits[0] = decimal1.bits[0] & decimal2.bits[0];
    result.bits[1] = decimal1.bits[1] & decimal2.bits[1];
    result.bits[2] = decimal1.bits[2] & decimal2.bits[2];
    result.bits[3] = decimal1.bits[3] & decimal2.bits[3];

    return result;
}

s21_decimal s21_int128_binary_xor(s21_decimal decimal1, s21_decimal decimal2) {
    s21_decimal result = s21_decimal_get_zero();
    result.bits[0] = decimal1.bits[0] ^ decimal2.bits[0];
    result.bits[1] = decimal1.bits[1] ^ decimal2.bits[1];
    result.bits[2] = decimal1.bits[2] ^ decimal2.bits[2];
    result.bits[3] = decimal1.bits[3] ^ decimal2.bits[3];

    return result;
}

s21_decimal s21_int128_binary_not(s21_decimal decimal) {
    s21_decimal result = s21_decimal_get_zero();
    result.bits[0] = ~decimal.bits[0];
    result.bits[1] = ~decimal.bits[1];
    result.bits[2] = ~decimal.bits[2];
    result.bits[3] = ~decimal.bits[3];

    return result;
}

s21_int256 s21_int128_binary_multiplication(s21_decimal decimal1, s21_decimal decimal2) {
    s21_int256 int256_result = s21_create_int256_from_decimal(s21_decimal_get_zero());
    s21_int256 int256_tmp1 = s21_create_int256_from_decimal(decimal1);

    int max_bit = s21_decimal_get_not_zero_bit(decimal2);

    for (int i = 0; i <= max_bit; i++) {
        if (s21_decimal_is_set_bit(decimal2, i) != 0) {
            int256_result = s21_int256_binary_addition(int256_result, int256_tmp1);
        }
        int256_tmp1 = s21_int256_binary_shift_left(int256_tmp1, 1);
    }

    return int256_result;
}

s21_int256 s21_int256_binary_multiplication(s21_int256 decimal1, s21_decimal decimal2) {
    s21_int256 int256_result = s21_create_int256_from_decimal(s21_decimal_get_zero());
    s21_int256 int256_tmp1 = decimal1;

    int max_bit = s21_decimal_get_not_zero_bit(decimal2);

    for (int i = 0; i <= max_bit; i++) {
        if (s21_decimal_is_set_bit(decimal2, i) != 0) {
            int256_result = s21_int256_binary_addition(int256_result, int256_tmp1);
        }

        int256_tmp1 = s21_int256_binary_shift_left(int256_tmp1, 1);
    }

    return int256_result;
}

s21_decimal s21_int128_binary_shift_left(s21_decimal decimal, int shift) {
    s21_decimal result = decimal;
    while (shift > 0) {
        result = s21_int128_binary_shift_left_one(result);
        --shift;
    }

    return result;
}

s21_decimal s21_int128_binary_shift_right(s21_decimal decimal, int shift) {
    s21_decimal result = decimal;
    while (shift > 0) {
        result = s21_int128_binary_shift_right_one(result);
        --shift;
    }

    return result;
}

s21_int256 s21_int256_binary_shift_left(s21_int256 decimal, int shift) {
    s21_int256 result = decimal;
    while (shift > 0) {
        int b0 = s21_decimal_is_set_bit(result.decimals[0], MAX_BITS - 1);
        result.decimals[0] = s21_int128_binary_shift_left_one(result.decimals[0]);
        result.decimals[1] = s21_int128_binary_shift_left_one(result.decimals[1]);
        if (b0) {
            result.decimals[1] = s21_decimal_set_bit(result.decimals[1], 0);
        }
        --shift;
    }

    return result;
}

s21_int256 s21_int256_binary_shift_right(s21_int256 decimal, int shift) {
    s21_int256 result = decimal;
    while (shift > 0) {
        int b1 = s21_decimal_is_set_bit(result.decimals[1], 0);
        result.decimals[0] = s21_int128_binary_shift_right_one(result.decimals[0]);
        result.decimals[1] = s21_int128_binary_shift_right_one(result.decimals[1]);
        if (b1) {
            result.decimals[0] = s21_decimal_set_bit(result.decimals[0], MAX_BITS - 1);
        }
        --shift;
    }

    return result;
}

s21_decimal s21_int128_binary_shift_left_one(s21_decimal decimal) {
    s21_decimal result = s21_decimal_get_zero();

    int b0 = s21_is_set_bit(decimal.bits[0], MAX_BLOCK_BITS - 1);
    unsigned int result0 = decimal.bits[0];
    result0 = result0 << 1;
    result.bits[0] = result0;

    int b1 = s21_is_set_bit(decimal.bits[1], MAX_BLOCK_BITS - 1);
    unsigned int result1 = decimal.bits[1];
    result1 = result1 << 1;
    result.bits[1] = result1;

    int b2 = s21_is_set_bit(decimal.bits[2], MAX_BLOCK_BITS - 1);
    unsigned int result2 = decimal.bits[2];
    result2 = result2 << 1;
    result.bits[2] = result2;

    unsigned int result3 = decimal.bits[3];
    result3 = result3 << 1;
    result.bits[3] = result3;

    if (b0) {
        result.bits[1] = s21_set_bit(result.bits[1], 0);
    }

    if (b1) {
        result.bits[2] = s21_set_bit(result.bits[2], 0);
    }

    if (b2) {
        result.bits[3] = s21_set_bit(result.bits[3], 0);
    }

    return result;
}

s21_decimal s21_int128_binary_shift_right_one(s21_decimal decimal) {
    s21_decimal result = s21_decimal_get_zero();

    int b3 = s21_is_set_bit(decimal.bits[3], 0);
    unsigned int result3 = decimal.bits[3];
    result3 = result3 >> 1;
    result.bits[3] = result3;

    int b2 = s21_is_set_bit(decimal.bits[2], 0);
    unsigned int result2 = decimal.bits[2];
    result2 = result2 >> 1;
    result.bits[2] = result2;

    int b1 = s21_is_set_bit(decimal.bits[1], 0);
    unsigned int result1 = decimal.bits[1];
    result1 = result1 >> 1;
    result.bits[1] = result1;

    unsigned int result0 = decimal.bits[0];
    result0 = result0 >> 1;
    result.bits[0] = result0;

    if (b3) {
        result.bits[2] = s21_set_bit(result.bits[2], MAX_BLOCK_BITS - 1);
    }

    if (b2) {
        result.bits[1] = s21_set_bit(result.bits[1], MAX_BLOCK_BITS - 1);
    }

    if (b1) {
        result.bits[0] = s21_set_bit(result.bits[0], MAX_BLOCK_BITS - 1);
    }

    return result;
}

s21_decimal s21_int128_binary_subtraction(s21_decimal decimal1, s21_decimal decimal2) {
    s21_decimal result;
    decimal2 = s21_int128_binary_not(decimal2);
    decimal2 = s21_int128_binary_addition(decimal2, s21_decimal_get_one());
    result = s21_int128_binary_addition(decimal1, decimal2);

    return result;
}

s21_int256 s21_int256_binary_subtraction(s21_int256 decimal1, s21_int256 decimal2) {
    s21_int256 result;
    decimal2.decimals[0] = s21_int128_binary_not(decimal2.decimals[0]);
    decimal2.decimals[1] = s21_int128_binary_not(decimal2.decimals[1]);

    s21_int256 one = s21_create_int256_from_decimal(s21_decimal_get_one());

    decimal2 = s21_int256_binary_addition(decimal2, one);
    result = s21_int256_binary_addition(decimal1, decimal2);

    return result;
}