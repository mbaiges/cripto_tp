# Compiler flags

# extra: -Werror

CFLAGS= -Wall -Wextra -Wno-implicit-fallthrough -Wno-unused-function -Wno-unused-parameter -fsanitize=address \
                -pedantic -pedantic-errors -std=c11 -g \
                $(MYCFLAGS)

CTESTFLAGS= -Werror -lcheck -lm -lrt -lsubunit $(CFLAGS) $(MYCFLAGS)

# Context

C_SOURCE_CONTEXT= src/main
C_UTILS_SOURCE_CONTEXT= $(C_SOURCE_CONTEXT)/utils

# Test Context

C_TEST_SOURCE_CONTEXT= src/test
C_UTILS_TEST_SOURCE_CONTEXT= $(C_TEST_SOURCE_CONTEXT)/utils

C_TEST_TARGET_CONTEXT= target/test
C_UTILS_TEST_TARGET_CONTEXT= $(C_TEST_TARGET_CONTEXT)/utils

# Test

# Utils

# BUFFER_TEST_OUT= $(C_UTILS_TEST_TARGET_CONTEXT)/buffer_test
# BUFFER_TEST_DEPS= $(C_UTILS_TEST_SOURCE_CONTEXT)/buffer_test.c

# NETUTILS_TEST_OUT= $(C_UTILS_TEST_TARGET_CONTEXT)/netutils_test
# NETUTILS_TEST_DEPS= $(C_UTILS_TEST_SOURCE_CONTEXT)/netutils_test.c $(C_UTILS_SOURCE_CONTEXT)/netutils.c $(C_UTILS_SOURCE_CONTEXT)/buffer.c

# TEST_OUT= $(BUFFER_TEST_OUT) $(PARSER_TEST_OUT)

# Outputs

SS_OUT = ss
SS_DEPS = $(C_UTILS_SOURCE_CONTEXT)/bmp_handler.c $(C_SOURCE_CONTEXT)/main.c 	

OUT= $(SS_OUT)

all: $(SS_OUT)

$(SS_OUT): $(SS_DEPS)
	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CFLAGS)

test: $(TEST_OUT)

# TODO: Gotta fix this
# tests_check: $(TEST_OUT)
# 	./$(BUFFER_TEST_OUT)
# 	./$(PARSER_TEST_OUT)

# $(BUFFER_TEST_OUT): $(BUFFER_TEST_DEPS)
# 	@mkdir -p $(C_UTILS_TEST_TARGET_CONTEXT)	
# 	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CTESTFLAGS)
	
# $(PARSER_TEST_OUT): $(PARSER_TEST_DEPS)
# 	@mkdir -p $(C_UTILS_TEST_TARGET_CONTEXT)	
# 	$(CC) -o $@ $^ -I $(C_SOURCE_CONTEXT)/libs $(CTESTFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all	clean
