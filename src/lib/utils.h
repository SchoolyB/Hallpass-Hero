
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Conditional compilation for Windows and Linux
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START MACROS+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
// Booleans
#define FALSE 0
#define TRUE 1
//--------------------------------------------------------------------------------//

// Colors
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define GREEN "\x1B[32m"
#define PURPLE "\x1B[35m"
#define YELLOW "\x1B[33m"
// fonts
#define BOLD "\x1B[1m"
#define UNDERLINE "\x1B[4m"

//--------------------------------------------------------------------------------//

// Handling user confirmation for yes and no
#define INPUT_IS_YES(param) (strcmp(param, "y") == 0 || strcmp(param, "Y") == 0 || \
                             strcmp(param, "yes") == 0 || strcmp(param, "Yes") == 0)

#define INPUT_IS_NO(param) (strcmp(param, "n") == 0 || strcmp(param, "N") == 0 || \
                            strcmp(param, "no") == 0 || strcmp(param, "No") == 0)
//--------------------------------------------------------------------------------//

// Standard fgets() and removing the newline character at the end of the user's string input
#define FGETS(param) (fgets(param, sizeof(param), stdin))

  //--------------------------------------------------------------------------------//

  // Declaration of utility functions

  int UTILS_ERROR_LOGGER(char *error_message, char *func, char *action);
  void UTILS_REMOVE_NEWLINE_CHAR(char *param);

#ifdef __cplusplus
} // extern "C"
#endif
#endif /* UTILS_H */