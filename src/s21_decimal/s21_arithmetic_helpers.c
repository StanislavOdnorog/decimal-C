#include "../s21_decimal.h"

int s21_int256_get_shift_to_decimal(s21_int256 value) {
    int cnt = 0;

    if (!(s21_int128_binary_equal_zero(value.decimals[0]) &&
          s21_int128_binary_equal_zero(value.decimals[1]))) {
        s21_int256 max = s21_create_int256_from_decimal(s21_decimal_get_max());
        s21_int256 quotient = s21_int256_binary_division(value, max, NULL);

        while (1) {
            int compare = s21_int128_binary_compare(quotient.decimals[0], s21_int128_get_ten_pow(cnt));
            if (compare == -1 || compare == 0) {
                break;
            }
            ++cnt;
        }

        s21_int256 tmp = s21_int256_binary_division(
            value, s21_create_int256_from_decimal(s21_int128_get_ten_pow(cnt)), NULL);
        if (!s21_int128_binary_equal_zero(tmp.decimals[1]) || tmp.decimals[0].bits[3] != 0) {
            ++cnt;
        }
    }

    return cnt;
}

int s21_max(int value_1, int value_2) {
    int result = value_2;
    if (value_1 > value_2) {
        result = value_1;
    }

    return result;
}

void s21_decimal_leveling(s21_decimal value_1, s21_decimal value_2, s21_int256 *value_1l,
                          s21_int256 *value_2l) {
    int power1 = s21_decimal_get_power(value_1);
    int power2 = s21_decimal_get_power(value_2);
    s21_decimal tmp1 = value_1;
    s21_decimal tmp2 = value_2;

    s21_decimal_null_service_bits(&tmp1);
    s21_decimal_null_service_bits(&tmp2);

    if (power1 > power2) {
        *value_1l = s21_create_int256_from_decimal(tmp1);
        *value_2l = s21_int128_binary_multiplication(tmp2, s21_int128_get_ten_pow(power1 - power2));
    } else if (power1 < power2) {
        *value_1l = s21_int128_binary_multiplication(tmp1, s21_int128_get_ten_pow(power2 - power1));
        *value_2l = s21_create_int256_from_decimal(tmp2);
    } else {
        *value_1l = s21_create_int256_from_decimal(tmp1);
        *value_2l = s21_create_int256_from_decimal(tmp2);
    }
}
