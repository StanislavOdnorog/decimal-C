#include "../s21_decimal.h"

// PRINT DECIMAL //
void print_list(char *list, char exp) {
  int i = 0;
  if (exp == 0) {
    printf("0");
  }
  while (list[i] == 0 && i < exp - 1) i++;
  for (; i < MAX_DECIMAL_EXP; i++) {
    if (i == exp) printf(".");
    printf("%d", list[i]);
  }
  printf("\n");
}

char *form_list_from_num(char *list, char num, char zeroes) {
  while (zeroes >= 0 && num > 0) {
    list[zeroes--] = num % 10;
    num /= 10;
  }
  return list;
}

char *add_list_to_list(char *a, char *b) {
  char temp = 0;
  for (int i = MAX_DECIMAL_EXP - 1; i >= 0; i--) {
    a[i] += b[i] + temp;
    temp = 0;
    if (a[i] > 9) {
      temp = a[i] / 10;
      a[i] %= 10;
    }
  }
  return a;
}

char *mul_list(char *a, char *b) {
  char result[MAX_DECIMAL_EXP] = {0};
  char res;
  for (int i = MAX_DECIMAL_EXP - 1; i >= 0; i--) {
    for (int j = MAX_DECIMAL_EXP - 1; j >= 0; j--) {
      char num[MAX_DECIMAL_EXP] = {0};
      res = a[i] * b[j];
      form_list_from_num(num, res, j + i - MAX_DECIMAL_EXP + 1);
      add_list_to_list(result, num);
    }
  }
  for (int i = 0; i < MAX_DECIMAL_EXP; i++) {
    a[i] = result[i];
  }
  return a;
}

char *form_list_from_dec(char *result, s21_decimal num) {
  char two[MAX_DECIMAL_EXP] = {0};
  two[MAX_DECIMAL_EXP - 1] = 2;

  if (get_bit(num, 0)) result[MAX_DECIMAL_EXP - 1] = 1;

  for (unsigned i = 1; i <= MANTIS_END; i++) {
    if (!get_bit(num, i)) continue;

    char temp[MAX_DECIMAL_EXP] = {0};
    temp[MAX_DECIMAL_EXP - 1] = 2;

    for (unsigned j = 1; j < i; j++) mul_list(temp, two);

    add_list_to_list(result, temp);
  }
  return result;
}

void print_dec(s21_decimal *num) {
  char result[MAX_DECIMAL_EXP + 1] = {0};

  form_list_from_dec(result, *num);

  if (!get_sign(*num) && !s21_is_zero(*num)) printf("-");
  print_list(result, MAX_DECIMAL_EXP - get_exp(*num));
}
