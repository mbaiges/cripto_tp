# Compiler flags

# extra: 
# -fsanitize=address
CFLAGS= -Wall -Wextra -Wno-implicit-fallthrough -Wno-unused-function -Wno-unused-parameter -Wno-unused-result \
                -pedantic -pedantic-errors -std=c99 -D_DEFAULT_SOURCE -O3 -g \
                $(MYCFLAGS)

CTESTFLAGS= -lcheck -lm -lrt -lsubunit -pthread $(CFLAGS) $(MYCFLAGS)

# Context

C_SOURCE_CONTEXT= src/main
C_UTILS_SOURCE_CONTEXT= $(C_SOURCE_CONTEXT)/utils

C_TARGET_CONTEXT= target/main

# Test Context

C_TEST_SOURCE_CONTEXT= src/test
C_UTILS_TEST_SOURCE_CONTEXT= $(C_TEST_SOURCE_CONTEXT)/utils

C_TEST_TARGET_CONTEXT= target/test
C_UTILS_TEST_TARGET_CONTEXT= $(C_TEST_TARGET_CONTEXT)/utils

# Test

# Utils

BMP_TEST_OUT= $(C_UTILS_TEST_TARGET_CONTEXT)/bmp_handler_test
BMP_TEST_DEPS= $(C_UTILS_TEST_SOURCE_CONTEXT)/bmp_handler_test.c

GALOIS_TEST_OUT= $(C_UTILS_TEST_TARGET_CONTEXT)/galois_test
GALOIS_TEST_DEPS= $(C_UTILS_TEST_SOURCE_CONTEXT)/galois_test.c

POLYNOMIAL_TEST_OUT= $(C_UTILS_TEST_TARGET_CONTEXT)/polynomial_test
POLYNOMIAL_TEST_DEPS= $(C_UTILS_TEST_SOURCE_CONTEXT)/polynomial_test.c

TEST_OUT= $(BMP_TEST_OUT) $(GALOIS_TEST_OUT) $(POLYNOMIAL_TEST_OUT)

# Outputs

SS_OUT = $(C_TARGET_CONTEXT)/ss
SS_DEPS = $(C_UTILS_SOURCE_CONTEXT)/secret.c $(C_UTILS_SOURCE_CONTEXT)/arguments.c $(C_UTILS_SOURCE_CONTEXT)/polynomial.c $(C_UTILS_SOURCE_CONTEXT)/bmp_handler.c $(C_UTILS_SOURCE_CONTEXT)/galois.c $(C_SOURCE_CONTEXT)/main.c 	

OUT= $(SS_OUT) $(TEST_OUT)

all: $(SS_OUT)

$(SS_OUT): $(SS_DEPS)
	@mkdir -p $(C_TARGET_CONTEXT)
	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CFLAGS)

test: $(TEST_OUT)

tests_check: $(TEST_OUT)
	./$(BMP_TEST_OUT)
	./$(GALOIS_TEST_OUT)
	./$(POLYNOMIAL_TEST_OUT)

$(BMP_TEST_OUT): $(BMP_TEST_DEPS)
	@mkdir -p $(C_UTILS_TEST_TARGET_CONTEXT)	
	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CTESTFLAGS)

$(GALOIS_TEST_OUT): $(GALOIS_TEST_DEPS)
	@mkdir -p $(C_UTILS_TEST_TARGET_CONTEXT)	
	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CTESTFLAGS)

$(POLYNOMIAL_TEST_OUT): $(POLYNOMIAL_TEST_DEPS)
	@mkdir -p $(C_UTILS_TEST_TARGET_CONTEXT)	
	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CTESTFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all	clean
