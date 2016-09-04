# labrat

A simple, single-file test harness for C/C++. Allows specifying test cases and
benchmarks alongside your code, and running your tests through a simple flag
to your main application. Here's an example:

```c
// inside calculator.c:

...

TEST_CASE(test_calculates)
{
    const char* calc_str = "15 5 +";
    int result = calculate(calc_str);

    ASSERT_EQ(result, 20, "%d");  // actual, expected, format
}
```

```c
// inside main.c

#define LR_IMPLEMENTATION
#include "labrat.h"

int main(int argc, char const *argv[])
{
    LR_PRELUDE(argc, argv);
    
    ... // do what we would do if tests didn't run.
}
```

### Using GCC:
```sh
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

![sample output](https://github.com/SquareWave/labrat/blob/master/demo/demo.png?raw=true)



### Using MSVC (command line):
```bat
:: run the setup provided by Visual Studio to get the command line working
"C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

:: generate the labrat executable
cl /TC /DLR_GEN_EXECUTABLE labrat.h

:: run labrat to generate test metadata
labrat.exe

:: compile our program
cl program.c calculator.c

:: run the tests
program.exe --lr-run-tests

:: run benchmarks
program.exe --lr-run-benchmarks 1000

```

![sample output on Windows](https://github.com/SquareWave/labrat/blob/master/demo/demo_win32.png?raw=true)


## How it Works

It works by recursively grabbing every source and header file in the
directory, tokenizing them, and searching for the `TEST_CASE` and
`BENCHMARK` tokens. It then builds a `labrat_data.c` file which includes
all of the necessary information for running the tests. Then when
you want to run your tests, it just includes this file in such a way
that it generates code which handles running the tests.
