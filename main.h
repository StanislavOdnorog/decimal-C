#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

// LIBRARIES //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// MACRO //
#define SIGN_EXP 7
#define SIGN_HEX_POS 0x80
#define SIGN_HEX_NEG 0
#define MAX_EXP_NUM 128
#define MAX_10_EXP 29

#define MAX_BITS 127
#define PART_BITS 32

#define M1 32
#define M2 64
#define M3 96

// STRUCTURE //
typedef struct{
  unsigned e;
  unsigned m[3];
} s21_decimal;

// BIT MANIPULATION //
void set_bit(s21_decimal *num, unsigned n, unsigned val);
unsigned get_bit(s21_decimal num, unsigned n);
void print_dec_bin(s21_decimal num);
bool get_sign(s21_decimal num);
void set_sign(s21_decimal *num, bool sign);
s21_decimal s21_shift_l(s21_decimal a, int n);
s21_decimal s21_shift_r(s21_decimal a, int n);

// DATA UNSIGNED GETTERS AND SETTERS //
unsigned get_exp(s21_decimal num);
void set_exp(s21_decimal *num, unsigned n);
void add_exp(s21_decimal *num, int n);
void sub_exp(s21_decimal *num, int n);

// COMPARATORS //
int s21_is_zero(s21_decimal a);
int s21_is_not_zero(s21_decimal a);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_module(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);

// ARITHMETICS //
s21_decimal add_fract(s21_decimal a, s21_decimal b);
s21_decimal sub_fract(s21_decimal a, s21_decimal b);
s21_decimal s21_add(s21_decimal a, s21_decimal b);
s21_decimal s21_sub(s21_decimal a, s21_decimal b);
s21_decimal s21_mul(s21_decimal a, s21_decimal b);
// s21_decimal div_fract(s21_decimal a, s21_decimal b);
void normalize_decs(s21_decimal *a, s21_decimal *b);

// PRINT DECIMAL (WAY TO HEAVY!!!) //
void print_list(char* list, char exp);
char *form_list_from_num(char *list, char num, char zeroes);
char *add_list_to_list(char *a, char *b);
char *mul_list(char *a, char *b);
char *form_list_from_dec(char *result, s21_decimal num);
void print_dec(s21_decimal *num);

// TESTS //
void add_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);
void mul_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);
void sub_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);

#endif
