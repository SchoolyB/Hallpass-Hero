/****************************************************************
 * Author: Marshall Burns
 * Date: 11/24/2023
 * Description: The CUAZAR test library is a simple library that allows you
 * to test your C code.
 *
 * Currently, the library supports the following:
 * - Basic Logic & Arithmetic
 * - String Comparisons
 * - Functions return values
 * - Function Execution Time
 *
 *
 ***************************************************************/
#ifndef CUAZAR_H
#define CUAZAR_H

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef RED
#undef RED
#endif
#define RED "\x1b[31m"

#ifdef GREEN
#undef GREEN
#endif
#define GREEN "\x1b[32m"

#ifdef YELLOW
#undef YELLOW
#endif
#define YELLOW "\x1b[33m"

#ifdef BLUE
#undef BLUE
#endif
#define BLUE "\x1b[34m"

#ifdef RESET
#undef RESET
#endif
#define RESET "\x1b[0m"

#ifdef BOLD
#undef BOLD
#endif
#define BOLD "\x1b[1m"

#ifdef UNDERLINE
#undef UNDERLINE
#endif
#define UNDERLINE "\x1b[4m"

#ifdef RESET
#undef RESET
#endif
#define RESET "\x1b[0m"

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

extern int testModeIsOn;

int Init_Cuazar(void);
int Kill_Cuazar(void);

/************************************************************************************
 * Module(s): Basic Logic & Arithmetic
 * Macro: __CUAZAR_EQUALS__
 * Description: Used to compare the expected result with the actual result
 * -----------------------------------------------------------------------------------
 * Example Usage:
 * int a = 1;
 * int b = 2;
 * __CUAZAR_EQUALS__(1, 4, a + b, return 1);
 *
 * Example Output: "TEST #1 FAILED. Expected 4 but got 3 as the result."
 * -----------------------------------------------------------------------------------
 * Author: Marshall Burns
 * Date: 11/24/2023
 ************************************************************************************/
#define __CUAZAR_EQUALS__(testNum, expected, result)                                                                                                                              \
  do                                                                                                                                                                              \
  {                                                                                                                                                                               \
    if ((testModeIsOn == TRUE))                                                                                                                                                   \
    {                                                                                                                                                                             \
      do                                                                                                                                                                          \
      {                                                                                                                                                                           \
        if ((expected) != (result))                                                                                                                                               \
        {                                                                                                                                                                         \
          printf(RED "TEST " BOLD "#%d " RESET RED "FAILED. Expected " BOLD "%d" RESET RED " but got " BOLD "%d" RESET RED " as the result.\n" RESET, testNum, expected, result); \
        }                                                                                                                                                                         \
        else                                                                                                                                                                      \
        {                                                                                                                                                                         \
          printf(GREEN "TEST " BOLD "#%d " RESET GREEN "PASSED.\n" RESET, testNum);                                                                                               \
        }                                                                                                                                                                         \
      } while (0);                                                                                                                                                                \
    }                                                                                                                                                                             \
  } while (0)

/************************************************************************************
 * Module(s): Basic Logic & Arithmetic / Strings
 * Macro: __CUAZAR_EQUALS_STR__
 * Description: Used to compare the expected string result with the actual string result
 * -----------------------------------------------------------------------------------
 * Example Usage:
 * char input[10] = "Jello";
 * __CUAZAR_EQUALS_STR__(TRUE, 1, "Hello", input);
 *
 * Example Output: "TEST #1 FAILED. Expected "Hello" but got "Jello" as the result."
 * -----------------------------------------------------------------------------------
 * Author: Marshall Burns
 * Date: 11/24/2023
 ************************************************************************************/
#define __CUAZAR_EQUALS_STR__(testNum, expectedStr, resultStr)                                                                                                                          \
  do                                                                                                                                                                                    \
  {                                                                                                                                                                                     \
    if ((testModeIsOn == TRUE))                                                                                                                                                         \
    {                                                                                                                                                                                   \
      do                                                                                                                                                                                \
      {                                                                                                                                                                                 \
        if (strcmp((expectedStr), (resultStr)) != 0)                                                                                                                                    \
        {                                                                                                                                                                               \
          printf(RED "TEST " BOLD "#%d " RESET RED "FAILED. Expected " BOLD "%s" RESET RED " but got " BOLD "%s" RESET RED " as the result.\n" RESET, testNum, expectedStr, resultStr); \
        }                                                                                                                                                                               \
        else                                                                                                                                                                            \
        {                                                                                                                                                                               \
          printf(GREEN "TEST " BOLD "#%d " RESET GREEN "PASSED.\n" RESET, testNum);                                                                                                     \
        }                                                                                                                                                                               \
      } while (0);                                                                                                                                                                      \
    }                                                                                                                                                                                   \
  } while (0)

/************************************************************************************
 * Module(s): Return Values
 * Macro: __CUAZAR_RETURN__
 * Description: Used to compare the expected return value with the actual return value
 * !NOTE:  Before using this macro there  are a few things to know:
 * ! 1.The macro CAN NOT be used inside the function that is being tested.
 * ! 2.The function that is being tested MUST be called inside the macro.
 * ! 3.The function that is being tested MUST return a value.
 * -----------------------------------------------------------------------------------
 * Example Usage:
 * __CUAZAR_RETURN__(TRUE, 1, 0, new_func()); //This would fail
 *
 * Example Output: "TEST #1 FAILED. Expected a return value of 1 but received a return value of 0."
 * -----------------------------------------------------------------------------------
 * Author: Marshall Burns
 * Date: 11/24/2023
 * **********************************************************************************/
#define __CUAZAR_RETURN__(testNum, expected, func)                                                                                                                                                         \
  do                                                                                                                                                                                                       \
  {                                                                                                                                                                                                        \
    if ((testModeIsOn == TRUE))                                                                                                                                                                            \
    {                                                                                                                                                                                                      \
      do                                                                                                                                                                                                   \
      {                                                                                                                                                                                                    \
        if ((expected) != (func))                                                                                                                                                                          \
        {                                                                                                                                                                                                  \
          printf(RED "TEST " BOLD "#%d " RESET RED "FAILED. Expected a return value of " BOLD "%d" RESET RED " but received a return value of " BOLD "%d" RESET RED ".\n" RESET, testNum, expected, func); \
        }                                                                                                                                                                                                  \
        else                                                                                                                                                                                               \
        {                                                                                                                                                                                                  \
          printf(GREEN "TEST " BOLD "#%d " RESET GREEN "PASSED.\n" RESET, testNum);                                                                                                                        \
        }                                                                                                                                                                                                  \
      } while (0);                                                                                                                                                                                         \
    }                                                                                                                                                                                                      \
  } while (0)

/************************************************************************************
 * Module: Basic Logic & Arithmetic
 * Macro: __CUAZAR_BOOL__
 * Description: Used to compare the expected boolean result with the actual boolean result
 * -----------------------------------------------------------------------------------
 * Example Usage:
 * __CUAZAR_BOOL__(TRUE, 1, 1, 1);
 *
 * Potential Output: "TEST #1 PASSED."
 * -----------------------------------------------------------------------------------
 * Author: Marshall Burns
 * Date: 11/24/2023
 ************************************************************************************/
#define __CUAZAR_BOOL__(testNum, expectedBool, resultBool)                                                                                                     \
  do                                                                                                                                                           \
  {                                                                                                                                                            \
    if ((testModeIsOn == TRUE))                                                                                                                                \
    {                                                                                                                                                          \
      do                                                                                                                                                       \
      {                                                                                                                                                        \
        if ((expectedBool) != (resultBool))                                                                                                                    \
        {                                                                                                                                                      \
          printf(RED "TEST " BOLD "#%d " RESET RED "FAILED. Expected " BOLD "%s" RESET RED " but got " BOLD "%s" RESET RED " as the result.\n" RESET, testNum, \
                 (expectedBool == TRUE) ? "TRUE" : "FALSE",                                                                                                    \
                 (resultBool == TRUE) ? "TRUE" : "FALSE");                                                                                                     \
        }                                                                                                                                                      \
        else                                                                                                                                                   \
        {                                                                                                                                                      \
          printf(GREEN "TEST " BOLD "#%d " RESET GREEN "PASSED.\n" RESET, testNum);                                                                            \
        }                                                                                                                                                      \
      } while (0);                                                                                                                                             \
    }                                                                                                                                                          \
  } while (0)

/************************************************************************************
 * Module: Execution Time
 * Macro: __CUAZAR_EXEC_TIME__
 * Description: Used to test how long it takes for a function to fully execute.
 * !NOTE:  Before using this macro there are a couple things to note:
 * ! 1. The macro CAN NOT be used inside the function that is being tested.
 * ! 2. This macro only works on functions that do not take in any kind of user input
 * ! 3. The function that is being tested MUST be called inside the macro
 * ! 4. Uses the C standard library <time.h>
 *------------------------------------------------------------------------------------
 * Example Usage:
 * __CUAZAR_EXEC_TIME__(1, myCoolFunc());
 *
 * Example Output: "TEST #1 PASSED. Execution time: 0.000020 seconds."
 *------------------------------------------------------------------------------------
 * Author: Marshall Burns
 * Date: 11/25/2023
 ************************************************************************************/

#define __CUAZAR_EXEC_TIME__(testNum, func)                                                                                                      \
  do                                                                                                                                             \
  {                                                                                                                                              \
    if ((testModeIsOn == TRUE))                                                                                                                  \
    {                                                                                                                                            \
      do                                                                                                                                         \
      {                                                                                                                                          \
        clock_t start = clock();                                                                                                                 \
        func;                                                                                                                                    \
        clock_t end = clock();                                                                                                                   \
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;                                                                              \
        printf(GREEN "TEST " BOLD "#%d " RESET GREEN "PASSED. Execution time: " BOLD "%f" RESET GREEN " seconds.\n" RESET, testNum, time_spent); \
      } while (0);                                                                                                                               \
    }                                                                                                                                            \
  } while (0)

#endif
