/*
===============================================================================
File Name   : utils.h
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This header file contains all the declarations for the utility
              functions, their declarations, macros and enums.
-------------------------------------------------------------------------------
Notes       : This uses extern "C" linkage to allow the C++ functions to be
              called from C code.
===============================================================================
*/

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

  enum ErrorLevel
  {
    MINOR,    // 0
    MODERATE, // 1
    CRITICAL  // 2
  };
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

  int UTILS_ERROR_LOGGER(char *error_message, char *function, enum ErrorLevel level);

  void UTILS_REMOVE_NEWLINE_CHAR(char *param);
  void UTILS_CLEAR_INPUT_BUFFER();
  void show_current_menu(char *str);
  void show_current_step(char *str, int currentStep, int totalSteps);

  typedef struct
  {
    char FirstName[30];
    char LastName[30];
    /*
    Made the StudentID a string instead
     of an integer to allow stuff
    like 'CoolName1234', or CName1234
    */
    char StudentID[20];
    // Program CurrentProgram;

  } Student;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UTILS_H */