#include "../s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    s21_comparison_result code = S21_COMPARISON_FALSE;

    int sign1 = s21_decimal_get_sign(value_1);
    int sign2 = s21_decimal_get_sign(value_2);

    if (s21_is_equal(value_1, s21_decimal_get_zero()) && s21_is_equal(value_2, s21_decimal_get_zero())) {
        code = S21_COMPARISON_FALSE;
    } else if (sign1 == S21_NEGATIVE && sign2 == S21_POSITIVE) {
        code = S21_COMPARISON_TRUE;
    } else if (sign1 == S21_POSITIVE && sign2 == S21_NEGATIVE) {
        code = S21_COMPARISON_FALSE;
    } else if (sign1 == S21_NEGATIVE && sign2 == S21_NEGATIVE) {
        code = s21_is_less_handle(s21_abs(value_2), s21_abs(value_1));
    } else {
        code = s21_is_less_handle(value_1, value_2);
    }

    return code;
}

int s21_is_less_handle(s21_decimal value_1, s21_decimal value_2) {
    s21_comparison_result code = S21_COMPARISON_FALSE;
    s21_decimal tmp1 = value_1;
    s21_decimal tmp2 = value_2;
    s21_int256 tmp1l;
    s21_int256 tmp2l;

    s21_decimal_leveling(tmp1, tmp2, &tmp1l, &tmp2l);

    int compare = s21_int256_binary_compare(tmp1l, tmp2l);

    if (compare == -1) {
        code = S21_COMPARISON_TRUE;
    } else {
        code = S21_COMPARISON_FALSE;
    }

    return code;
}