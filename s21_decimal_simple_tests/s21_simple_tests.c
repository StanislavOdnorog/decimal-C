#include "../s21_decimal.h"

// TESTS //
void add_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2){
  s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);
  s21_decimal test2 = {sign2, num2, 0, 0};
  set_exp(&test2, exp2);

  print_dec(&test);
  // print_dec_bin(test);
  printf("+\n");
  print_dec(&test2);
  // print_dec_bin(test2);
  printf("=\n");
  s21_decimal result = {0};
  s21_add(test, test2, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void mul_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2){
  s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);
  s21_decimal test2 = {sign2, num2, 0, 0};
  set_exp(&test2, exp2);

  print_dec(&test);
  // print_dec_bin(test);
  printf("*\n");
  print_dec(&test2);
  // print_dec_bin(test2);
  printf("=\n");
  s21_decimal result;
  s21_mul(test, test2, &result);


  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void div_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2){
  s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);
  s21_decimal test2 = {sign2, num2, 0, 0};
  set_exp(&test2, exp2);

  print_dec(&test);
  // print_dec_bin(test);
  printf(":\n");
  print_dec(&test2);
  // print_dec_bin(test2);
  printf("=\n");
  s21_decimal result;
  s21_div(test, test2, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void sub_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2){
  s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);
  s21_decimal test2 = {sign2, num2, 0, 0};
  set_exp(&test2, exp2);

  print_dec(&test);
  // print_dec_bin(test);
  printf("-\n");

  print_dec(&test2);
  // print_dec_bin(test2);
  printf("=\n");
  s21_decimal result;
  s21_sub(test, test2, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void from_dec_to_int_test(unsigned sign1, unsigned num1, unsigned exp1){
  s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);

  print_dec(&test);
  // print_dec_bin(test);
  printf("DECtoINT=\n");
  int result = 0;
  s21_from_decimal_to_int(test, &result);

  printf("%d\n", result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void from_int_to_dec_test(int value){
  printf("%d\n", value);
  // print_dec_bin(test);
  printf("INTtoDEC=\n");
  s21_decimal result = ZERO();
  s21_from_int_to_decimal(value, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}


void from_float_to_dec_test(float value){
  printf("%f\n", value);
  // print_dec_bin(test);
  printf("FLOATtoDEC=\n");
  s21_decimal result = ZERO();
  s21_from_float_to_decimal(value, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void from_dec_to_float_test(unsigned sign1, unsigned num1, unsigned exp1){
  s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);

  print_dec(&test);
  // print_dec_bin(test);
  
  printf("DECtoFLOAT=\n");

  float result = 0;
  s21_from_decimal_to_float(test, &result);

  printf("%f\n", result);
  // print_dec_bin(result);
  printf("------------------\n");
}


void truncate_test(unsigned sign1, unsigned num1, unsigned exp1){
 s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);

  print_dec(&test);
  // print_dec_bin(test);
  printf("TRUNCATE=\n");
  s21_decimal result;
  s21_truncate(test, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}


void round_test(unsigned sign1, unsigned num1, unsigned exp1){
 s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);

  print_dec(&test);
  // print_dec_bin(test);
  printf("ROUND=\n");
  s21_decimal result;
  s21_round(test, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

void floor_test(unsigned sign1, unsigned num1, unsigned exp1){
 s21_decimal test = {sign1, num1, 0, 0};
  set_exp(&test, exp1);

  print_dec(&test);
  // print_dec_bin(test);
  printf("FLOOR=\n");
  s21_decimal result;
  s21_floor(test, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}
