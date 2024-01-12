#include "../s21_decimal.h"

int main(void) {
  s21_decimal decimal = ONE();
  print_decimal_binary(decimal);
  decimal = shift_decimal_left(decimal, MANTIS_END);
  print_decimal_binary(decimal);
  decimal = shift_decimal_right(decimal, MANTIS_END);
  print_decimal_binary(decimal);
  // // MAX AND MIN VALUES //
  //
  // printf("---------MAX POS NUMBER--------\n");
  // s21_decimal max_pos = {SIGN_HEX_POS, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
  // // print_dec_bin(max_pos);
  // print_dec(&max_pos);
  //
  // printf("\n");
  // printf("---------MAX NEG NUMBER--------\n");
  // s21_decimal max_neg = {SIGN_HEX_NEG, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
  // // print_dec_bin(max_neg);
  // print_dec(&max_neg);
  //
  // printf("\n");
  // printf("---------MIN POS NUMBER--------\n");
  // s21_decimal min_pos = {SIGN_HEX_POS, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
  // set_exp(&min_pos, MAX_DECIMAL_EXP);
  // // print_dec_bin(min_pos);
  // print_dec(&min_pos);
  //
  // printf("\n");
  // printf("---------MIN NEG NUMBER--------\n");
  // s21_decimal min_neg = {SIGN_HEX_NEG, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
  // set_exp(&min_neg, MAX_DECIMAL_EXP);
  // // print_dec_bin(min_neg);
  // print_dec(&min_neg);
  //
  // printf("\n");
  //
  // // ARITHMETICS //
  // add_test(SIGN_HEX_NEG, SIGN_HEX_POS, 3127, 751, 5, 10);
  // add_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 567, 333, 1, 1);
  // add_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 0);
  // add_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342342912, 112321, 20, 0);
  // add_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 20);
  //
  //
  // sub_test(SIGN_HEX_NEG, SIGN_HEX_POS, 3127, 751, 5, 10);
  // sub_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 567, 333, 1, 1);
  // sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 0);
  // sub_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342342912, 112321, 20, 0);
  // sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 233, 112, 10, 1);
  // sub_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_DECIMAL_EXP - 1);

  // mul_test(SIGN_HEX_NEG, SIGN_HEX_POS, 3127, 751, 5, 10);
  // mul_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 567, 333, 1, 1);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 0);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342342912, 112321, 20, 0);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 234234, 112321, 0, 20);
  //
  // mul_test(SIGN_HEX_NEG, SIGN_HEX_POS, 6000, 200, 5, 10);
  // mul_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 356, 2, 1, 1);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 245, 245, 0, 0);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342, 111, 15, 0);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 233, 112, 10, 1);
  // mul_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_DECIMAL_EXP - 1);
  //
  // div_test(SIGN_HEX_NEG, SIGN_HEX_POS, 6000, 200, 5, 10);
  // div_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 356, 2, 1, 1);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 245, 245, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 2342, 111, 15, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 233, 112, 10, 1);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 1, 0, MAX_DECIMAL_EXP - 1);
  //
  // div_test(SIGN_HEX_NEG, SIGN_HEX_POS, 354, 1, 4, 1);
  // div_test(SIGN_HEX_NEG, SIGN_HEX_NEG, 111, 9, 7, 1);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 456, 2, 1, 2);
  // div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 1, 4444, 0, MAX_DECIMAL_EXP - 5);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 22233, 0, MAX_DECIMAL_EXP - 5);
  //
  // printf("\n------ KATYA'S TESTS ------\n");
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 0, 0, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 00, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_NEG, 00, 1, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 398, 2, 2, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 200001, 3, 5, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 2, 3, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 200001, 7, 5, 2);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 355, 113, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 1, 3, 0, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 888888889, 9, 5, 0);
  // div_test(SIGN_HEX_POS, SIGN_HEX_POS, 555556548, 7, 4, 0);

  // printf("\n------ CONVERETER TESTS ------\n");
  // printf("-------- INT TO DEC --------\n");
  // from_int_to_dec_test(0);
  // from_int_to_dec_test(4);
  // from_int_to_dec_test(10);
  // from_int_to_dec_test(2147483647);
  // from_int_to_dec_test(-0);
  // from_int_to_dec_test(-4);
  // from_int_to_dec_test(-10);
  // from_int_to_dec_test(-2147483648);

  // printf("-------- DEC TO INT --------\n");
  // from_dec_to_int_test(SIGN_HEX_POS, 0, 0);
  // from_dec_to_int_test(SIGN_HEX_NEG, 1, 0);
  // from_dec_to_int_test(SIGN_HEX_POS, 1, 0);
  // from_dec_to_int_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP - 2);
  // from_dec_to_int_test(SIGN_HEX_POS, 999999999, MAX_DECIMAL_EXP - 7);
  // from_dec_to_int_test(SIGN_HEX_POS, 999999999, 0);
  // from_dec_to_int_test(SIGN_HEX_POS, 1111111111, 5);
  // from_dec_to_int_test(SIGN_HEX_POS, 2147483647, 0);
  // from_dec_to_int_test(SIGN_HEX_NEG, 2147483648, 0);
  // from_dec_to_int_test(SIGN_HEX_POS, 999999999, 8);
  // from_dec_to_int_test(SIGN_HEX_POS, 299999999, 7);
  //
  // printf("-------- TRUNCATE --------\n");
  // truncate_test(SIGN_HEX_POS, 0, 0);
  // truncate_test(SIGN_HEX_NEG, 1, 0);
  // truncate_test(SIGN_HEX_POS, 1, 0);
  // truncate_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP - 2);
  // truncate_test(SIGN_HEX_POS, 999999929, MAX_DECIMAL_EXP - 7);
  // truncate_test(SIGN_HEX_POS, 999999, 2);
  // truncate_test(SIGN_HEX_POS, 1111111111, 5);
  // truncate_test(SIGN_HEX_POS, 214748, 3);
  // truncate_test(SIGN_HEX_NEG, 214748, 0);
  // truncate_test(SIGN_HEX_POS, 9999999, 4);
  // truncate_test(SIGN_HEX_POS, 2999999, 7);
  //
  // printf("-------- FROM FLOAT TO DEC --------\n");
  // from_float_to_dec_test(0.0);
  // from_float_to_dec_test(4.99);
  // from_float_to_dec_test(10.000001);
  // from_float_to_dec_test(214.424);
  // from_float_to_dec_test(-0.224);
  // from_float_to_dec_test(0.5);
  // from_float_to_dec_test(0.75);
  // from_float_to_dec_test(-2147.2424);
  //
  // from_float_to_dec_test(0.39);
  // from_float_to_dec_test(-0.99);
  // from_float_to_dec_test(-4.42424);
  // from_float_to_dec_test(33333.123);
  // from_float_to_dec_test(-0.34343);
  // from_float_to_dec_test(-4240.2421);
  // from_float_to_dec_test(-0.00004);
  // from_float_to_dec_test(-2147.2424);
  //
  // printf("-------- DECT TO FLOAT --------\n");
  // from_dec_to_float_test(SIGN_HEX_POS, 0, 0);
  // from_dec_to_float_test(SIGN_HEX_NEG, 1, 0);
  // from_dec_to_float_test(SIGN_HEX_POS, 1, 0);
  // from_dec_to_float_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP - 2);
  // from_dec_to_float_test(SIGN_HEX_POS, 999999929, MAX_DECIMAL_EXP - 7);
  // from_dec_to_float_test(SIGN_HEX_POS, 999999, 2);
  // from_dec_to_float_test(SIGN_HEX_POS, 1111111111, 5);
  // from_dec_to_float_test(SIGN_HEX_POS, 214748, 3);
  // from_dec_to_float_test(SIGN_HEX_NEG, 214748, 0);
  // from_dec_to_float_test(SIGN_HEX_POS, 9999999, 4);
  // from_dec_to_float_test(SIGN_HEX_POS, 2999999, 7);
  //
  // from_dec_to_float_test(SIGN_HEX_POS, 123123, 0);
  // from_dec_to_float_test(SIGN_HEX_NEG, 999999, 0);
  // from_dec_to_float_test(SIGN_HEX_POS, 4214, 0);
  // from_dec_to_float_test(SIGN_HEX_POS, 412412, MAX_DECIMAL_EXP - 2);
  // from_dec_to_float_test(SIGN_HEX_NEG, 4124214, MAX_DECIMAL_EXP - 7);
  // from_dec_to_float_test(SIGN_HEX_POS, 4124214, 10);
  // from_dec_to_float_test(SIGN_HEX_POS, 123, 4);
  // from_dec_to_float_test(SIGN_HEX_NEG, 453621, 6);
  // from_dec_to_float_test(SIGN_HEX_NEG, 123, 1);
  // from_dec_to_float_test(SIGN_HEX_POS, 4444, 3);
  // from_dec_to_float_test(SIGN_HEX_POS, 4521, 2);
  //
  // printf("-------- ROUND --------\n");
  // round_test(SIGN_HEX_POS, 0, 0);
  // round_test(SIGN_HEX_NEG, 1, 0);
  // round_test(SIGN_HEX_POS, 1, 0);
  // round_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP - 2);
  // round_test(SIGN_HEX_POS, 999999929, MAX_DECIMAL_EXP - 7);
  // round_test(SIGN_HEX_POS, 999999, 2);
  // round_test(SIGN_HEX_POS, 1111111111, 5);
  // round_test(SIGN_HEX_POS, 214748, 3);
  // round_test(SIGN_HEX_NEG, 214748, 0);
  // round_test(SIGN_HEX_POS, 9999999, 4);
  // round_test(SIGN_HEX_POS, 2999999, 7);
  //
  // printf("-------- FLOOR --------\n");
  // floor_test(SIGN_HEX_POS, 0, 0);
  // floor_test(SIGN_HEX_NEG, 1, 0);
  // floor_test(SIGN_HEX_POS, 1, 0);
  // floor_test(SIGN_HEX_POS, 1, MAX_DECIMAL_EXP - 2);
  // floor_test(SIGN_HEX_NEG, 999999929, MAX_DECIMAL_EXP - 7);
  // floor_test(SIGN_HEX_POS, 999999, 2);
  // floor_test(SIGN_HEX_POS, 1111111111, 5);
  // floor_test(SIGN_HEX_POS, 214748, 3);
  // floor_test(SIGN_HEX_NEG, 214748, 0);
  // floor_test(SIGN_HEX_POS, 9999999, 4);
  // floor_test(SIGN_HEX_POS, 2999999, 7);

  return EXIT_SUCCESS;
}
