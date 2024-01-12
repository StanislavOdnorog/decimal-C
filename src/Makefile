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

FUNCTIONS=./s21_decimal/*.c

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
 
.PHONY: simple_test s21_decimal.a

all: clean s21_decimal.a gcov_report
 
build_object_files:
	$(CC) $(FLAGS) $(GCOVFLAGS) $(GLFLAGS) -c $(FUNC_FILES_C)
 
build_library:
	ar rcs $(LIBNAME) $(FUNC_FILES_O)
 
s21_decimal.a:
	$(CC) $(FLAGS) -c $(FUNC_FILES_C)
	make build_library
	rm -rf *.o
 
simple_test: 
	$(CC) ./s21_decimal/*.c ./s21_decimal_simple_tests/*.c -lm -o simple_test
	./simple_test

test: clean s21_decimal.a
	$(CC) $(CHECKFLAGS) $(GLFLAGS) $(GCOVFLAGS) s21_decimal_tests/*.c s21_decimal/*.c main.c -L. s21_decimal.a  -o s21_test -lcheck $(ADD_LIB)
	./s21_test
 
gcov_report: clean build_object_files build_library test
	lcov -o tests.info -c -d .  
	genhtml -o report tests.info
	$(OPEN_CMD) report/index.html
 
clean:
	rm -rf *.o
	rm -rf *.gcno
	rm -rf *.gcov
	rm -rf *.gcda
	rm -rf *.a
	rm -rf *.info
	rm -rf $(EXECS)
	rm -rf ./simple_test
	rm -rf ./s21_test
	rm -rf ./report
 
rebuild:
	make
 
