#include "../s21_decimal.h"
 
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
  s21_decimal zero = ZERO();
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();

  int error_code = 0;
  if(s21_is_greater(value_1, inf) || s21_is_greater(value_2, inf))
    error_code = 1;
  if(s21_is_less(value_1, neg_inf) || s21_is_less(value_2, neg_inf))
    error_code = 2;


  normalize_decs(&value_1, &value_2);
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
  return error_code;
}
