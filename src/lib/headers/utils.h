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

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define RESET "\x1B[0m"
  typedef struct
  {
    const char *colorCode;
  } Color;

  extern Color red;
  extern Color green;
  extern Color yellow;
  extern Color reset;

  // Error messages
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
// Handling user confirmation for yes and no and cancel
#define INPUT_IS_YES(param) (strcmp(param, "y") == 0 || strcmp(param, "Y") == 0 || \
                             strcmp(param, "yes") == 0 || strcmp(param, "Yes") == 0)

#define INPUT_IS_NO(param) (strcmp(param, "n") == 0 || strcmp(param, "N") == 0 || \
                            strcmp(param, "no") == 0 || strcmp(param, "No") == 0)

#define INPUT_IS_CANCEL(param) (strcmp(param, "c") == 0 || strcmp(param, "C") == 0 || \
                                strcmp(param, "cancel") == 0 || strcmp(param, "Cancel") == 0)
  //--------------------------------------------------------------------------------//

#define __utils_fgets_and_remove_newline(param) (fgets(param, sizeof(param), stdin), __utils_remove_newline_char(param))
  //--------------------------------------------------------------------------------//

  // Declaration of utility functions
  int __utils_error_logger(char *error_message, char *function, enum ErrorLevel level);
  int __utils_runtime_logger(char *action, char *functionName);
  void __utils_operation_cancelled(const char *functionName);
  int __utils_check_for_sqlite_db(void);
  void show_current_menu(char *str);
  void show_current_step(char *str, int currentStep, int totalSteps);
  void __utils_remove_newline_char(char *param);
  void __utils_clear_input_buffer();
  int list_all_students(void);
  int has_one_non_space_char(const char *str);
  int wait_for_char_input(void);
  int search_for_student(void);
  int read_and_display_help_docs(const char *helpFile);

  // just some variables used when Idk what do name a variable or when I need to pass a variable to a function but I don't need the value
  typedef struct
  {
    char throwAwayStr[32];
    int throwAwayInt;
  } ThrowAways;

  // this is only used to set the passed in tableName of the check_if_table_exists() function
  typedef struct
  {
    char TableName[50];
  } DesiredTableName;
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
    int isAddingToStudentsTable; // modifies functions depending on if the user is adding students directly to the "students" table or to a roster
  } GlobalTrigger;

  typedef struct
  {
    const char *dbPath;
    char newDBPath[50];
    char currentDBName[20];
    char newDBName[20];
  } DatabaseInfo;

  typedef struct
  {
    char settingKeyName[50];
    int settingKeyValue;
    int autoStudentIDGenerationEnabled;
    int colorEnabled;
    int runtimeLoggingEnabled;
    DatabaseInfo databaseInfo; // may not need this
  } ProgramSettings;

  extern Roster roster;                   // initialized in main.c
  extern ThrowAways throwAways;           // initialized in main.c
  extern UserInput userInput;             // initialized in main.c
  extern DatabaseInfo databaseInfo;       // initialized in main.c
  extern ProgramSettings programSettings; // initialized in main.c
  extern GlobalTrigger globalTrigger;     // initialized in main.c
#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UTILS_H */
