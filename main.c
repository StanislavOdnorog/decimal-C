#include "main.h"
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

void div_helper(s21_decimal *a, s21_decimal *b, s21_decimal *remainder, s21_decimal one, s21_decimal ten, s21_decimal b_copy, int *new_exp, s21_decimal *res, s21_decimal *mult){
    *remainder = one;
    *b = b_copy;
    normalize_decs(a, b);
    if (s21_is_greater(*b, *a)) {
      (*new_exp)++;
      *a = s21_mul(*a, ten);
      *res = s21_mul(*res, ten);
      *mult = s21_mul(*mult, ten);
    }
    else {
      while (s21_is_less_or_equal(*b, *a)) {
        *b = s21_shift_l(*b, 1);
        *remainder = s21_shift_l(*remainder, 1);
      }

      do {
        if (s21_is_greater_or_equal(*a, *b)){
          *a = s21_sub(*a, *b);
          *res = s21_add(*res, *remainder);
        }
        *remainder = s21_shift_r(*remainder, 1);
        *b = s21_shift_r(*b, 1);
      } while (s21_is_not_zero(*remainder));
    }
}


s21_decimal s21_div(s21_decimal a, s21_decimal b) {
  s21_decimal zero = ZERO;
  s21_decimal one = ONE;
  s21_decimal res = ONE;
  s21_decimal remainder = ONE;
  s21_decimal mult = ONE;
  s21_decimal five = FIVE;
  s21_decimal ten = TEN;
  s21_decimal max = MAX;

  int new_exp = get_exp(b) - get_exp(b);
  unsigned new_sign = get_sign(a) == get_sign(b);
  set_sign(&a, 1);
  set_sign(&b, 1);


  if (s21_is_zero(b) || s21_is_zero(b))
    return zero;

  if (s21_is_equal(a, b)){
    set_sign(&remainder, new_sign);
    return remainder;
  }

  s21_decimal b_copy = b;
  while (s21_is_not_zero(a) && s21_is_less_or_equal(res, max)) {
    div_helper(&a, &b, &remainder, one, ten, b_copy, &new_exp, &res, &mult);
  }
  if (s21_is_greater(res,max)){
    div_helper(&a, &b, &remainder, one, ten, b_copy, &new_exp, &res, &mult);
  }

  normalize_decs(&a,&b_copy);
  set_exp(&a,0);
  set_exp(&res,0);
  set_exp(&b_copy,0);

  if (a.m[0]*10 / b_copy.m[0] >= 5){
    res = s21_add(res, one);
  }

  res = sub_fract(res, mult);
  set_exp(&res, new_exp);
  set_sign(&res, new_sign);
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

// CONVERTERS //
int s21_from_int_to_decimal(int src, s21_decimal *dst){
  // TODO: make error exception
  bool sign = src > 0;
  dst->e = 0;
  set_sign(dst, sign);
  if (!sign)
    src = -src;
  dst->m[0] = src;
  return 1;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  // TODO: make error exception

  s21_decimal ten = TEN;
  s21_decimal one = ONE;
  unsigned new_sign = get_sign(src);
  set_sign(&src, 1);
  unsigned temp = 0;

  normalize_decs(&src, &one);
  if (s21_is_greater(src, one)) {
      unsigned mult = 1;
    char result[MAX_10_EXP+1] = {0};
    form_list_from_dec(result, src);
    for (int i = MAX_10_EXP - 1 - get_exp(src); i > 0; i--){
      temp += result[i] * mult;
      mult *= 10;
    }
  }
  *dst = temp;
  
  if (!new_sign)
    *dst = -*dst;

  return 1;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst){
  // TODO: make error exception
  s21_decimal ten = TEN;
  s21_decimal temp = ZERO;
  bool sign = src > 0;
  dst->e = 0;
  set_sign(dst, 1);
  if (!sign)
    src = -src;

  unsigned *ptr = (unsigned*) &src;
  dst->m[0] = trunc(src);

  unsigned exp = 0;
  while ((float)src != (float)trunc(src) && exp < 7){
    src -= trunc(src);
    src *= 10.0;

    s21_from_int_to_decimal((int)src, &temp);

    *dst = s21_mul(*dst, ten);
    *dst = s21_add(*dst, temp);
    exp++;
  }

  set_exp(dst, exp);
  set_sign(dst, sign);

  return 1;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst){
  // TODO: make error exception
  s21_decimal ten = TEN;
  s21_decimal one = ONE;
  unsigned new_sign = get_sign(src);
  set_sign(&src, 1);

  unsigned temp = 0;
  normalize_decs(&src, &one);
  unsigned mult = 1;
  char result[MAX_10_EXP+1] = {0};
  form_list_from_dec(result, src);
  for (int i = MAX_10_EXP - 1; i > 0; i--){
    temp += result[i] * mult;
    mult *= 10;
  }

  float res = 0.;

  *dst = (double)temp;
  for (int i = 0; i < get_exp(src); i++) {
    *dst /= (double)round(10.0);
  }

  if (!new_sign)
    *dst = -*dst;

  return 1;
}

// ROUNDERS //
int s21_floor(s21_decimal value, s21_decimal *result){
  // TODO: make error exception
  s21_decimal five = FIVE;
  s21_decimal one = ONE;

  set_exp(&five, 1);
  unsigned new_sign = get_sign(value);
  set_sign(&value, 1);

  s21_truncate(value, result);

  normalize_decs(&value, result);
  s21_decimal rest = s21_sub(value, *result);

  if (!new_sign)
    *result = s21_add(*result, one);

  set_sign(result, new_sign);
  return 1;
}

int s21_round(s21_decimal value, s21_decimal *result){
  // TODO: make error exception
  s21_decimal five = FIVE;
  s21_decimal one = ONE;

  set_exp(&five, 1);
  unsigned new_sign = get_sign(value);
  set_sign(&value, 1);

  s21_truncate(value, result);

  normalize_decs(&value, result);
  s21_decimal rest = s21_sub(value, *result);
  if (s21_is_greater_or_equal(rest, five)){
    *result = s21_add(*result, one);
  }

  set_sign(result, new_sign);
  return 1;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  // TODO: make error exception
  int res = 0;
  s21_from_decimal_to_int(value, &res);
  s21_from_int_to_decimal(res, result);
  return 1;
}

int s21_negate(s21_decimal value, s21_decimal *result){
  // TODO: make error exception
  set_exp(&value, !get_exp(value));
  *result = value;
  return 1;
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
  s21_decimal result = ZERO;
  s21_from_int_to_decimal(value, &result);

  print_dec(&result);
  // print_dec_bin(result);
  printf("------------------\n");
}


void from_float_to_dec_test(float value){
  printf("%f\n", value);
  // print_dec_bin(test);
  printf("FLOATtoDEC=\n");
  s21_decimal result = ZERO;
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


void truncate_test(s21_decimal value){
  print_dec(&value);
  // print_dec_bin(test);
  
  printf("Truncate=\n");
  s21_decimal result = ZERO;
  s21_truncate(value, &result);

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
  div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 1, 4444, 0, MAX_10_EXP-5);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 22233, 0, MAX_10_EXP-5);

  printf("\n------ KATYA'S TESTS ------\n");
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 0, 0, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 00, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 00, 1, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 398, 2, 2, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 200001, 3, 5, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 2, 3, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 200001, 7, 5, 2);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 355, 113, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 3, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 888888889, 9, 5, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 555556548, 7, 4, 0);

  printf("\n------ CONVERETER TESTS ------\n");
  printf("-------- INT TO DEC --------\n");
  from_int_to_dec_test(0);
  from_int_to_dec_test(4);
  from_int_to_dec_test(10);
  from_int_to_dec_test(2147483647);
  from_int_to_dec_test(-0);
  from_int_to_dec_test(-4);
  from_int_to_dec_test(-10);
  from_int_to_dec_test(-2147483648);

  printf("-------- DEC TO INT --------\n");
  from_dec_to_int_test(SIGN_HEX_POS, 0, 0);
  from_dec_to_int_test(SIGN_HEX_NEG, 1, 0);
  from_dec_to_int_test(SIGN_HEX_POS, 1, 0);
  from_dec_to_int_test(SIGN_HEX_POS, 1, MAX_10_EXP-2);
  from_dec_to_int_test(SIGN_HEX_POS, 999999999, MAX_10_EXP-7);
  from_dec_to_int_test(SIGN_HEX_POS, 999999999, 0);
  from_dec_to_int_test(SIGN_HEX_POS, 1111111111, 5);
  from_dec_to_int_test(SIGN_HEX_POS, 2147483647, 0);
  from_dec_to_int_test(SIGN_HEX_NEG, 2147483648, 0);
  from_dec_to_int_test(SIGN_HEX_POS, 999999999, 8);
  from_dec_to_int_test(SIGN_HEX_POS, 299999999, 7);

  printf("-------- TRUNCATE --------\n");
  s21_decimal five = FIVE;
  s21_decimal ten = TEN;
  s21_decimal one = ONE;
  s21_decimal max = MAX;
  truncate_test(five);
  truncate_test(ten);
  truncate_test(s21_div(s21_add(one, one), s21_add(s21_add(one,one), one)));
  truncate_test(s21_div(one,s21_mul(ten, five)));
  truncate_test(s21_mul(s21_mul(ten, ten), s21_mul(ten, ten)));
  truncate_test(s21_div(one, s21_mul(s21_mul(s21_mul(s21_mul(five,five),s21_mul(five,five)),s21_mul(s21_mul(five,five),s21_mul(five,five))),s21_mul(s21_mul(s21_mul(five,five),s21_mul(five,five)),s21_mul(s21_mul(five,five),s21_mul(five,five))))));
  truncate_test(s21_div(s21_sub(max, s21_mul(ten,ten)),s21_mul(s21_mul(s21_mul(s21_mul(ten,ten), ten), s21_mul(s21_mul(ten,ten), s21_mul(ten,ten))),s21_mul(s21_mul(s21_mul(ten,ten), s21_mul(ten,ten)), s21_mul(s21_mul(ten,ten), s21_mul(s21_mul(ten,ten),s21_mul(s21_mul(ten,ten), s21_mul(ten,ten))))))));
  truncate_test(s21_sub(s21_div(one, five), s21_mul(one, ten)));

  printf("-------- FROM FLOAT TO DEC --------\n");
  from_float_to_dec_test(0.0);
  from_float_to_dec_test(4.99);
  from_float_to_dec_test(10.000001);
  from_float_to_dec_test(214.424);
  from_float_to_dec_test(-0.224);
  from_float_to_dec_test(0.5);
  from_float_to_dec_test(0.75);
  from_float_to_dec_test(-2147.2424);


  printf("-------- DECT TO FLOAT --------\n");
  from_dec_to_float_test(SIGN_HEX_POS, 0, 0);
  from_dec_to_float_test(SIGN_HEX_NEG, 1, 0);
  from_dec_to_float_test(SIGN_HEX_POS, 1, 0);
  from_dec_to_float_test(SIGN_HEX_POS, 1, MAX_10_EXP-2);
  from_dec_to_float_test(SIGN_HEX_POS, 999999929, MAX_10_EXP-7);
  from_dec_to_float_test(SIGN_HEX_POS, 999999, 2);
  from_dec_to_float_test(SIGN_HEX_POS, 1111111111, 5);
  from_dec_to_float_test(SIGN_HEX_POS, 214748, 3);
  from_dec_to_float_test(SIGN_HEX_NEG, 214748, 0);
  from_dec_to_float_test(SIGN_HEX_POS, 9999999, 4);
  from_dec_to_float_test(SIGN_HEX_POS, 2999999, 7);

  printf("-------- ROUND --------\n");
  round_test(SIGN_HEX_POS, 0, 0);
  round_test(SIGN_HEX_NEG, 1, 0);
  round_test(SIGN_HEX_POS, 1, 0);
  round_test(SIGN_HEX_POS, 1, MAX_10_EXP-2);
  round_test(SIGN_HEX_POS, 999999929, MAX_10_EXP-7);
  round_test(SIGN_HEX_POS, 999999, 2);
  round_test(SIGN_HEX_POS, 1111111111, 5);
  round_test(SIGN_HEX_POS, 214748, 3);
  round_test(SIGN_HEX_NEG, 214748, 0);
  round_test(SIGN_HEX_POS, 9999999, 4);
  round_test(SIGN_HEX_POS, 2999999, 7);

  printf("-------- FLOOR --------\n");
  floor_test(SIGN_HEX_POS, 0, 0);
  floor_test(SIGN_HEX_NEG, 1, 0);
  floor_test(SIGN_HEX_POS, 1, 0);
  floor_test(SIGN_HEX_POS, 1, MAX_10_EXP-2);
  floor_test(SIGN_HEX_NEG, 999999929, MAX_10_EXP-7);
  floor_test(SIGN_HEX_POS, 999999, 2);
  floor_test(SIGN_HEX_POS, 1111111111, 5);
  floor_test(SIGN_HEX_POS, 214748, 3);
  floor_test(SIGN_HEX_NEG, 214748, 0);
  floor_test(SIGN_HEX_POS, 9999999, 4);
  floor_test(SIGN_HEX_POS, 2999999, 7);

  return EXIT_SUCCESS;
}
