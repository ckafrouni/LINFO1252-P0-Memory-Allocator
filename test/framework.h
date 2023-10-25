#if !defined(TESTING_FW_H)
#define TESTING_FW_H

#include <string.h>
#include <stdio.h>

#define NUM_FMT "\033[34m#%d\033[0m "
#define ARROW "\033[34m->\033[0m "
#define TITLE_FMT "\033[1m'%s'\033[0m "
#define PASSED "\033[0;32mPASSED\033[0m"
#define FAILED "\033[0;31mFAILED\033[0m"

#define MAX_TESTS 30

typedef int (*test_func)(void);
typedef struct
{
    test_func func;
    char title[100];
} test_t;

static test_t tests[MAX_TESTS];

void register_test(char *title, int (*test)())
{
    static int test_count = 0;
    if (test_count >= MAX_TESTS)
    {
        printf("Max tests reached, skipping test: %s\n", title);
        return;
    }

    test_t test_struct = {
        .func = test};

    strncpy(test_struct.title, title, 100);

    tests[test_count++] = test_struct;
}

int run_tests(char *title)
{

    printf("\033[35m================== TESTS ==================\033[0m\n\n");
    printf("\033[1m- %s\n\n\033[0m", title);

    int passed = 0;
    int failed = 0;

    int num = 0;

    for (int i = 0; i < MAX_TESTS; i++)
    {
        test_t test = tests[i];
        if (test.func == NULL)
            break;

        num++;
        printf(NUM_FMT, num);
        printf(ARROW);
        printf(TITLE_FMT, test.title);
        printf(ARROW);

        int result = test.func();
        printf("%s\n", result ? PASSED : FAILED);
        passed = result ? passed + 1 : passed;
    }

    printf("\n\033[35m=================== %d/%d ===================\033[0m\n", passed, num);

    return failed;
}

#endif // TESTING_FW_H