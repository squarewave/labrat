# labrat

A simple, single-file test harness for C/C++. Allows specifying test cases and
benchmarks alongside your code, and running your tests through a simple flag
to your main application. Here's an example:

```
// inside calculator.c:

...

TEST_CASE(test_calculates)
{
    const char* calc_str = "15 5 +";
    int result = calculate(calc_str);

    ASSERT_EQ(20, result, "%d");  expected, actual, format
}
```

```
// inside main.c

#define LR_IMPLEMENTATION
#include "labrat.h"

int main(int argc, char const *argv[])
{
    LR_PRELUDE(argc, argv);
    
    ... // do what we would do if tests didn't run.
}
```

### Using clang:
```
# generate the labrat executable
gcc -x c labrat.h -D LR_GEN_EXECUTABLE -o labrat

# run labrat to generate test metadata
./labrat

# compile our program
gcc program.c calculator.c -o program

# run the tests
./program --lr-run-tests

# run benchmarks
./program --lr-run-benchmarks 1000

```