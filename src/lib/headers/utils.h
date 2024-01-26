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
  extern const char *dbPath;

  enum ErrorLevel
  {
    MINOR,    // 0
    MODERATE, // 1
    CRITICAL  // 2
  };
// Handling user confirmation for yes and no and cancel
#define INPUT_IS_YES(param) (strcmp(param, "y") == 0 || strcmp(param, "Y") == 0 || \
                             strcmp(param, "yes") == 0 || strcmp(param, "Yes") == 0)

#define INPUT_IS_NO(param) (strcmp(param, "n") == 0 || strcmp(param, "N") == 0 || \
                            strcmp(param, "no") == 0 || strcmp(param, "No") == 0)

#define INPUT_IS_CANCEL(param) (strcmp(param, "c") == 0 || strcmp(param, "C") == 0 || \
                                strcmp(param, "cancel") == 0 || strcmp(param, "Cancel") == 0)
  //--------------------------------------------------------------------------------//

#define UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(param) (fgets(param, sizeof(param), stdin), __utils_remove_newline_char(param))
  //--------------------------------------------------------------------------------//

  // Declaration of utility functions

  int __utils_error_logger(char *error_message, char *function, enum ErrorLevel level);
  int __utils_runtime_logger(char *action, char *functionName);
  void __utils_remove_newline_char(char *param);
  void __utils_clear_input_buffer();
  void show_current_menu(char *str);
  void show_current_step(char *str, int currentStep, int totalSteps);
  int wait_for_char_input(void);
  int list_all_students(void);
  int search_for_student(void);
  int has_one_non_space_char(const char *str);

  // this type allows the same input through each C source file. As opposed to constantly declaring char arrays
  typedef struct
  {
    int NumInput;
    char StrInput[50];
  } UserInput;
  typedef struct
  {
    char RelationshipToStudent[20];
    char FirstName[30];
    char LastName[30];
    char PhoneNumber[20]; // need to hash this
  } EmergencyContact;

  typedef struct
  {
    char BirthDate[20]; // need to hash this
    char Address[50];   // need to hash this
    EmergencyContact EmergencyContact;
  } PrivateInfo;

  typedef struct
  {
    char FirstName[30];
    char LastName[30];
    char StudentID[20];
    char Email[50];
    PrivateInfo PrivateInfo;
    // Program CurrentProgram;

  } Student;

  typedef struct
  {
    char Str1[100];
    char Str2[100];

  } GenericDataType;

  typedef struct
  {
    char ColumnName[50];
    char ColumnType[20];
    char ColumnSortingMethod[20];

  } Column;

  typedef struct
  {
    char rosterName[50];
    char rosterNameWithPrefix[60];
  } Roster;

  typedef struct
  {
    int studentCreationInterrupted;
    int isTriggered;
  } GlobalTrigger;

  typedef struct
  {
    int runtimeLoggingEnabled;
  } ProgramSettings;

  extern ProgramSettings programSettings; // initialized in main.c
  extern GlobalTrigger globalTrigger;     // initialized in main.c
#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UTILS_H */
