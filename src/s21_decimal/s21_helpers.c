#include "../s21_decimal.h"

void s21_decl_to_null(s21_decimal *decl) {
  decl->bits[0] = 0;
  decl->bits[1] = 0;
  decl->bits[2] = 0;
  decl->bits[3] = 0;
}

void s21_clear_decimal(s21_decimal *decimal) {
  decimal->bits[0] = 0;
  decimal->bits[1] = 0;
  decimal->bits[2] = 0;
  decimal->bits[3] = 0;
}

void s21_decimal_null_service_bits(s21_decimal *value) { value->bits[3] = 0; }

s21_decimal s21_decimal_get_zero(void) {
  s21_decimal result;
  s21_clear_decimal(&result);

  return result;
}

s21_decimal s21_decimal_get_one(void) {
  s21_decimal result;
  s21_clear_decimal(&result);
  result.bits[0] = 1;

  return result;
}

s21_decimal s21_decimal_get_ten(void) {
  s21_decimal result;
  s21_clear_decimal(&result);
  result.bits[0] = 10;

  return result;
}

s21_decimal s21_get_ten_pow(int pow) { return all_ten_pows[pow]; }

s21_decimal s21_decimal_get_zerofive(void) {
  s21_decimal result = {{0x5, 0x0, 0x0, 0x10000}};

  return result;
}

s21_decimal s21_decimal_get_min(void) {
  s21_decimal result = {{0x1, 0x0, 0x0, 0x1C0000}};

  return result;
}

s21_decimal s21_decimal_get_max(void) {
  s21_decimal result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  return result;
}

s21_decimal s21_decimal_get_int_max(void) {
  s21_decimal result = {{0x7FFFFFFF, 0x0, 0x0, 0x0}};

  return result;
}

s21_decimal s21_decimal_get_int_min(void) {
  s21_decimal result = {{0x80000000, 0x0, 0x0, 0x80000000}};

  return result;
}

s21_big_decimal s21_create_big_decimal_from_decimal(s21_decimal value_1) {
  s21_big_decimal result;
  result.decimals[0] = value_1;
  result.decimals[1] = s21_decimal_get_zero();
  return result;
}

s21_decimal s21_decimal_get_inf(void) {
  s21_decimal result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF}};

  return result;
}

int s21_is_correct_decimal(s21_decimal decimal) {
  int code = 1;

  if (s21_decimal_get_empty1(decimal) != 0 ||
      s21_decimal_get_empty2(decimal) != 0) {
    code = 0;
  } else {
    int power = s21_decimal_get_power(decimal);
    if (power < 0 || power > 28) {
      code = 0;
    }
  }

  return code;
}

int s21_decimal_get_sign(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.sign;
}

int s21_decimal_get_power(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.power;
}

int s21_decimal_get_empty1(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.empty1;
}

int s21_decimal_get_empty2(s21_decimal decimal) {
  decimal_bit3 bits3;
  bits3.i = decimal.bits[3];

  return bits3.parts.empty2;
}

void s21_decimal_set_sign(s21_decimal *decimal, int sign) {
  decimal_bit3 bits3;
  bits3.i = decimal->bits[3];
  if (sign == S21_POSITIVE) {
    bits3.parts.sign = S21_POSITIVE;
  } else {
    bits3.parts.sign = S21_NEGATIVE;
  }

  decimal->bits[3] = bits3.i;
}

void s21_decimal_set_power(s21_decimal *decimal, int power) {
  decimal_bit3 bits3;
  bits3.i = decimal->bits[3];
  bits3.parts.power = power;

  decimal->bits[3] = bits3.i;
}

int s21_decimal_even(s21_decimal value) { return (value.bits[0] & 1) != 1; }

s21_decimal s21_remove_trailing_zeros(s21_decimal value) {
  s21_decimal result = value;
  int power = s21_decimal_get_power(value);
  int sign = s21_decimal_get_sign(value);

  if (power > 0 && s21_is_correct_decimal(value)) {
    s21_decimal remainder = s21_decimal_get_zero();
    s21_decimal tmp = value;
    s21_decimal_null_service_bits(&tmp);

    while (power > 0) {
      tmp = s21_binary_division(tmp, s21_get_ten_pow(1), &remainder);
      if (s21_binary_equal_zero(remainder)) {
        --power;
        result = tmp;
      } else {
        break;
      }
    }
    s21_decimal_set_power(&result, power);
    s21_decimal_set_sign(&result, sign);
  }

  return result;
}

s21_decimal s21_decimal_get_from_char(char c) {
  s21_decimal result;

  switch (c) {
    case '0':
      result = s21_decimal_get_zero();
      break;
    case '1':
      result = s21_decimal_get_one();
      break;
    case '2':
      s21_from_int_to_decimal(2, &result);
      break;
    case '3':
      s21_from_int_to_decimal(3, &result);
      break;
    case '4':
      s21_from_int_to_decimal(4, &result);
      break;
    case '5':
      s21_from_int_to_decimal(5, &result);
      break;
    case '6':
      s21_from_int_to_decimal(6, &result);
      break;
    case '7':
      s21_from_int_to_decimal(7, &result);
      break;
    case '8':
      s21_from_int_to_decimal(8, &result);
      break;
    case '9':
      s21_from_int_to_decimal(9, &result);
      break;
  }

  return result;
}

int s21_get_float_exp_from_string(char *str) {
  int result = 0;
  char *ptr = str;
  while (*ptr) {
    if (*ptr == 'E') {
      ++ptr;
      result = strtol(ptr, NULL, 10);
      break;
    }
    ++ptr;
  }

  return result;
}

s21_decimal s21_float_string_to_decimal(char *str) {
  int digits_counter = 6;
  s21_decimal result = s21_decimal_get_zero();
  char *ptr = str;

  int exp = s21_get_float_exp_from_string(str);

  while (*ptr) {
    if (*ptr == '.') {
      ++ptr;
      continue;
    } else if (*ptr >= '0' && *ptr <= '9') {
      s21_decimal tmp = s21_decimal_get_zero();
      s21_mul(s21_decimal_get_from_char(*ptr), s21_get_ten_pow(digits_counter),
              &tmp);
      s21_add(result, tmp, &result);
      --digits_counter;
      ++ptr;
    } else {
      break;
    }
  }
  exp = exp - 6;

  if (exp > 0) {
    s21_mul(result, s21_get_ten_pow(exp), &result);
  } else if (exp < 0) {
    if (exp < -28) {
      s21_div(result, s21_get_ten_pow(28), &result);
      exp += 28;
    }
    s21_div(result, s21_get_ten_pow(-exp), &result);
  }

  return result;
}