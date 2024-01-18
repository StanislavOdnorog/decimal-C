#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_arithmetic_result code = S21_ARITHMETIC_OK;
    if (!result) {
        code = S21_ARITHMETIC_ERROR;
    } else if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
        code = S21_ARITHMETIC_ERROR;
        *result = s21_decimal_get_inf();
    } else {
        *result = s21_decimal_get_zero();
        s21_decimal res = s21_decimal_get_zero();

        int sign1 = s21_decimal_get_sign(value_1);
        int sign2 = s21_decimal_get_sign(value_2);

        if (sign1 == S21_POSITIVE && sign2 == S21_POSITIVE) {
            code = s21_add_handle(value_1, value_2, &res);
        } else if (sign1 == S21_POSITIVE && sign2 == S21_NEGATIVE) {
            code = s21_sub(value_1, s21_abs(value_2), &res);
        } else if (sign1 == S21_NEGATIVE && sign2 == S21_POSITIVE) {
            code = s21_sub(s21_abs(value_1), value_2, &res);
            s21_negate(res, &res);
        } else if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
            code = s21_add_handle(s21_abs(value_1), s21_abs(value_2), &res);
            s21_negate(res, &res);
        }

        if (s21_decimal_get_sign(res) == S21_NEGATIVE && code == S21_ARITHMETIC_BIG) {
            code = S21_ARITHMETIC_SMALL;
        }

        *result = res;
    }

    return code;
}

int s21_add_handle(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_arithmetic_result code = S21_ARITHMETIC_OK;
    s21_int256 value_1l;
    s21_int256 value_2l;
    int power1 = s21_decimal_get_power(value_1);
    int power2 = s21_decimal_get_power(value_2);
    int max_power = s21_max(power1, power2);

    s21_decimal_leveling(value_1, value_2, &value_1l, &value_2l);
    s21_int256 res = s21_int256_binary_addition(value_1l, value_2l);
    int shift = s21_int256_get_shift_to_decimal(res);
    int res_power = max_power - shift;

    if (res_power < 0) {
        code = S21_ARITHMETIC_BIG;
        *result = s21_decimal_get_inf();
    } else {
        while (shift > 28) {
            res =
                s21_int256_binary_division(res, s21_create_int256_from_decimal(s21_decimal_get_ten()), NULL);
            --shift;
        }

        s21_int256 remainder = s21_create_int256_from_decimal(s21_decimal_get_zero());
        s21_int256 powerten = s21_create_int256_from_decimal(s21_int128_get_ten_pow(shift));

        res = s21_int256_binary_division(res, powerten, &remainder);
        s21_decimal_set_power(&remainder.decimals[0], shift);
        res.decimals[0] = s21_round_banking(res.decimals[0], remainder.decimals[0]);
        s21_decimal_set_power(&res.decimals[0], res_power);

        if (!s21_int128_binary_equal_zero(res.decimals[1]) || !s21_is_correct_decimal(res.decimals[0])) {
            code = S21_ARITHMETIC_BIG;
            *result = s21_decimal_get_inf();
        } else {
            *result = res.decimals[0];
        }
    }

    return code;
}
