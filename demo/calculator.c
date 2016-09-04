#include <stdio.h>
#include <assert.h>

int calculate(char* str)
{
    int pending = 0;
    char* ptr = str;
    int stack_size = 0;
    int stack[256];
    char c;

    #define CASE_OPERATOR(oper_char,oper) case oper_char: {\
        assert(stack_size > 1);\
        int a = stack[--stack_size];\
        int b = stack[--stack_size];\
        stack[stack_size++] = b oper a;\
    } break;

    while ((c = *ptr)) {
        switch (c) {
            case ' ': {
                stack[stack_size++] = pending;
                pending = 0;
            } break;
            CASE_OPERATOR('+', +)
            CASE_OPERATOR('-', -)
            CASE_OPERATOR('/', /)
            CASE_OPERATOR('*', *)
            default: {
                assert(c >= '0' && c <= '9');
                pending *= 10;
                pending += (int)(c - '0');
            } break;  
        }
        ptr++;
    }

    assert((!pending && stack_size == 1) || (stack_size == 0 && pending));
    return pending ? pending : stack[0];
}

#include "labrat.h"

TEST_CASE(test_adds) {
    char* str = "20 5 +";

    int result = calculate(str);

    ASSERT_EQ(result, 25, "%d");
}

TEST_CASE(test_subtracts) {
    char* str = "20 5 -";

    int result = calculate(str);

    ASSERT_EQ(result, 15, "%d");
}

TEST_CASE(test_multiplies) {
    char* str = "20 5 *";

    int result = calculate(str);

    ASSERT_EQ(result, 100, "%d");
}

TEST_CASE(test_divides) {
    char* str = "20 5 /";

    int result = calculate(str);

    ASSERT_EQ(result, 4, "%d");
}

BENCHMARK(benchmark_add, iterations) {
    for (int i = 0; i < iterations; ++i) {
        char* str = "20 5 +";
        int result = calculate(str);
    }
}