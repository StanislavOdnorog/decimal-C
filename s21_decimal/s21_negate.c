#include "../s21_decimal.h"
 
int s21_negate(s21_decimal value, s21_decimal *result){
  s21_decimal inf = INF();
  s21_decimal neg_inf = NEG_INF();

  if(s21_is_greater(value, inf))
    return 1;
  if(s21_is_less(value, neg_inf))
    return 1;
  set_exp(&value, !get_exp(value));
  *result = value;
  return 1;
}
