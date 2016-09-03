//  labrat.h v0.1 - public domain (see license at bottom of file)
//
//  authored 2016 by Doug Thayer
//
//  This library allows decentralized specification of test routines, without
//  forcing you to use build system X. Just build this file with the flag
//  LR_GEN_EXECUTABLE set to generate an executable. This executable will
//  find tests in the current directory and build a list of them that it can
//  consume.
//
//      Example:
//
//------// inside calculator.h:
//
//      ...
//
//      TEST_CASE(test_calculates)
//      {
//          const char* calc_str = "15 5 +";
//          int result = calculate(calc_str);
//
//          ASSERT_EQ(20, result, "%d"); // expected, actual, format
//      }
//
//------// in the file that defines main()
//
//      #define LR_IMPLEMENTATION
//      #include "labrat.h"
//
//      int main(int argc, char const *argv[])
//      {
//          ... // determine if we want to run tests
//
//          if (run_tests) {
//              lr_run_tests();
//              return 0;
//          }
//      }
//------
//
//  Note:
//      - the ASSERT functions do not guarantee single execution, so avoid
//        passing in parameters with side-effects.
//
//  Todo:
//      - allow arguments to filter the files searched by the test-finder
//      - support platforms other than Windows
//

/******************************************************************************/
///////////////////////////////// BEGIN HEADER /////////////////////////////////
/******************************************************************************/

#ifndef LABRAT_H
#define LABRAT_H

#pragma warning(push, 0)
#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#pragma warning(pop)

// This will forward declare all of our test and benchmark functions so they
// may be referenced here.
#if !defined(LR_GEN_EXECUTABLE) || defined(LR_SELF_TEST) ///////////////////////
#define TEST_DEFINITION(id) void id(void);
#define BENCH_DEFINITION(id) void id(int64_t iterations);
#include "labrat_data.c"
#undef TEST_DEFINITION
#undef BENCH_DEFINITION
#endif
////////////////////////////////////////////////////////////////////////////////

#include "windows.h"
HANDLE _lr_console_h = 0;

#define LR_SET_COLOR_GRN() SetConsoleTextAttribute(_lr_console_h, 2) 
#define LR_SET_COLOR_RED() SetConsoleTextAttribute(_lr_console_h, 4) 
#define LR_SET_COLOR_DEF() SetConsoleTextAttribute(_lr_console_h, 7) 
#define LR_SET_COLOR_YEL() SetConsoleTextAttribute(_lr_console_h, 6) 
#define LR_SET_COLOR_WHT() SetConsoleTextAttribute(_lr_console_h, 15) 

static bool __lr_test_passed;
static int64_t __lr_benchmark_time;
static int64_t __lr_benchmark_start;
static int64_t __lr_benchmark_end;

#define _LR_GETCYCLES() __rdtsc()
#define _LR_ARRAY_COUNT(array) sizeof(array) / sizeof(array[0])
#define _LR_DIR(file) (strrchr((file), '\\') ? \
                       strrchr((file), '\\') + 1 : (strrchr((file), '/') ? \
                                                    strrchr((file), \
                                                            '/') + 1 : (file)))
#define _LR_FILENAME _LR_DIR(__FILE__)


#define TEST_CASE(__lr_test_id__) void __lr_test_id__(void)
#define BENCHMARK(__lr_bench_id__, __lr_iterations__) \
    void __lr_bench_id__(int64_t __lr_iterations__)
#define BEGIN_BENCHMARK() __lr_benchmark_start = _LR_GETCYCLES()
#define END_BENCHMARK() __lr_benchmark_end = _LR_GETCYCLES()

#define ASSERT_TRUE(exp) do { \
    if (!(exp)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected (%s) to be true -- %s, line %d\n", \
               #exp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_FALSE(exp) do { \
    if (exp) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected (%s) to be false -- %s, line %d\n", \
               #exp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_EQ(actual, expected, format) do { \
    if ((expected) != (actual)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected "format " to equal "format " -- %s, line %d\n", \
               (actual), (expected), _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_NOT_EQ(actual, comp, format) do { \
    if ((comp) == (actual)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected "format " to not equal "format " -- %s, line %d\n", \
               actual, comp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_GT(actual, comp, format) do { \
    if ((comp) <= (actual)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected "format " to be greater than "format " -- %s, line %d\n", \
               actual, comp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_LT(actual, comp, format) do { \
    if ((comp) >= (actual)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected "format " to be less than "format " -- %s, line %d\n", \
               actual, comp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_GT_OR_EQ(actual, comp, format) do { \
    if ((comp) < (actual)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected "format " to be greater than or equal to "format " -- %s, " \
               "line %d\n", \
               actual, comp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define ASSERT_LT_OR_EQ(actual, comp, format) do { \
    if ((comp) > (actual)) { \
        LR_SET_COLOR_YEL(); \
        printf("Assertion Failed:\n" \
               "Expected "format " to be less than or equal to " \
               format " -- %s, line %d\n", \
               actual, comp, _LR_FILENAME, __LINE__); \
        __lr_test_passed = false; \
        LR_SET_COLOR_DEF(); \
        return; \
    } \
} while (0)

#define LR_PRELUDE(argc, argv) do {\
    if (argc == 2 && strcmp("--lr-run-tests", argv[1]) == 0) {\
        _lr_console_h = GetStdHandle(STD_OUTPUT_HANDLE);\
        lr_run_tests();\
    } else if (argc == 3 && strcmp("--lr-run-benchmarks", argv[1]) == 0) {\
        _lr_console_h = GetStdHandle(STD_OUTPUT_HANDLE);\
        int64_t _lr_iterations = atoi(argv[2]);\
        lr_run_benchmarks(_lr_iterations);\
    } } while(0)

void lr_run_tests(void);

#endif //#ifndef LABRAT_H

/******************************************************************************/
////////////////////////////////// END HEADER //////////////////////////////////
/******************************************************************************/

#ifdef LR_GEN_EXECUTABLE
#define LR_IMPLEMENTATION
#endif //#ifdef LR_GEN_EXECUTABLE

/******************************************************************************/
///////////////////////////// BEGIN IMPLEMENTATION /////////////////////////////
/******************************************************************************/

#ifdef LR_IMPLEMENTATION

/******************************************************************************/
////////////////////// Sean Barrett's Stretchy Buffer///////////////////////////
//////// https://github.com/nothings/stb/blob/master/stretchy_buffer.h /////////
/******************************************************************************/
#define lr_sb_free(a)   ((a) ? free(lr__sbraw(a)), 0 : 0)
#define lr_sb_push(a, v) (lr__sbmaybegrow(a, 1), (a)[lr__sbn(a)++] = (v))
#define lr_sb_count(a)  ((a) ? lr__sbn(a) : 0)
#define lr_sb_add(a, n)  (lr__sbmaybegrow(a, n), \
                          lr__sbn(a) += (n), \
                          & (a)[lr__sbn(a) - (n)])
#define lr_sb_last(a)   ((a)[lr__sbn(a) - 1])

#define lr__sbraw(a) ((int64_t *)(a) - 2)
#define lr__sbm(a)   lr__sbraw(a)[0]
#define lr__sbn(a)   lr__sbraw(a)[1]

#define lr__sbneedgrow(a, n)  ((a) == 0 || lr__sbn(a) + (n) >= lr__sbm(a))
#define lr__sbmaybegrow(a, n) (lr__sbneedgrow(a, (n)) ? lr__sbgrow(a, n) : 0)
#define lr__sbgrow(a, \
                   n)      (*((void **)&a) = \
                                lr__sbgrowf(((void *)a), (n), sizeof(*(a))))

static void *lr__sbgrowf(void *arr, int64_t increment, int64_t itemsize)
{
    int64_t dbl_cur = arr ? 2 * lr__sbm(arr) : 0;
    int64_t min_needed = lr_sb_count(arr) + increment;
    int64_t m = dbl_cur > min_needed ? dbl_cur : min_needed;
    int64_t *p = (int64_t *)realloc(arr ?
                                    lr__sbraw(arr) :
                                    0, itemsize * m + sizeof(int64_t) * 2);

    if (p) {
        if (!arr)
            p[1] = 0;
        p[0] = m;
        return p + 2;
    } else {
#ifdef STRETCHY_BUFFER_OUT_OF_MEMORY
        STRETCHY_BUFFER_OUT_OF_MEMORY;
#endif
        return (void *)(2 * sizeof(int64_t));
    }
}
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************/

#include "windows.h"

#if !defined(LR_GEN_EXECUTABLE) || defined(LR_SELF_TEST)
bool __lr_test_definition(void (*func)(void), const char *name)
{
    __lr_test_passed = true;
    func();
    if (__lr_test_passed) {
        LR_SET_COLOR_GRN();
        printf("\t[ PASSED ] -- %s\n", name);
        LR_SET_COLOR_DEF();
        return true;
    } else {
        LR_SET_COLOR_RED();
        printf("\t[ FAILED ] -- %s\n", name);
        LR_SET_COLOR_DEF();
        return false;
    }
}

void lr_run_tests(void)
{
#ifndef LR_OFF // just produce an empty function if LR_OFF
    LR_SET_COLOR_WHT();
    printf("\nRunning tests:\n\n");
    LR_SET_COLOR_DEF();

    bool tests[] = {
        0,
#define TEST_DEFINITION(id) 0,
#include "labrat_data.c"
#undef TEST_DEFINITION
    };
    int32_t ix = 1;

#define TEST_DEFINITION(id) tests[ix++] = __lr_test_definition(id, #id);
#include "labrat_data.c"
#undef TEST_DEFINITION

    int32_t passed = 0;
    int32_t total = _LR_ARRAY_COUNT(tests) - 1;
    for (int64_t i = 1; i <= total; i++)
        if (tests[i])
            passed++;

    bool all_passed = passed == total;
    puts("\nFinished running tests: ");
    if (all_passed) {
        LR_SET_COLOR_GRN();
    } else {
        LR_SET_COLOR_RED();
    }
    printf("%d ", passed);
    LR_SET_COLOR_WHT();

    printf("of %d tests passed (", total);

    if (all_passed) {
        LR_SET_COLOR_GRN();
    } else {
        LR_SET_COLOR_RED();
    }
    printf("%d", total - passed);
    LR_SET_COLOR_WHT();

    puts(" failed)\n");
    LR_SET_COLOR_DEF();
#endif // #ifndef LR_OFF
}

void lr_run_benchmarks(int64_t iterations)
{
#ifndef LR_OFF // just produce an empty function if LR_OFF
    int32_t tests_passed = 0;
    int32_t total_tests = 0;
    LR_SET_COLOR_WHT();
    printf("\nRunning benchmarks:\n\n");
    LR_SET_COLOR_DEF();

    uint64_t start_time;
    uint64_t end_time;

    int32_t max_bench_name_size = -1;
    int32_t bench_name_sizes[] = {
        -1,
#define BENCH_DEFINITION(id) (sizeof(#id) - 1),
#include "labrat_data.c"
#undef BENCH_DEFINITION
    };

    for (int64_t i = 0; i < _LR_ARRAY_COUNT(bench_name_sizes); i++)
        if (bench_name_sizes[i] > max_bench_name_size)
            max_bench_name_size = bench_name_sizes[i];

#define BENCH_DEFINITION(id) \
    __lr_benchmark_start = -1; \
    __lr_benchmark_end = -1; \
    start_time = _LR_GETCYCLES(); \
    id(iterations); \
    end_time = _LR_GETCYCLES(); \
    if (__lr_benchmark_start != -1) { \
        start_time = __lr_benchmark_start; \
    } \
    if (__lr_benchmark_end != -1) { \
        end_time = __lr_benchmark_end; \
    } \
    LR_SET_COLOR_WHT();\
    printf("\t[ FINISHED ] -- " \
           "%-*s: %12lld cycles / iteration\n", \
           max_bench_name_size + 2, \
           #id, \
           (end_time - start_time) / iterations);\
    LR_SET_COLOR_DEF();
#include "labrat_data.c"
#undef BENCH_DEFINITION

    bool all_passed = tests_passed == total_tests;
    LR_SET_COLOR_WHT();
    printf("\nFinished running benchmarks.");
    LR_SET_COLOR_DEF();
#endif // #ifndef LR_OFF
}
#endif // #if !defined(LR_GEN_EXECUTABLE) || defined(LR_SELF_TEST)

enum {
    LR_TOKEN_L_PAREN,
    LR_TOKEN_R_PAREN,
    LR_TOKEN_L_BRACE,
    LR_TOKEN_R_BRACE,
    LR_TOKEN_L_BRACKET,
    LR_TOKEN_R_BRACKET,
    LR_TOKEN_COMMA,
    LR_TOKEN_PERIOD,
    LR_TOKEN_ASTERISK,
    LR_TOKEN_MINUS,
    LR_TOKEN_PLUS,
    LR_TOKEN_EXCLAMATION,
    LR_TOKEN_TILDE,
    LR_TOKEN_BACKSLASH,
    LR_TOKEN_SLASH,
    LR_TOKEN_EQ,
    LR_TOKEN_AMPERSAND,
    LR_TOKEN_POUND,
    LR_TOKEN_SEMICOLON,
    LR_TOKEN_COLON,
    LR_TOKEN_QUESTION_MARK,
    LR_TOKEN_IDENTIFIER,
    LR_TOKEN_NUMBER,
    LR_TOKEN_CHARACTER,
    LR_TOKEN_STRING,
    LR_TOKEN_UNKNOWN,
    LR_TOKEN_EOF,
};

typedef struct {
    int64_t len;
    char *data;
} lr_slice_t;

typedef struct {
    uint32_t type;
    lr_slice_t slice;
} c_token_t;

char *get_token_type_name(uint32_t type)
{
    switch (type) {
    case LR_TOKEN_L_PAREN: { return "LR_TOKEN_L_PAREN"; }
    case LR_TOKEN_R_PAREN: { return "LR_TOKEN_R_PAREN"; }
    case LR_TOKEN_L_BRACE: { return "LR_TOKEN_L_BRACE"; }
    case LR_TOKEN_R_BRACE: { return "LR_TOKEN_R_BRACE"; }
    case LR_TOKEN_L_BRACKET: { return "LR_TOKEN_L_BRACKET"; }
    case LR_TOKEN_R_BRACKET: { return "LR_TOKEN_R_BRACKET"; }
    case LR_TOKEN_ASTERISK: { return "LR_TOKEN_ASTERISK"; }
    case LR_TOKEN_COMMA: { return "LR_TOKEN_COMMA"; }
    case LR_TOKEN_PERIOD: { return "LR_TOKEN_PERIOD"; }
    case LR_TOKEN_MINUS: { return "LR_TOKEN_MINUS"; }
    case LR_TOKEN_PLUS: { return "LR_TOKEN_PLUS"; }
    case LR_TOKEN_EXCLAMATION: { return "LR_TOKEN_EXCLAMATION"; }
    case LR_TOKEN_TILDE: { return "LR_TOKEN_TILDE"; }
    case LR_TOKEN_BACKSLASH: { return "LR_TOKEN_BACKSLASH"; }
    case LR_TOKEN_SLASH: { return "LR_TOKEN_SLASH"; }
    case LR_TOKEN_EQ: { return "LR_TOKEN_EQ"; }
    case LR_TOKEN_AMPERSAND: { return "LR_TOKEN_AMPERSAND"; }
    case LR_TOKEN_POUND: { return "LR_TOKEN_POUND"; }
    case LR_TOKEN_SEMICOLON: { return "LR_TOKEN_SEMICOLON"; }
    case LR_TOKEN_COLON: { return "LR_TOKEN_COLON"; }
    case LR_TOKEN_QUESTION_MARK: { return "LR_TOKEN_QUESTION_MARK"; }
    case LR_TOKEN_IDENTIFIER: { return "LR_TOKEN_IDENTIFIER"; }
    case LR_TOKEN_NUMBER: { return "LR_TOKEN_NUMBER"; }
    case LR_TOKEN_CHARACTER: { return "LR_TOKEN_CHARACTER"; }
    case LR_TOKEN_STRING: { return "LR_TOKEN_STRING"; }
    case LR_TOKEN_UNKNOWN: { return "LR_TOKEN_UNKNOWN"; }
    case LR_TOKEN_EOF: { return "LR_TOKEN_EOF"; }
    default: { return "---"; }
    }
}

inline lr_slice_t lr_as_slice(char *s, int64_t len)
{
    lr_slice_t result;

    result.len = len;
    result.data = s;
    return result;
}

inline lr_slice_t lr_slice(lr_slice_t s, int64_t start, int64_t end)
{
    lr_slice_t result;

    result.len = end - start;
    result.data = s.data + start;
    return result;
}

inline lr_slice_t lr_slice_r(lr_slice_t s, int64_t start)
{
    lr_slice_t result;

    result.len = s.len - start;
    result.data = s.data + start;
    return result;
}

inline lr_slice_t lr_slice_l(lr_slice_t s, int64_t end)
{
    lr_slice_t result;

    result.len = end;
    result.data = s.data;
    return result;
}

inline lr_slice_t lr_copy_slice(lr_slice_t s)
{
    lr_slice_t result;

    result.data = (char *)malloc(s.len);
    result.len = s.len;
    memcpy(result.data, s.data, s.len);
    return result;
}

inline bool lr_slices_equal(lr_slice_t lhs, lr_slice_t rhs)
{
    if (lhs.len != rhs.len)
        return false;
    else
        return memcmp(lhs.data, rhs.data, lhs.len) != 0;
}

#undef LR_IMPLEMENTATION
#endif // #ifdef LR_IMPLEMENTATION

/******************************************************************************/
////////////////////////////// END IMPLEMENTATION //////////////////////////////
/******************************************************************************/


/******************************************************************************/
/////////////////////////////// BEGIN EXECUTABLE ///////////////////////////////
/******************************************************************************/
#ifdef LR_GEN_EXECUTABLE
char **lr_get_directory(char *start_dir, int64_t *file_count)
{
    char **result = 0;

    char *stack[1024] = { 0 };
    int32_t stack_size = 0;

    stack[stack_size++] = start_dir;

    while (stack_size) {
        char *dir = stack[--stack_size];

        char path[2048];
        snprintf(path, _LR_ARRAY_COUNT(path), "%s/*.*", dir);

        WIN32_FIND_DATA fd;
        HANDLE handle = NULL;
        if ((handle = FindFirstFile(path, &fd)) == INVALID_HANDLE_VALUE)
            continue;

        do {
            if (strcmp(fd.cFileName, ".") != 0 &&
                strcmp(fd.cFileName, "..") != 0) {
                snprintf(path, _LR_ARRAY_COUNT(path), "%s/%s", dir,
                         fd.cFileName);

                if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    int64_t path_length = strlen(path) + 1;
                    stack[stack_size] = (char *)malloc(path_length);
                    memcpy(stack[stack_size], path, path_length);
                    stack_size++;
                } else {
                    int64_t path_length = strlen(path) + 1;
                    char *filename = (char *)malloc(path_length);
                    memcpy(filename, path, path_length);

                    lr_sb_push(result, filename);
                }
            }
        } while (FindNextFile(handle, &fd));

        FindClose(handle);

        if (dir != start_dir)
            free(dir);
    }

    *file_count = lr_sb_count(result);
    return result;
}

inline int32_t lr_is_whitespace(char c)
{
    return c == ' ' || (c >= '\t' && c <= '\r');
}

inline lr_slice_t lr_eat_whitespace_and_comments(lr_slice_t s)
{
    char *data = s.data;
    int64_t len = s.len;

    while (len > 0) {
        if (lr_is_whitespace(*data)) {
            ++data;
            --len;
        } else if (len > 1 && data[0] == '/' && data[1] == '/') {
            data += 2;
            len -= 2;
            while (len > 0 && *data != '\n') {
                ++data;
                --len;
            }
        } else if (len > 1 && data[0] == '/' && data[1] == '*') {
            data += 2;
            len -= 2;
            while (len > 0 && !(data[0] == '*' && len > 1 && data[1] == '/')) {
                ++data;
                --len;
            }
            if (len > 2) {
                data += 2;
                len -= 2;
            } else {
                data += len;
                len = 0;
            }
        } else {
            break;
        }
    }

    lr_slice_t result;
    result.data = data;
    result.len = len;
    return result;
}

inline int32_t lr_is_digit(char c)
{
    return c >= '0' && c <= '9';
}

inline int32_t lr_is_word_char(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_');
}

inline int32_t lr_is_identifier(char c)
{
    return lr_is_word_char(c) || lr_is_digit(c);
}

inline bool lr_lex_identifier(lr_slice_t *c, c_token_t *token)
{
    if (lr_is_word_char(*c->data)) {
        int32_t len = 1;
        while (c->len - len) {
            if (lr_is_word_char(c->data[len]) || lr_is_digit(c->data[len]))
                ++len;
            else
                break;
        }

        lr_slice_t token_slice = lr_slice_l(*c, len);
        *c = lr_slice_r(*c, len);
        token->type = LR_TOKEN_IDENTIFIER;
        token->slice = token_slice;
        return true;
    } else {
        return false;
    }
}

bool lr_lex_constant(lr_slice_t *c, c_token_t *token)
{
    uint32_t token_type;

    if (lr_is_digit(*c->data)) {
        token_type = LR_TOKEN_NUMBER;
        int32_t len = 1;
        while (c->len - len) {
            char ch = c->data[len];
            if (!lr_is_digit(ch) &&
                ch != '.' &&
                ch != 'x' &&
                ch != 'X')
                break;
            len++;
        }

        lr_slice_t token_slice = lr_slice_l(*c, len);
        *c = lr_slice_r(*c, len);
        token->type = token_type;
        token->slice = token_slice;
        return true;
    } else if (*c->data == '\'') {
        token_type = LR_TOKEN_CHARACTER;
        int32_t len = 1;
        while (c->len - len - 1) {
            if (c->data[len] == '\\') {
                if (c->data[len + 1] == 'x') {
                    len = min(len + 4, (int32_t)c->len + 1);
                    continue;
                } else {
                    len = min(len + 2, (int32_t)c->len + 1);
                    continue;
                }
            }
            if (c->data[len] == '\'')
                break;
            len++;
        }

        lr_slice_t token_slice = lr_slice_l(*c, len + 1);
        *c = lr_slice_r(*c, len + 1);
        token->type = token_type;
        token->slice = token_slice;
        return true;
    } else {
        return false;
    }
}

bool lr_lex_string(lr_slice_t *c, c_token_t *token)
{
    if (*c->data == '"') {
        int32_t len = 1;
        while (c->len - len - 1) {
            if (c->data[len] == '\\') {
                if (c->data[len + 1] == 'x') {
                    len = min(len + 4, (int32_t)c->len + 1);
                    continue;
                } else {
                    len = min(len + 2, (int32_t)c->len + 1);
                    continue;
                }
            }
            if (c->data[len] == '"')
                break;
            len++;
        }

        lr_slice_t token_slice = lr_slice_l(*c, len + 1);
        *c = lr_slice_r(*c, len + 1);
        token->type = LR_TOKEN_STRING;
        token->slice = token_slice;
        return true;
    } else {
        return false;
    }
}

bool lr_lex_single_char_token(lr_slice_t *c, c_token_t *token)
{
    uint32_t token_type;
    lr_slice_t token_slice;

    switch (*c->data) {
    case '(': { token_type = LR_TOKEN_L_PAREN; break; }
    case ')': { token_type = LR_TOKEN_R_PAREN; break; }
    case '{': { token_type = LR_TOKEN_L_BRACE; break; }
    case '}': { token_type = LR_TOKEN_R_BRACE; break; }
    case '[': { token_type = LR_TOKEN_L_BRACKET; break; }
    case ']': { token_type = LR_TOKEN_R_BRACKET; break; }
    case '*': { token_type = LR_TOKEN_ASTERISK; break; }
    case ',': { token_type = LR_TOKEN_COMMA; break; }
    case '.': { token_type = LR_TOKEN_PERIOD; break; }
    case '-': { token_type = LR_TOKEN_MINUS; break; }
    case '+': { token_type = LR_TOKEN_PLUS; break; }
    case '!': { token_type = LR_TOKEN_EXCLAMATION; break; }
    case '~': { token_type = LR_TOKEN_TILDE; break; }
    case '\\': { token_type = LR_TOKEN_BACKSLASH; break; }
    case '/': { token_type = LR_TOKEN_SLASH; break; }
    case '=': { token_type = LR_TOKEN_EQ; break; }
    case '&': { token_type = LR_TOKEN_AMPERSAND; break; }
    case '#': { token_type = LR_TOKEN_POUND; break; }
    case ';': { token_type = LR_TOKEN_SEMICOLON; break; }
    case ':': { token_type = LR_TOKEN_COLON; break; }
    case '?': { token_type = LR_TOKEN_QUESTION_MARK; break; }
    default: { token_type = LR_TOKEN_UNKNOWN; break; }
    }

    if (token_type != LR_TOKEN_UNKNOWN) {
        token_slice = lr_slice_l(*c, 1);
        *c = lr_slice_r(*c, 1);
        token->type = token_type;
        token->slice = token_slice;
        return true;
    } else {
        return false;
    }
}

c_token_t *lr_lex_file(lr_slice_t s, int32_t *count)
{
    int32_t capacity = 64;
    c_token_t *result = (c_token_t *)malloc(capacity * sizeof(c_token_t));
    lr_slice_t c = s;

    int32_t i = 0;

    while (true) {
        if (i >= capacity) {
            capacity += capacity / 2;
            result = (c_token_t *)realloc(result, capacity * sizeof(c_token_t));
        }

        c_token_t *t = &result[i];

        if (!c.len) {
            t->type = LR_TOKEN_EOF;
            t->slice = lr_slice_l(c, 0);
            ++i;
            break;
        } else {
            c = lr_eat_whitespace_and_comments(c);

            if (c.len) {
                if (lr_lex_single_char_token(&c, t)) {
                } else if (lr_lex_constant(&c, t)) {
                } else if (lr_lex_string(&c, t)) {
                } else if (lr_lex_identifier(&c, t)) {
                } else {
                    t->type = LR_TOKEN_UNKNOWN;
                    t->slice = lr_slice_l(c, 1);
                    c = lr_slice_r(c, 1);
                }

                assert(c.len >= 0);
                ++i;
            }
        }
    }

    *count = i;
    return result;
}

lr_slice_t lr_read_file(char *f)
{
    FILE *fp = fopen(f, "rb");

    if (!fp)
        return lr_as_slice(0, 0);

    fseek(fp, 0, SEEK_END);
    int64_t len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *data = (char *)malloc(len + 1);
    if (!fread(data, len, 1, fp))
        return lr_as_slice(0, 0);

    // ensure string result is null-terminated
    data[len] = 0;

    fclose(fp);

    return lr_as_slice(data, len);
}

void lr_free_file(lr_slice_t slice)
{
    free(slice.data);
}

bool match_identifier(c_token_t token, char *identifier)
{
    if (token.type != LR_TOKEN_IDENTIFIER)
        return false;

    for (int32_t i = 0; i < token.slice.len; i++)
        if (!identifier[i] || token.slice.data[i] != identifier[i])
            return false;

    return !identifier[token.slice.len];
}

bool match_string(c_token_t token, char *s)
{
    if (token.type != LR_TOKEN_STRING)
        return false;

    for (int32_t i = 1; i < token.slice.len - 1; i++)
        if (!s[i - 1] || token.slice.data[i] != s[i - 1])
            return false;

    return !s[token.slice.len - 2];
}

bool match_labrat_include(c_token_t *ts, int32_t ct, int32_t i)
{
#ifdef LR_SELF_TEST
    return true;
#else
    const int32_t include_len = 3;
    if (i + include_len > ct)
        return false;
    return ts[i].type == LR_TOKEN_POUND &&
           match_identifier(ts[i + 1], "include") &&
           match_string(ts[i + 2], "labrat.h");
#endif
}

bool match_test_case(c_token_t *ts, int32_t ct, int32_t i, c_token_t *id)
{
    const int32_t test_case_len = 4;

    if (i + test_case_len > ct)
        return false;
    bool result = match_identifier(ts[i], "TEST_CASE") &&
                  ts[i + 1].type == LR_TOKEN_L_PAREN &&
                  ts[i + 2].type == LR_TOKEN_IDENTIFIER &&
                  !match_identifier(ts[i + 2], "__lr_test_id__") &&
                  ts[i + 3].type == LR_TOKEN_R_PAREN;

    if (result)
        *id = ts[i + 2];

    return result;
}

bool match_benchmark(c_token_t *ts, int32_t ct, int32_t i, c_token_t *id)
{
    const int32_t test_case_len = 4;

    if (i + test_case_len > ct)
        return false;
    bool result = match_identifier(ts[i], "BENCHMARK") &&
                  ts[i + 1].type == LR_TOKEN_L_PAREN &&
                  ts[i + 2].type == LR_TOKEN_IDENTIFIER &&
                  !match_identifier(ts[i + 2], "__lr_bench_id__") &&
                  ts[i + 3].type == LR_TOKEN_COMMA &&
                  ts[i + 4].type == LR_TOKEN_IDENTIFIER &&
                  ts[i + 5].type == LR_TOKEN_R_PAREN;

    if (result)
        *id = ts[i + 2];

    return result;
}

void lr_write_data_header(lr_slice_t *tests, lr_slice_t *benchmarks)
{
    FILE *fp = fopen("./labrat_data.c", "wb");

    fprintf(fp,
            "#ifndef TEST_DEFINITION\n"
            "#define TEST_DEFINITION(id)\n"
            "#endif\n"
            "#ifndef BENCH_DEFINITION\n"
            "#define BENCH_DEFINITION(id)\n"
            "#endif\n");

    for (int64_t i = 0; i < lr_sb_count(tests); i++) {
        fprintf(fp, "TEST_DEFINITION(%.*s)\n",
                (int32_t)tests[i].len,
                tests[i].data);
    }

    for (int64_t i = 0; i < lr_sb_count(benchmarks); i++) {
        fprintf(fp, "BENCH_DEFINITION(%.*s)\n",
                (int32_t)benchmarks[i].len,
                benchmarks[i].data);
    }

    fprintf(fp,
            "#undef TEST_DEFINITION\n"
            "#undef BENCH_DEFINITION\n");

    fclose(fp);
}

bool should_exclude_file(char *filename)
{
#ifdef LR_SELF_TEST
    if (strcmp(_LR_DIR(filename), _LR_FILENAME) != 0)
        return true;

#else
    if (strcmp(_LR_DIR(filename), _LR_FILENAME) == 0)
        return true;

#endif

    return false;
}

int32_t main(int argc, char const *argv[])
{
    int64_t file_count;
    char **files = lr_get_directory(".", &file_count);
    lr_slice_t *tests = 0;
    lr_slice_t *benchmarks = 0;

    for (int32_t i = 0; i < file_count; i++) {
        if (should_exclude_file(files[i]))
            continue;
        puts(files[i]);

        lr_slice_t filedata = lr_read_file(files[i]);
        if (!filedata.data) {
            printf("LABRAT: Failed to read file: %s\n", files[i]);
            continue;
        }

        int32_t token_count;
        c_token_t *tokens = lr_lex_file(filedata, &token_count);

        bool lr_included = false;
        for (int32_t j = 0; j < token_count; j++) {
            c_token_t token = tokens[j];

            lr_included = lr_included || match_labrat_include(tokens,
                                                              token_count, j);

            if (lr_included) {
                c_token_t identifier;
                if (match_test_case(tokens, token_count, j, &identifier))
                    lr_sb_push(tests, lr_copy_slice(identifier.slice));
                else if (match_benchmark(tokens, token_count, j, &identifier))
                    lr_sb_push(benchmarks, lr_copy_slice(identifier.slice));
            }
        }

        lr_free_file(filedata);
    }

    lr_write_data_header(tests, benchmarks);

#ifdef LR_SELF_TEST
    lr_run_tests();
#endif

    return 0;
}

// Self tests:
// These should all run and fail -- this lets us see the text from the asserts

TEST_CASE(test_finds_tests) {
    ASSERT_TRUE(false);
}

TEST_CASE(test_prints_assert_eq) {
    int32_t actual = 41;
    int32_t expected = 42;

    ASSERT_EQ(actual, expected, "%d");
}

TEST_CASE(test_prints_assert_not_eq) {
    int32_t actual = 42;
    int32_t expected = 42;

    ASSERT_NOT_EQ(actual, expected, "%d");
}

TEST_CASE(test_prints_assert_gt) {
    int32_t actual = 43;
    int32_t compare_to = 42;

    ASSERT_GT(actual, compare_to, "%d");
}

TEST_CASE(test_prints_assert_lt) {
    int32_t actual = 41;
    int32_t compare_to = 42;

    ASSERT_LT(actual, compare_to, "%d");
}

#undef LR_GEN_EXECUTABLE
#endif // #ifdef LR_GEN_EXECUTABLE

/******************************************************************************/
//////////////////////////////// END EXECUTABLE ////////////////////////////////
/******************************************************************************/

// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>