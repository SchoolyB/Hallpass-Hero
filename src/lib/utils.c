/*
===============================================================================
File Name   : utils.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains several utility functions that are used
              throughout the 'C' portion of the source code. This file also
              contains the typedefs for the structs used in the program.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/db.hpp"
/************************************************************************************
 * __utils_error_logger(): Logs errors errors that occur specifically in 'C' files
 * Note: For 'C++' code error logging see cpp_utils_error_logger in utils.cpp
 ************************************************************************************/
int __utils_error_logger(char *errorMessage, char *function, enum ErrorLevel level)
{

  FILE *errorLog = fopen("../logs/errors.log", "a");
  time_t currentTime;
  time(&currentTime);

  switch (level)
  {
  case MINOR:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Minor Error: %s, in function call: %s()\n", errorMessage, function);
    fprintf(errorLog, "======================================================================================\n");
    fflush(errorLog);
    return 0;
    break;
  case MODERATE:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Moderate Error: %s, in function call: %s()\n", errorMessage, function);
    fprintf(errorLog, "======================================================================================\n");
    fflush(errorLog);
    return 1;
  case CRITICAL:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "CRITICAL ERROR: %s, in function call: %s()\n", errorMessage, function);
    fprintf(errorLog, "======================================================================================\n");
    printf(RED "Critical Error Occurred @ %s: For more information see logs/errors.log \n" RESET, ctime(&currentTime));
    fflush(errorLog);
    exit(1);
  default:
    break;
  }
}
/************************************************************************************
 * __utils_runtime_logger(): Logs runtime activity
 *
 ************************************************************************************/
int __utils_runtime_logger(char *action, char *functionName)
{
  if (programSettings.runtimeLoggingEnabled == FALSE)
  {
    return 0;
  }

  FILE *runtimeLog = fopen("../logs/runtime.log", "a");
  time_t currentTime;
  time(&currentTime);

  fprintf(runtimeLog, "Logged @ %s", ctime(&currentTime));

  fprintf(runtimeLog, "User Action: User %s, in function call: %s()\n", action, functionName);

  fprintf(runtimeLog, "======================================================================================\n\n");
  fflush(runtimeLog);
}

void __utils_operation_cancelled(const char *functionName)
{
  system("clear");
  printf("%sCancelling operation.%s\n", yellow.colorCode, reset.colorCode);
  __utils_runtime_logger("Cancelled operation", functionName);
  sleep(1);
  system("clear");
}

/************************************************************************************
 * __utils_check_for_sqlite_db(): Checks if a sqlite database exists in the build
 *                                folder. If it does not then it creates the default
 *                                db.sqlite file.
 *
 * See usage in ALL C source files and the main.c file.
 ************************************************************************************/
int __utils_check_for_sqlite_db(void)
{
  int databaseFound = read_from_dir_and_check_extension("../build", ".sqlite");
  if (databaseFound == FALSE)
  {
    create_student_db_and_table();
  }
  return 0;
}

/************************************************************************************
 * show_current_menu(): Simply shows the current menu the user is in to prevent confusion
 ************************************************************************************/
void show_current_menu(char *str)
{
  printf("Current Menu: " BOLD "%s" RESET "\n", str);
}

/************************************************************************************
 * show_current_step(): When a feature requires multiple steps use this.
 *  Example: Step 1 of 2 name roster. Step 2 of 2 confirm name of roster.
 ************************************************************************************/
void show_current_step(char *str, int currentStep, int totalSteps)
{
  printf(BOLD "%s" RESET " Step: %d/%d\n", str, currentStep, totalSteps);
  printf("--------------------------------------------------\n");
}

void __utils_remove_newline_char(char *param)
{
  size_t len = strlen(param);
  if (len > 0 && param[len - 1] == '\n')
  {
    param[len - 1] = '\0';
  }
}

/************************************************************************************
 * __utils_clear_input_buffer(): Clears the input buffer to prevent unwanted behavior
 ************************************************************************************/
void __utils_clear_input_buffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
    // Keep reading until a newline or EOF is encountered
  }
}

/************************************************************************************
 * list_all_students(): Lists all students in the student database
 ************************************************************************************/
int list_all_students(void)
{

  int result = show_all_students_in_student_db(databaseInfo.dbPath);
  if (result == 0)
  {
    int showingStudents = TRUE;
    while (showingStudents == TRUE)
    {
      puts("Press any key to continue...");
      getchar();
      showingStudents = FALSE;
      system("clear");
    }
  }
  else if (result == 1)
  {
    __utils_error_logger("Could not find students in database or table: 'students' does not exist", "list_all_students()", MODERATE);
    puts(RED "Error: Could not find students in database or table: 'students' does not exist " RESET);
    puts("May need to add a student to the database then try this action again.");
    wait_for_char_input();
  }

  return 0;
}

/************************************************************************************
 * check_for_special_chars(): Checks if the passed in string has any special
 *                            characters. If it does it returns TRUE, else FALSE
 ************************************************************************************/
int check_for_special_chars(const char *str)
{
  while (*str)
  {
    if (*str == '!' || *str == '@' || *str == '#' || *str == '$' || *str == '%' || *str == '^' || *str == '&' || *str == '*' || *str == '(' || *str == ')' || *str == '-' || *str == '_' || *str == '=' || *str == '+' || *str == '[' || *str == ']' || *str == '{' || *str == '}' || *str == '|' || *str == ';' || *str == ':' || *str == '"' || *str == '\'' || *str == '<' || *str == '>' || *str == ',' || *str == '.' || *str == '/' || *str == '?' || *str == '`' || *str == '~')
    {
      return TRUE; // Special character found
    }
    str++;
  }
  return FALSE; // No special characters found
}

/************************************************************************************
 * has_one_non_space_char(): Helper function that checks if a string has at least
 * one non-space character.
 ************************************************************************************/
int has_one_non_space_char(const char *str)
{
  while (*str)
  {
    if (*str != ' ')
    {
      return TRUE; // Non-space character found
    }
    str++;
  }
  return FALSE; // Only spaces found
}

/************************************************************************************
 * wait_for_char_input(): Simple function that waits for the user to press any key
 ************************************************************************************/
int wait_for_char_input(void)
{
  puts("Press any key to continue...");
  getchar();
  system("clear");
}

int hash_data(char *data)
{
  int hash = 0;
  int c;

  while ((c = *data++))
  {
    hash += c;
  }

  return hash;
}
