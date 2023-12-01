#include "main.h"
#include <math.h>
#include <stdio.h>

// BIT MANIPULATION //
void set_bit(s21_decimal *num, unsigned n, unsigned val){
  unsigned *ptr = &num->e + n / PART_BITS;
  if (val)
    *ptr |= (1 << (n % PART_BITS));
  else
    *ptr &= ~(1 << (n % PART_BITS));
}

unsigned get_bit(s21_decimal num, unsigned n){
  unsigned *ptr = &num.e + n / PART_BITS;
  return (*ptr >> (n % PART_BITS)) & 1;
}

void print_dec_bin(s21_decimal num){
  for (unsigned i = MAX_BITS; i > 0; i--) {
    printf("%u", get_bit(num, i));
    if (i % PART_BITS == 0)
      printf(" | ");
  }
  printf("%u\n", get_bit(num, 0));
}

bool get_sign(s21_decimal num){
  return get_bit(num, SIGN_EXP);
}

void set_sign(s21_decimal *num, bool sign){
  set_bit(num, SIGN_EXP, sign);
}

s21_decimal s21_shift_l(s21_decimal a, int n){
  for (int i = MAX_BITS; i >= M1; i--) {
    if (i-n < M1)
      set_bit(&a, i, 0);
    else
      set_bit(&a, i, get_bit(a, i-n));
  }
  return a;
}

s21_decimal s21_shift_r(s21_decimal a, int n){
  for (int i = M1; i <= MAX_BITS; i++) {
    if (i+n > MAX_BITS)
      set_bit(&a, i, 0);
    else
      set_bit(&a, i, get_bit(a, i+n));

  }
  return a;
}

// DATA UNSIGNED GETTERS AND SETTERS //
unsigned get_exp(s21_decimal num){
  set_sign(&num, 0);
  return num.e;
}

void set_exp(s21_decimal *num, unsigned n){
  if (n < MAX_EXP_NUM){
    bool sign = get_sign(*num);
    num->e = n;
    set_sign(num, sign);
  }
}

void add_exp(s21_decimal *num, int n){
  if ((num->e + n - SIGN_HEX_POS) < MAX_EXP_NUM){
    bool sign = get_sign(*num);
    num->e += n;
    set_sign(num, sign);
  }
}

void sub_exp(s21_decimal *num, int n){
  if ((num->e - n - SIGN_HEX_POS) >= 0){
    bool sign = get_sign(*num);
    num->e -= n;
    set_sign(num, sign);
  }
}

// COMPARATORS //
int s21_is_zero(s21_decimal a){
  return a.m[0] == 0 && a.m[1] == 0 && a.m[2] == 0;
}

int s21_is_not_zero(s21_decimal a){
  return a.m[0] != 0 || a.m[1] != 0 || a.m[2] != 0; 
}

int s21_is_equal(s21_decimal a, s21_decimal b){
  return (a.e == b.e) && (a.m[0] == b.m[0]) && (a.m[1] == b.m[1]) && (a.m[2] == b.m[2]);
}

int s21_is_not_equal(s21_decimal a, s21_decimal b){
  return (a.e != b.e) || (a.m[0] != b.m[0]) || (a.m[1] != b.m[1]) || (a.m[2] != b.m[2]);
}

int s21_is_greater(s21_decimal a, s21_decimal b){
  if (get_sign(a) > get_sign(b)) return true;
  else if (get_sign(a) < get_sign(b)) return false;

  if (get_exp(a) < get_exp(b)) return true;
  else if (get_sign(a) > get_sign(b)) return false;

  for (int i = 2; i >= 0; i--){
    if (a.m[i] > b.m[i]) return true;
    else if (a.m[i] < b.m[i]) return false;
  }
  return false;
}

int s21_is_greater_module(s21_decimal a, s21_decimal b){
  set_sign(&a, 1);
  set_sign(&b, 1);
  return s21_is_greater(a,b);
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b){
  return s21_is_greater(a, b) || s21_is_equal(a, b);
}

int s21_is_less(s21_decimal a, s21_decimal b){
  return !s21_is_greater_or_equal(a,b);
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b){
  return s21_is_less(a, b) || s21_is_equal(a, b);
}

// ARITHMETICS //
s21_decimal add_fract(s21_decimal a, s21_decimal b){
  bool carry = 0;
  for (int i = M1; i <= MAX_BITS; i++) {
    if (carry){
      carry = get_bit(a,i) || get_bit(b,i);
      set_bit(&a,i, !(get_bit(a,i) ^ get_bit(b,i))); // XNOR
    }
    else {
      carry = get_bit(a,i) && get_bit(b,i);
      set_bit(&a,i, get_bit(a,i) ^ get_bit(b,i)); // XOR
    }
  }
  return a;
}

s21_decimal sub_fract(s21_decimal a, s21_decimal b) {
  s21_decimal result = {s21_is_greater_module(a, b) ? a.e : b.e,0,0,0};
  unsigned borrow = 0;

  for (int i = 0; i < 3; ++i) {
    unsigned a_part = a.m[i];
    unsigned b_part = b.m[i];

    if (a_part < b_part + borrow) {
      result.m[i] = (1LU << PART_BITS) + a_part - b_part - borrow;
      borrow = 1;
    } else {
      result.m[i] = a_part - b_part - borrow;
      borrow = 0;
    }
  }

  return result;
}

s21_decimal s21_add(s21_decimal a, s21_decimal b){  // TODO: FIX INPUT AND RETURN
  normalize_decs(&a, &b);

  s21_decimal zero = {SIGN_HEX_POS,0,0,0};

  s21_decimal a_pos = {a.e, a.m[0], a.m[1], a.m[2]};
  s21_decimal b_pos = {b.e, b.m[0], b.m[1], b.m[2]};
  set_sign(&a_pos, 1);
  set_sign(&b_pos, 1);

  s21_decimal res = {a_pos.e >= b_pos.e ? a_pos.e: b_pos.e,0,0,0};

  if (s21_is_greater(a, b)){
    if (s21_is_greater(a, zero)){
      if (s21_is_greater(b, zero)){
        res = add_fract(a_pos, b_pos);
      } else{
        if (s21_is_greater_module(a, b)){
          res = sub_fract(a_pos, b_pos);
        } else {
          res = sub_fract(b_pos, a_pos);
          set_sign(&res, 0);
        }
      }
    } else {
      res = add_fract(a_pos, b_pos);
      set_sign(&res, 0);
    }
  } else {
    if (s21_is_greater(a, zero)){
      res = add_fract(a_pos, b_pos);
    } else {
      if (s21_is_greater(b, zero)){
        if (s21_is_greater_module(a, b)){
          res = sub_fract(a_pos, b_pos);
          set_sign(&res, 0);
        } else {
          res = sub_fract(b_pos, a_pos);
        }
      } else{
        res = add_fract(a_pos, b_pos);
        set_sign(&res, 0);
      }
    }
  }
  return res;
}

s21_decimal s21_sub(s21_decimal a, s21_decimal b){ // TODO: FIX INPUT AND RETURN
  set_sign(&b, !get_sign(b));
  return s21_add(a, b);
}

s21_decimal s21_mul(s21_decimal a, s21_decimal b){ // TODO: FIX INPUT AND RETURN
  s21_decimal res = {SIGN_HEX_POS,0,0,0};
  s21_decimal zero = {SIGN_HEX_POS,0,0,0};
  unsigned new_exp = get_exp(a) + get_exp(b);
  new_exp = new_exp > MAX_10_EXP ? MAX_10_EXP : new_exp; 
  bool new_sign = get_sign(a) == get_sign(b);

  set_exp(&res, new_exp);
  set_sign(&res, new_sign);

  while (!s21_is_zero(b)) {
    res = add_fract(res, ((get_bit(b,M1) & 1)) ? a : zero); 
    a = s21_shift_l(a, 1);
    b = s21_shift_r(b, 1);
  }
  return res;
}

s21_decimal s21_pow(s21_decimal a, int exp){
  s21_decimal res = {a.e, a.m[0], a.m[1], a.m[2]};
  s21_decimal one = {a.e,1,0,0};
  if (exp == 0){
   return one; 
  }
  for (int i = 1; i < exp; i++) {
    res = s21_add(res, s21_mul(res, a));
  }
  return res;
}

s21_decimal s21_div(s21_decimal a, s21_decimal b) {
  s21_decimal ten = {SIGN_HEX_POS,10,0,0};
  s21_decimal one = {SIGN_HEX_POS,1,0,0};
  s21_decimal remainder = {SIGN_HEX_POS,1,0,0};
  s21_decimal res = {SIGN_HEX_POS,1,0,0};
  s21_decimal mult = {SIGN_HEX_POS,1,0,0};

  int new_exp = get_exp(a) - get_exp(b);
  normalize_decs(&a, &b);

  bool new_sign = get_sign(a) == get_sign(b);
  set_sign(&a, 1);
  set_sign(&b, 1);

  s21_decimal b_copy = b;

  if (s21_is_equal(a, b))
    return remainder;

  while (s21_is_not_zero(a) && get_bit(res, MAX_BITS-1) == 0) {  
    remainder = one;
    b = b_copy;
    if (s21_is_greater(b, a)) {
      new_exp++;
      a = s21_mul(a, ten);
      res = s21_mul(res, ten);
      mult = s21_mul(mult, ten);
    }
    else {
      while (s21_is_greater_or_equal(a, b)) {
        b = s21_shift_l(b, 1);
        remainder = s21_shift_l(remainder, 1);
      }
           
      do {
        if (s21_is_greater_or_equal(a, b)){
          a = sub_fract(a, b);
          res = add_fract(res, remainder);
        }
        remainder = s21_shift_r(remainder, 1);
        b = s21_shift_r(b, 1);
      } while (s21_is_not_zero(remainder));
    }

  }
  res = sub_fract(res, mult);
  set_sign(&res, new_sign);
  set_exp(&res, new_exp - MAX_10_EXP);
  return res;
}

void normalize_decs(s21_decimal *a, s21_decimal *b){
  s21_decimal ten = {SIGN_HEX_POS,10,0,0};
  set_exp(&ten, 1);
  while (get_exp(*a) != get_exp(*b)) {
    if(get_exp(*a) < get_exp(*b)){
      *a = s21_mul(*a,ten);
    } else {
      *b = s21_mul(*b,ten);
    }
  }
}

// PRINT DECIMAL (WAY TO HEAVY!!!) //
void print_list(char* list, char exp){
  int i = 0;
  if (exp == 0) {
    printf("0");
  }
  while (list[i] == 0 && i < exp-1) i++;
  for (; i < MAX_10_EXP; i++) {
    if (i == exp) printf(".");
    printf("%d", list[i]);
  }
  printf("\n");
}

char *form_list_from_num(char *list, char num, char zeroes){
  while (zeroes >= 0 && num > 0) {
    list[zeroes--] = num % 10;
    num /= 10;
  }
  return list;
}

char *add_list_to_list(char *a, char *b){
  char temp = 0;
  for (int i = MAX_10_EXP-1; i >= 0; i--) {
    a[i] += b[i] + temp; 
    temp = 0;
    if (a[i] > 9){
      temp = a[i] / 10;
      a[i] %= 10;
    }
  }
  return a;
}

char *mul_list(char *a, char *b){
  char result[MAX_10_EXP] = {0};
  char res;
  for (int i = MAX_10_EXP-1; i >= 0; i--) {
    for (int j = MAX_10_EXP-1; j >= 0; j--) {
      char num[MAX_10_EXP] = {0};
      res = a[i] * b[j];
      form_list_from_num(num, res, j + i - MAX_10_EXP+1);
      add_list_to_list(result, num);
    }
  }
  for (int i = 0; i < MAX_10_EXP; i++) {
    a[i] = result[i];
  }
  return a;
}

char *form_list_from_dec(char *result, s21_decimal num){
  char two[MAX_10_EXP] = {0};
  two[MAX_10_EXP-1] = 2;

  if (get_bit(num, M1)) result[MAX_10_EXP-1] = 1;

  for (unsigned i = M1+1; i <= MAX_BITS; i++) {
    if (!get_bit(num, i)) continue;

    char temp[MAX_10_EXP] = {0};
    temp [MAX_10_EXP-1] = 2;

    for (int j = 1; j < i-M1; j++)
      mul_list(temp, two);

    add_list_to_list(result, temp);
  }
  return result;
}

void print_dec(s21_decimal *num){
  char result[MAX_10_EXP+1] = {0};

  form_list_from_dec(result, *num);

  if (!get_sign(*num) && !s21_is_zero(*num))
    printf("-");
  print_list(result, MAX_10_EXP - get_exp(*num));
}

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
  s21_decimal result;
  result = s21_add(test, test2);

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
  result = s21_mul(test, test2);


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
  result = s21_div(test, test2);

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
  result = s21_sub(test, test2);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}

int main(int argc, char *argv[])
{
  // MAX AND MIN VALUES //

  printf("---------MAX POS NUMBER--------\n");
  s21_decimal max_pos = {SIGN_HEX_POS, 4294967295, 4294967295, 4294967295};
  // print_dec_bin(max_pos);
  print_dec(&max_pos);

  printf("\n");
  printf("---------MAX NEG NUMBER--------\n");
  s21_decimal max_neg = {SIGN_HEX_NEG, 4294967295, 4294967295, 4294967295};
  // print_dec_bin(max_neg);
  print_dec(&max_neg);

  printf("\n");
  printf("---------MIN POS NUMBER--------\n");
  s21_decimal min_pos = {SIGN_HEX_POS, 4294967295, 4294967295, 4294967295};
  set_exp(&min_pos, MAX_10_EXP);
  // print_dec_bin(min_pos);
  print_dec(&min_pos);

  printf("\n");
  printf("---------MIN NEG NUMBER--------\n");
  s21_decimal min_neg = {SIGN_HEX_NEG, 4294967295, 4294967295, 4294967295};
  set_exp(&min_neg, MAX_10_EXP);
  // print_dec_bin(min_neg);
  print_dec(&min_neg);

  printf("\n");

  // ARITHMETICS // 
  add_test(SIGN_HEX_NEG, SIGN_HEX_POS, 3127, 751, 5, 10);
  add_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 567, 333, 1, 1);
  add_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 0);
  add_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342342912, 112321, 20, 0);
  add_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 20);

  mul_test(SIGN_HEX_NEG, SIGN_HEX_POS, 3127, 751, 5, 10);
  mul_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 567, 333, 1, 1);
  mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 0);
  mul_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342342912, 112321, 20, 0);
  mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 20);

  sub_test(SIGN_HEX_NEG, SIGN_HEX_POS, 3127, 751, 5, 10);
  sub_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 567, 333, 1, 1);
  sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 0);
  sub_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342342912, 112321, 20, 0);
  sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 233, 112, 10, 1);
  sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_10_EXP-1);

  div_test(SIGN_HEX_NEG, SIGN_HEX_POS, 6000, 200, 5, 10);
  div_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 356, 2, 1, 1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 245, 245, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342, 111, 15, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 233, 112, 10, 1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_10_EXP-1);

  div_test(SIGN_HEX_NEG, SIGN_HEX_POS, 354, 1, 4, 1);
  div_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 111, 9, 7, 1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 456, 2, 1, 2);
  div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 1, 4444, 0, MAX_10_EXP-1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 22233, 0, MAX_10_EXP-1);

  return EXIT_SUCCESS;
}
