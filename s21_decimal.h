#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

// LIBRARIES //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// MACRO //
#define SIGN_HEX_POS 0x80
#define SIGN_HEX_NEG 0
#define SIGN_EXP_BITE_POS 7

#define MAX_EXP 128
#define MAX_DECIMAL_EXP 29
#define MAX_UNSIGNED 0xFFFFFFFF

#define MANTIS_1_PART 32
#define MANTIS_END 127
#define PART_BITS 32

#define ZERO() {SIGN_HEX_POS,0,0,0}
#define ONE() {SIGN_HEX_POS,1,0,0}
#define FIVE() {SIGN_HEX_POS,5,0,0}
#define TEN() {SIGN_HEX_POS,10,0,0}
#define MAX() {SIGN_HEX_POS,MAX_UNSIGNED,MAX_UNSIGNED,0xFFFFFFF}
#define INF() {0xFFFFFF,MAX_UNSIGNED,MAX_UNSIGNED,MAX_UNSIGNED}
#define NEG_INF() {0x7FFFFF,MAX_UNSIGNED,MAX_UNSIGNED,MAX_UNSIGNED}

// STRUCTURE //
typedef struct{
  unsigned data;
  unsigned mantis[3];
} s21_decimal;

// BIT MANIPULATION //
s21_decimal shift_decimal_left(s21_decimal decimal, int shift);
s21_decimal shift_decimal_right(s21_decimal decimal, int shift);

// DATA GETTERS AND SETTERS //
void set_bit(s21_decimal *decimal, unsigned position, bool value);
bool get_bit(s21_decimal decimal, unsigned position);
bool get_sign(s21_decimal decimal);
void set_sign(s21_decimal *decimal, bool sign);
unsigned get_exp(s21_decimal decimal);
void set_exp(s21_decimal *num, unsigned n);
void s21_decl_to_null(s21_decimal *decl);

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
void normalize_decs(s21_decimal *a, s21_decimal *b);
s21_decimal add_helper(s21_decimal a, s21_decimal b);
s21_decimal sub_helper(s21_decimal a, s21_decimal b);
void div_helper(
		s21_decimal *a,
		s21_decimal *b,
		s21_decimal *remainder,
		s21_decimal one,
		s21_decimal ten,
		s21_decimal b_copy,
    int *new_exp,
		s21_decimal *res,
		s21_decimal *mult);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// PRINT DECIMAL // 
void print_decimal_binary(s21_decimal decimal);
void print_list(char* list, char exp);
char *form_list_from_num(char *list, char num, char zeroes);
char *add_list_to_list(char *a, char *b);
char *mul_list(char *a, char *b);
char *form_list_from_dec(char *result, s21_decimal num);
void print_dec(s21_decimal *num);

// CONVERTERS //
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// ROUNDERS //
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// TESTS //
void add_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);
void mul_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);
void div_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);
void sub_test(unsigned sign1, unsigned sign2, unsigned num1, unsigned num2, unsigned exp1, unsigned exp2);
void from_dec_to_int_test(unsigned sign1, unsigned num1, unsigned exp1);
void from_int_to_dec_test(int value);
void from_float_to_dec_test(float value);
void from_dec_to_float_test(unsigned sign1, unsigned num1, unsigned exp1);
void truncate_test(unsigned sign1, unsigned num1, unsigned exp1);
void round_test(unsigned sign1, unsigned num1, unsigned exp1);
void floor_test(unsigned sign1, unsigned num1, unsigned exp1);

#endif
