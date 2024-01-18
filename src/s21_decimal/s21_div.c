#include "../s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_arithmetic_result code = S21_ARITHMETIC_OK;

    if (!result) {
        code = S21_ARITHMETIC_ERROR;
    } else if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
        code = S21_ARITHMETIC_ERROR;
        *result = s21_decimal_get_inf();
    } else if (s21_is_equal(value_2, s21_decimal_get_zero())) {
        code = S21_ARITHMETIC_ZERO_DIV;
        *result = s21_decimal_get_inf();
    } else {
        *result = s21_decimal_get_zero();
        int sign1 = s21_decimal_get_sign(value_1);
        int sign2 = s21_decimal_get_sign(value_2);
        s21_int256 value_1l;
        s21_int256 value_2l;
        s21_decimal_leveling(value_1, value_2, &value_1l, &value_2l);

        s21_int256 remainder = s21_create_int256_from_decimal(s21_decimal_get_zero());
        s21_int256 res;

        res = s21_int256_binary_division(value_1l, value_2l, &remainder);

        if (res.decimals[0].bits[3] != 0 || !s21_int128_binary_equal_zero(res.decimals[1])) {
            if (sign1 != sign2) {
                code = S21_ARITHMETIC_SMALL;
            } else {
                code = S21_ARITHMETIC_BIG;
            }
            *result = s21_decimal_get_inf();
        } else {
            code = s21_div_handle(value_2l, res, remainder, result);
            if (sign1 != sign2) {
                s21_decimal_set_sign(result, S21_NEGATIVE);
            }
            if (s21_decimal_get_sign(*result) == S21_NEGATIVE && code == S21_ARITHMETIC_BIG) {
                code = S21_ARITHMETIC_SMALL;
            }
            if (code == S21_ARITHMETIC_OK && s21_is_not_equal(value_1, s21_decimal_get_zero()) &&
                s21_is_equal(*result, s21_decimal_get_zero())) {
                code = S21_ARITHMETIC_SMALL;
            }
        }
    }

    return code;
}

int s21_div_handle(s21_int256 value_2l, s21_int256 res, s21_int256 remainder, s21_decimal *result) {
    s21_arithmetic_result code = S21_ARITHMETIC_OK;

    int power1 = s21_div_calc_fractional(&res, value_2l, &remainder);

    s21_int256 tmp_res = s21_create_int256_from_decimal(s21_decimal_get_zero());
    int power2 = s21_div_calc_fractional(&tmp_res, value_2l, &remainder);

    s21_decimal_set_power(&tmp_res.decimals[0], power2);

    if (s21_is_equal(tmp_res.decimals[0], s21_decimal_get_zerofive())) {
        if (!s21_int128_binary_equal_zero(remainder.decimals[0]) ||
            !s21_int128_binary_equal_zero(remainder.decimals[1])) {
            s21_add(tmp_res.decimals[0], s21_decimal_get_min(), &tmp_res.decimals[0]);
        }
    }
    res.decimals[0] = s21_round_banking(res.decimals[0], tmp_res.decimals[0]);
    s21_decimal_set_power(&res.decimals[0], power1);
    if (!s21_int128_binary_equal_zero(res.decimals[1]) || !s21_is_correct_decimal(res.decimals[0])) {
        code = S21_ARITHMETIC_BIG;
        *result = s21_decimal_get_inf();
    } else {
        *result = res.decimals[0];
    }

    return code;
}

int s21_div_calc_fractional(s21_int256 *res, s21_int256 value_2l, s21_int256 *remainder) {
    int power = 0;
    s21_int256 number = *res;

    s21_int256 tmp;
    s21_int256 tmp_remainder = *remainder;

    while ((!s21_int128_binary_equal_zero((*remainder).decimals[0]) ||
            !s21_int128_binary_equal_zero((*remainder).decimals[1])) &&
           power < 28) {
        s21_int256 number_stored = number;
        s21_int256 remainder_stored = tmp_remainder;

        number = s21_int256_binary_multiplication(number, s21_decimal_get_ten());
        tmp_remainder = s21_int256_binary_multiplication(tmp_remainder, s21_decimal_get_ten());
        tmp = s21_int256_binary_division(tmp_remainder, value_2l, &tmp_remainder);
        number = s21_int256_binary_addition(number, tmp);

        if (!s21_is_correct_decimal(number.decimals[0])) {
            number = number_stored;
            tmp_remainder = remainder_stored;
            break;
        }

        ++power;
    }

    *res = number;
    *remainder = tmp_remainder;

    return power;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    s21_arithmetic_result code = S21_ARITHMETIC_OK;

    if (!result) {
        code = S21_ARITHMETIC_ERROR;
    } else if (!s21_is_correct_decimal(value_1) || !s21_is_correct_decimal(value_2)) {
        code = S21_ARITHMETIC_ERROR;
        *result = s21_decimal_get_inf();
    } else if (s21_is_equal(value_2, s21_decimal_get_zero())) {
        code = S21_ARITHMETIC_ZERO_DIV;
        *result = s21_decimal_get_inf();
    } else if (s21_is_less(s21_abs(value_1), s21_abs(value_2))) {
        code = S21_ARITHMETIC_OK;
        *result = value_1;
    } else {
        *result = s21_decimal_get_zero();

        int sign1 = s21_decimal_get_sign(value_1);
        int power1 = s21_decimal_get_power(value_1);
        int power2 = s21_decimal_get_power(value_2);
        int max_power = s21_max(power1, power2);
        s21_int256 value_1l;
        s21_int256 value_2l;
        s21_int256 remainder = s21_create_int256_from_decimal(s21_decimal_get_zero());

        s21_decimal_leveling(value_1, value_2, &value_1l, &value_2l);
        s21_int256_binary_division(value_1l, value_2l, &remainder);
        s21_decimal_set_power(&remainder.decimals[0], max_power);

        *result = remainder.decimals[0];
        s21_decimal_set_sign(result, sign1);
    }

    return code;
}

s21_decimal s21_round_banking(s21_decimal integral, s21_decimal fractional) {
    s21_decimal zerofive = s21_decimal_get_zerofive();
    s21_decimal result;

    if (s21_is_equal(fractional, zerofive)) {
        if (s21_decimal_even(integral)) {
            result = integral;
        } else {
            result = s21_int128_binary_addition(integral, s21_decimal_get_one());
        }
    } else if (s21_is_greater(fractional, zerofive)) {
        result = s21_int128_binary_addition(integral, s21_decimal_get_one());
    } else {
        result = integral;
    }

    return result;
}