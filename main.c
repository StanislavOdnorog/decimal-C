#include "main.h"

// BIT MANIPULATION //
void set_bit(s21_decimal *decimal, unsigned position, bool value){
  // Find the bit in "position" in "decimal" and set it to "value"
  unsigned *ptr = &decimal->data + position / PART_BITS;
  if (value)
    *ptr |= (1 << (position % PART_BITS));
  else
    *ptr &= ~(1 << (position % PART_BITS));
}

bool get_bit(s21_decimal decimal, unsigned position){
  // Find the bit in "position" in "decimal" and return it 
  unsigned *ptr = &decimal.data + position / PART_BITS;
  return (*ptr >> (position % PART_BITS)) & 1;
}

void print_decimal_binary(s21_decimal decimal){
  // Print the binary representation of "decimal"
  for (unsigned i = MANTIS_END; i >= 0; i--) {
    // Print bite (0 or 1)
    printf("%u", get_bit(decimal, i));
    if (i % PART_BITS == 0 && i != 0)
      // Print separator
      printf(" | ");
  }
  printf("\n");
}

s21_decimal shift_decimal_left(s21_decimal decimal, int shift){
  // Shift "decimal" "shift" times left and return it
  for (int i = MANTIS_END; i >= MANTIS_1_PART; i--) {
    if (i-shift < MANTIS_1_PART)
      set_bit(&decimal, i, 0);
    else
      set_bit(&decimal, i, get_bit(decimal, i-shift));
  }
  return decimal;
}

s21_decimal shift_decimal_right(s21_decimal decimal, int shift){
  // Shift "decimal" "shift" times right and return it
  for (int i = MANTIS_1_PART; i <= MANTIS_END; i++) {
    if (i+shift > MANTIS_END)
      set_bit(&decimal, i, 0);
    else
      set_bit(&decimal, i, get_bit(decimal, i+shift));
  }
  return decimal;
}

// DATA GETTERS AND SETTERS //

void set_sign(s21_decimal *decimal, bool sign){
  // Find the sign position in "decimal" and set it to "sign" 
  set_bit(decimal, SIGN_EXP_BITE_POS, sign);
}

bool get_sign(s21_decimal decimal){
  // Find the sign in "decimal" and return it 
  return get_bit(decimal, SIGN_EXP_BITE_POS);
}

unsigned get_exp(s21_decimal decimal){
  // Find the exp in "decimal" and return it 
  set_sign(&decimal, 0);
  return decimal.data;
}

void set_exp(s21_decimal *decimal, unsigned exp){
  // Find the exp in "decimal" and set it to "exp" 
  if (exp < MAX_EXP){
    bool sign = get_sign(*decimal);

    decimal->data = exp;

    set_sign(decimal, sign);
  }
}

void add_exp(s21_decimal *decimal, unsigned value){
  // Find the exp in "decimal" and add "value" to it 
  if ((decimal->data + value - SIGN_HEX_POS) < MAX_EXP){
    bool sign = get_sign(*decimal);
    set_sign(decimal, 0);

    decimal->data += value;

    set_sign(decimal, sign);
  }
}

void sub_exp(s21_decimal *decimal, unsigned value){
  // Find the exp in "decimal" and subtract "value" from it 
  if ((decimal->data - value - SIGN_HEX_POS) >= 0){
    bool sign = get_sign(*decimal);
    set_sign(decimal, 0);

    decimal->data -= value;

    set_sign(decimal, sign);
  }
}

// COMPARATORS //
int s21_is_zero(s21_decimal a){
  return a.mantis[0] == 0 && a.mantis[1] == 0 && a.mantis[2] == 0;
}

int s21_is_not_zero(s21_decimal a){
  return a.mantis[0] != 0 || a.mantis[1] != 0 || a.mantis[2] != 0; 
}

int s21_is_equal(s21_decimal a, s21_decimal b){
  return (a.data == b.data) && (a.mantis[0] == b.mantis[0]) && (a.mantis[1] == b.mantis[1]) && (a.mantis[2] == b.mantis[2]);
}

int s21_is_not_equal(s21_decimal a, s21_decimal b){
  return (a.data != b.data) || (a.mantis[0] != b.mantis[0]) || (a.mantis[1] != b.mantis[1]) || (a.mantis[2] != b.mantis[2]);
}

int s21_is_greater(s21_decimal a, s21_decimal b){
  if (get_sign(a) > get_sign(b)) return true;
  else if (get_sign(a) < get_sign(b)) return false;

  if (get_exp(a) < get_exp(b)) return true;
  else if (get_sign(a) > get_sign(b)) return false;

  for (int i = 2; i >= 0; i--){
    if (a.mantis[i] > b.mantis[i]) return true;
    else if (a.mantis[i] < b.mantis[i]) return false;
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
void normalize_decs(s21_decimal *a, s21_decimal *b){
  s21_decimal ten = TEN;
  set_exp(&ten, 1);

  while (get_exp(*a) != get_exp(*b)) {
    if(get_exp(*a) < get_exp(*b)){
      s21_mul(*a,ten,a);
    } else {
      s21_mul(*b,ten,b);
    }
  }
}

s21_decimal add_helper(s21_decimal a, s21_decimal b){
  bool carry = 0; // Set the carry to zero
  
  for (int i = MANTIS_1_PART; i <= MANTIS_END; i++) { // Itterate through all bites
    if (carry){ // If there is something in carry
      carry = get_bit(a,i) || get_bit(b,i); // Carry is equal to or operation
      set_bit(&a,i, !(get_bit(a,i) ^ get_bit(b,i))); // execute XNOR operation
    }
    else { // If there is nothing in carry
      carry = get_bit(a,i) && get_bit(b,i); // Carry is equal to and operation 
      set_bit(&a,i, get_bit(a,i) ^ get_bit(b,i)); // execute XOR operation
    }
  }
  return a;
}

s21_decimal sub_helper(s21_decimal a, s21_decimal b) {
  s21_decimal result = {s21_is_greater_module(a, b) ? a.data : b.data,0,0,0};
  unsigned borrow = 0; // Set thee result to the lowest exp, set the borrow var to zero

  for (int i = 0; i < 3; ++i) {
    unsigned a_part = a.mantis[i]; // Make a copy of a mantis
    unsigned b_part = b.mantis[i]; // Make a copy of b mantis

    if (a_part < b_part + borrow) { // If a mantis is less than b mantis + borrow
      result.mantis[i] = (1LU << PART_BITS) + a_part - b_part - borrow;
      borrow = 1; // Set the result to 2^32 in binary + a mantis - b mantis - borrow, set borrow 1
    } else { // If a mantis is greater or equal than b mantis + borrow
      result.mantis[i] = a_part - b_part - borrow;
      borrow = 0; // Set the result to a mantis - b mantis - borrow, set borrow 0
    }
  }
  return result;
}

void div_helper(s21_decimal *a, s21_decimal *b, s21_decimal *remainder, s21_decimal one, s21_decimal ten, s21_decimal b_copy, int *new_exp, s21_decimal *res, s21_decimal *mult){
    *remainder = one;
    *b = b_copy;
    normalize_decs(a, b);
    if (s21_is_greater(*b, *a)) {
      (*new_exp)++;
      s21_mul(*a, ten, a);
      s21_mul(*res, ten, res);
      s21_mul(*mult, ten, mult);
    }
    else {
      while (s21_is_less_or_equal(*b, *a)) {
        *b = shift_decimal_left(*b, 1);
        *remainder = shift_decimal_left(*remainder, 1);
      }

      do {
        if (s21_is_greater_or_equal(*a, *b)){
          s21_sub(*a, *b, a);
          s21_add(*res, *remainder, res);
        }
        *remainder = shift_decimal_right(*remainder, 1);
        *b = shift_decimal_right(*b, 1);
      } while (s21_is_not_zero(*remainder));
    }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
  // TODO: FIX INPUT AND RETURN
  normalize_decs(&value_1, &value_2);
  s21_decimal zero = ZERO;
  s21_decimal value_1_copy = {value_1.data, value_1.mantis[0], value_1.mantis[1], value_1.mantis[2]};
  s21_decimal value_2_copy = {value_2.data, value_2.mantis[0], value_2.mantis[1], value_2.mantis[2]};
  set_sign(&value_1_copy, 1);
  set_sign(&value_2_copy, 1);
  s21_decimal res = {value_1_copy.data >= value_2_copy.data ? value_1_copy.data : value_2_copy.data,0,0,0};

  // LOGIC TO CALL ADD HELPER AND SUB HELPER
  if (s21_is_greater(value_1, value_2)){
    if (s21_is_greater(value_1, zero)){
      if (s21_is_greater(value_2, zero)){
        res = add_helper(value_1_copy, value_2_copy);
      } else{
        if (s21_is_greater_module(value_1, value_2)){
          res = sub_helper(value_1_copy, value_2_copy);
        } else {
          res = sub_helper(value_2_copy, value_1_copy);
          set_sign(&res, 0);
        }
      }
    } else {
      res = add_helper(value_1_copy, value_2_copy);
      set_sign(&res, 0);
    }
  } else {
    if (s21_is_greater(value_1, zero)){
      res = add_helper(value_1_copy, value_2_copy);
    } else {
      if (s21_is_greater(value_2, zero)){
        if (s21_is_greater_module(value_1, value_2)){
          res = sub_helper(value_1_copy, value_2_copy);
          set_sign(&res, 0);
        } else {
          res = sub_helper(value_2_copy, value_1_copy);
        }
      } else{
        res = add_helper(value_1_copy, value_2_copy);
        set_sign(&res, 0);
      }
    }
  }
  *result = res;
  return 0;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){ 
  // TODO: FIX INPUT AND RETURN
  set_sign(&value_2, !get_sign(value_2));
  s21_add(value_1, value_2, result);
  return 0;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
  // TODO: FIX INPUT AND RETURN
  s21_decimal res = ZERO;
  s21_decimal zero = ZERO;
  unsigned new_exp = get_exp(value_1) + get_exp(value_2);
  new_exp = new_exp > MAX_DECIMAL_EXP ? MAX_DECIMAL_EXP : new_exp; 
  bool new_sign = get_sign(value_1) == get_sign(value_2);

  set_exp(&res, new_exp);
  set_sign(&res, new_sign);

  while (!s21_is_zero(value_2)) {
    res = add_helper(res, ((get_bit(value_2,MANTIS_1_PART) & 1)) ? value_1 : zero); 
    value_1 = shift_decimal_left(value_1, 1);
    value_2 = shift_decimal_right(value_2, 1);
  }
  *result = res;
  return 0;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal zero = ZERO;
  s21_decimal one = ONE;
  s21_decimal res = ONE;
  s21_decimal remainder = ONE;
  s21_decimal mult = ONE;
  s21_decimal five = FIVE;
  s21_decimal ten = TEN;
  s21_decimal max = MAX;

  int new_exp = get_exp(value_2) - get_exp(value_2);
  unsigned new_sign = get_sign(value_1) == get_sign(value_2);
  set_sign(&value_1, 1);
  set_sign(&value_2, 1);

  if (s21_is_zero(value_2)){
    set_sign(&remainder, new_sign);
    *result = remainder;
    return 3;
  }

  if (s21_is_equal(value_1, value_2)){
    set_sign(&remainder, new_sign);
    *result = remainder;
    return 0;
  }

  s21_decimal b_copy = value_2;
  while (s21_is_not_zero(value_1) && s21_is_less_or_equal(res, max)) {
    div_helper(&value_1, &value_2, &remainder, one, ten, b_copy, &new_exp, &res, &mult);
  }
  if (s21_is_greater(res,max)){
    div_helper(&value_1, &value_2, &remainder, one, ten, b_copy, &new_exp, &res, &mult);
  }

  normalize_decs(&value_1,&b_copy);
  set_exp(&value_1,0);
  set_exp(&res,0);
  set_exp(&b_copy,0);

  if (value_1.mantis[0]*10 / b_copy.mantis[0] >= 5){
    s21_add(res, one, &res);
  }

  res = sub_helper(res, mult);
  set_exp(&res, new_exp);
  set_sign(&res, new_sign);
  *result = res;
  return 0;
}


// CONVERTERS //
int s21_from_int_to_decimal(int src, s21_decimal *dst){
  // TODO: make error exception
  bool sign = src > 0;
  dst->data = 0;
  set_sign(dst, sign);
  if (!sign)
    src = -src;
  dst->mantis[0] = src;
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
  if (s21_is_greater_or_equal(src, one)) {
      unsigned mult = 1;
    char result[MAX_DECIMAL_EXP+1] = {0};
    form_list_from_dec(result, src);
    for (int i = MAX_DECIMAL_EXP - 1 - get_exp(src); i >= 0; i--){
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
  dst->data = 0;
  set_sign(dst, 1);
  if (!sign)
    src = -src;

  unsigned *ptr = (unsigned*) &src;
  dst->mantis[0] = trunc(src);

  unsigned exp = 0;
  while ((float)src != (float)trunc(src) && exp < 7){
    src -= trunc(src);
    src *= 10.0;

    s21_from_int_to_decimal((int)src, &temp);

    s21_mul(*dst, ten, dst);
    s21_add(*dst, temp, dst);
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
  char result[MAX_DECIMAL_EXP+1] = {0};
  form_list_from_dec(result, src);
  for (int i = MAX_DECIMAL_EXP - 1; i > 0; i--){
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
  *result = value;
  s21_decimal five = FIVE;
  s21_decimal one = ONE;
  set_exp(&five, 1);

  unsigned new_sign = get_sign(value);
  set_sign(&value, 1);

  s21_truncate(value, result);

  s21_decimal rest;
  s21_sub(value, *result, &rest);

  if (!new_sign && s21_is_not_zero(rest))
    s21_add(*result, one, result);

  set_sign(result, new_sign);
  return 1;
}

int s21_round(s21_decimal value, s21_decimal *result){
  // TODO: make error exception
  *result = value;
  s21_decimal five = FIVE;
  s21_decimal one = ONE;
  set_exp(&five, 1);

  unsigned new_sign = get_sign(value);
  set_sign(&value, 1);

  s21_truncate(value, result);

  s21_decimal rest;
  s21_sub(value, *result, &rest);

  normalize_decs(&rest, &five);
  if (s21_is_greater_or_equal(rest, five))
    s21_add(*result, one, result);

  set_sign(result, new_sign);
  return 1;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  // TODO: make error exception
  int res = 0;
  *result = value;
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
  for (; i < MAX_DECIMAL_EXP; i++) {
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
  for (int i = MAX_DECIMAL_EXP-1; i >= 0; i--) {
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
  char result[MAX_DECIMAL_EXP] = {0};
  char res;
  for (int i = MAX_DECIMAL_EXP-1; i >= 0; i--) {
    for (int j = MAX_DECIMAL_EXP-1; j >= 0; j--) {
      char num[MAX_DECIMAL_EXP] = {0};
      res = a[i] * b[j];
      form_list_from_num(num, res, j + i - MAX_DECIMAL_EXP+1);
      add_list_to_list(result, num);
    }
  }
  for (int i = 0; i < MAX_DECIMAL_EXP; i++) {
    a[i] = result[i];
  }
  return a;
}

char *form_list_from_dec(char *result, s21_decimal num){
  char two[MAX_DECIMAL_EXP] = {0};
  two[MAX_DECIMAL_EXP-1] = 2;

  if (get_bit(num, MANTIS_1_PART)) result[MAX_DECIMAL_EXP-1] = 1;

  for (unsigned i = MANTIS_1_PART+1; i <= MANTIS_END; i++) {
    if (!get_bit(num, i)) continue;

    char temp[MAX_DECIMAL_EXP] = {0};
    temp [MAX_DECIMAL_EXP-1] = 2;

    for (int j = 1; j < i-MANTIS_1_PART; j++)
      mul_list(temp, two);

    add_list_to_list(result, temp);
  }
  return result;
}

void print_dec(s21_decimal *num){
  char result[MAX_DECIMAL_EXP+1] = {0};

  form_list_from_dec(result, *num);

  if (!get_sign(*num) && !s21_is_zero(*num))
    printf("-");
  print_list(result, MAX_DECIMAL_EXP - get_exp(*num));
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
  set_exp(&min_pos, MAX_DECIMAL_EXP);
  // print_dec_bin(min_pos);
  print_dec(&min_pos);

  printf("\n");
  printf("---------MIN NEG NUMBER--------\n");
  s21_decimal min_neg = {SIGN_HEX_NEG, 4294967295, 4294967295, 4294967295};
  set_exp(&min_neg, MAX_DECIMAL_EXP);
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
  sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_DECIMAL_EXP-1);

  div_test(SIGN_HEX_NEG, SIGN_HEX_POS, 6000, 200, 5, 10);
  div_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 356, 2, 1, 1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 245, 245, 0, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342, 111, 15, 0);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 233, 112, 10, 1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_DECIMAL_EXP-1);

  div_test(SIGN_HEX_NEG, SIGN_HEX_POS, 354, 1, 4, 1);
  div_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 111, 9, 7, 1);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 456, 2, 1, 2);
  div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 1, 4444, 0, MAX_DECIMAL_EXP-5);
  div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 22233, 0, MAX_DECIMAL_EXP-5);

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
  from_dec_to_int_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP-2);
  from_dec_to_int_test(SIGN_HEX_POS, 999999999, MAX_DECIMAL_EXP-7);
  from_dec_to_int_test(SIGN_HEX_POS, 999999999, 0);
  from_dec_to_int_test(SIGN_HEX_POS, 1111111111, 5);
  from_dec_to_int_test(SIGN_HEX_POS, 2147483647, 0);
  from_dec_to_int_test(SIGN_HEX_NEG, 2147483648, 0);
  from_dec_to_int_test(SIGN_HEX_POS, 999999999, 8);
  from_dec_to_int_test(SIGN_HEX_POS, 299999999, 7);

  printf("-------- TRUNCATE --------\n");
  truncate_test(SIGN_HEX_POS, 0, 0);
  truncate_test(SIGN_HEX_NEG, 1, 0);
  truncate_test(SIGN_HEX_POS, 1, 0);
  truncate_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP-2);
  truncate_test(SIGN_HEX_POS, 999999929, MAX_DECIMAL_EXP-7);
  truncate_test(SIGN_HEX_POS, 999999, 2);
  truncate_test(SIGN_HEX_POS, 1111111111, 5);
  truncate_test(SIGN_HEX_POS, 214748, 3);
  truncate_test(SIGN_HEX_NEG, 214748, 0);
  truncate_test(SIGN_HEX_POS, 9999999, 4);
  truncate_test(SIGN_HEX_POS, 2999999, 7);

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
  from_dec_to_float_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP-2);
  from_dec_to_float_test(SIGN_HEX_POS, 999999929, MAX_DECIMAL_EXP-7);
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
  round_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP-2);
  round_test(SIGN_HEX_POS, 999999929, MAX_DECIMAL_EXP-7);
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
  floor_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP-2);
  floor_test(SIGN_HEX_NEG, 999999929, MAX_DECIMAL_EXP-7);
  floor_test(SIGN_HEX_POS, 999999, 2);
  floor_test(SIGN_HEX_POS, 1111111111, 5);
  floor_test(SIGN_HEX_POS, 214748, 3);
  floor_test(SIGN_HEX_NEG, 214748, 0);
  floor_test(SIGN_HEX_POS, 9999999, 4);
  floor_test(SIGN_HEX_POS, 2999999, 7);

  return EXIT_SUCCESS;
}
