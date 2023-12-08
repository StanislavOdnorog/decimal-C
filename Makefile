FLAGS= -lm -lsubunit -lcheck -lpthread -lrt
CC=gcc -Wno-error=missing-braces -Wextra -Werror -std=c11 
OS=MAC
FUNC_FILES_C= s21_decimal/*.c
FUNC_FILES_O= *.o
UNAME_S := $(shell uname -s)
GCOVFLAGS=-fprofile-arcs -ftest-coverage
LIBNAME=s21_decimal.a
GLFLAGS=--coverage
sourceFilesArray=$(ls sourceFiles)

EXECS=./s21_test_add ./s21_test_sub ./s21_test_mul ./s21_test_div ./s21_test_is_less ./s21_test_is_less_or_equal ./s21_test_is_greater ./s21_test_greater_or_equal ./s21_test_is_equal ./s21_test_is_not_equal ./s21_test_int_conversion ./s21_test_float_conversion ./s21_test_floor ./s21_test_round ./s21_test_truncate ./s21_test_negate

ifeq ($(UNAME_S),Linux)
	CHECKFLAGS= -lcheck
	OPEN_CMD = open
	ADD_LIB = -lcheck -lm -lpthread -lrt -lsubunit
	OS = LINUX
endif
 
ifeq ($(UNAME_S),Darwin)
	CHECKFLAGS= $(shell pkg-config --cflags --libs check)
	OPEN_CMD = open
endif
 
.PHONY: simple_test

install:
	if [ $(OS) = "LINUX" ]; then \
		sudo apt-get install -y lcov; \
		sudo apt-get install -y check; \
		sudo apt-get install -y libcapture-tiny-perl; \
		sudo apt-get install -y libdatetime-perl; \
	else \
		brew install lcov; \
	fi
 
all: s21_decimal.a
 
peer: gcov_report
 
build_object_files:
	$(CC) $(FLAGS) $(GCOVFLAGS) $(GLFLAGS) -c $(FUNC_FILES_C)
 
build_library:
	ar rcs $(LIBNAME) $(FUNC_FILES_O)
 
s21_decimal.a:
	$(CC) $(FLAGS) -c $(FUNC_FILES_C)
	make build_library
	rm -rf *.o
 
simple_test: 
	$(CC) ./s21_decimal/*.c s21_decimal_simple_tests/*.c -lm -o simple_test
	./simple_test

test: s21_decimal.a
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_add_test.c main.c -L. s21_decimal.a  -o s21_test_add -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_sub_test.c main.c -L. s21_decimal.a  -o s21_test_sub -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_mul_test.c main.c -L. s21_decimal.a  -o s21_test_mul -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_div_test.c main.c -L. s21_decimal.a  -o s21_test_div -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_is_less_test.c main.c -L. s21_decimal.a  -o s21_test_is_less -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_is_less_or_equal_test.c main.c -L. s21_decimal.a  -o s21_test_is_less_or_equal -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_is_greater_test.c main.c -L. s21_decimal.a  -o s21_test_is_greater -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_is_greater_or_equal_test.c main.c -L. s21_decimal.a  -o s21_test_is_greater_or_equal -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_is_equal_test.c main.c -L. s21_decimal.a  -o s21_test_is_equal -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_is_not_equal_test.c main.c -L. s21_decimal.a  -o s21_test_is_not_equal -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_int_conversion_test.c main.c -L. s21_decimal.a  -o s21_test_int_conversion -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_float_conversion_test.c main.c -L. s21_decimal.a  -o s21_test_float_conversion -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_floor_test.c main.c -L. s21_decimal.a  -o s21_test_floor -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_round_test.c main.c -L. s21_decimal.a  -o s21_test_round -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_truncate_test.c main.c -L. s21_decimal.a  -o s21_test_truncate -lcheck $(ADD_LIB)
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/s21_negate_test.c main.c -L. s21_decimal.a  -o s21_test_negate -lcheck $(ADD_LIB)
	make start_tests
 
start_tests:
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_add
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_sub
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_mul
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_div
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_is_less
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_is_less_or_equal
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_is_greater
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_is_greater_or_equal
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_is_equal
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_is_not_equal
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_int_conversion
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_float_conversion
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_floor
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_round
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_truncate
	# valgrind --tool=memcheck -q --leak-check=yes ./s21_test_negate
	./s21_test_add
	./s21_test_sub
	./s21_test_mul
	./s21_test_div
	./s21_test_is_less
	./s21_test_is_less_or_equal
	./s21_test_is_greater
	./s21_test_is_greater_or_equal
	./s21_test_is_equal
	./s21_test_is_not_equal
	./s21_test_int_conversion
	./s21_test_float_conversion
	./s21_test_floor
	./s21_test_round
	./s21_test_truncate
	./s21_test_negate
 
 
gcov_report: install clean build_object_files build_library test
	lcov -o tests.info -c -d .  
	genhtml -o report tests.info
	$(OPEN_CMD) report/index.html
	make clean
 
 
clean:
	rm -rf *.o
	rm -rf *.gcno
	rm -rf *.gcov
	rm -rf *.gcda
	rm -rf *.a
	rm -rf *.info
	rm -rf $(EXECS)
	rm -rf ./simple_test
 
rebuild:
	make clean
	make
 
