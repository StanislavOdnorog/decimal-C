#include "../s21_decimal.h"

s21_decimal s21_abs(s21_decimal value_1) {
    s21_decimal result = value_1;
    s21_decimal_set_sign(&result, S21_POSITIVE);
    return result;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
    s21_other_result code = S21_OTHER_OK;

    if (!result) {
        code = S21_OTHER_ERROR;
    } else if (!s21_is_correct_decimal(value)) {
        code = S21_OTHER_ERROR;
        *result = s21_decimal_get_inf();
    } else {
        *result = s21_decimal_get_zero();
        int power = s21_decimal_get_power(value);
        s21_decimal tmp = value;
        s21_decimal_null_service_bits(&tmp);

        tmp = s21_int128_binary_division(tmp, s21_int128_get_ten_pow(power), NULL);

        *result = tmp;
        if (s21_decimal_get_sign(value) == S21_NEGATIVE) {
            s21_decimal_set_sign(result, S21_NEGATIVE);
        }
    }

    return code;
}