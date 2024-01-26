/*
===============================================================================
File Name   : utils.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains several utility functions that are used
              throughout the 'C' portion of the source code. This file also
              contains the typedefs for the structs used in the program.
              TODO might move the funcs and typedefs to the header file
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/db.hpp"
/************************************************************************************
 * UTILS_ERROR_LOGGER(): Logs errors errors that occur specifically in 'C' files
 * Note: For 'C++' code error logging see CPP_UTILS_ERROR_LOGGER in utils.cpp
 ************************************************************************************/
int UTILS_ERROR_LOGGER(char *errorMessage, char *function, enum ErrorLevel level)
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

  // Add cases for other error levels if needed
  default:
    // Handle unknown error level
    break;
  }
}
/************************************************************************************
 * UTILS_RUNTIME_LOGGER(): Logs runtime activity
 *
 ************************************************************************************/
int UTILS_RUNTIME_LOGGER(char *action, char *functionName)
{
  FILE *runtimeLog = fopen("../logs/runtime.log", "a");
  time_t currentTime;
  time(&currentTime);

  fprintf(runtimeLog, "Logged @ %s", ctime(&currentTime));

  fprintf(runtimeLog, "User Action: %s, in function call: %s()\n", action, functionName);

  fprintf(runtimeLog, "======================================================================================\n\n");
  fflush(runtimeLog);
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

void UTILS_REMOVE_NEWLINE_CHAR(char *param)
{
  size_t len = strlen(param);
  if (len > 0 && param[len - 1] == '\n')
  {
    param[len - 1] = '\0';
  }
}

/************************************************************************************
 * UTILS_CLEAR_INPUT_BUFFER(): Clears the input buffer to prevent unwanted behavior
 ************************************************************************************/
void UTILS_CLEAR_INPUT_BUFFER()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
  {
    // Keep reading until a newline or EOF is encountered
  }
}

// TODO add desc for func
int list_all_students(void)
{

  int result = show_all_students_in_student_db("../build/db.sqlite");
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
    UTILS_ERROR_LOGGER("Could not find students in database or table: 'students' does not exist", "list_all_students()", MODERATE);
    puts(RED "Error: Could not find students in database or table: 'students' does not exist " RESET);
    puts("May need to add a student to the database then try this action again.");
    wait_for_char_input();
  }

  return 0;
}

/*Helper function that takes in a string/input.
Checks if the string has at least one non-space character
i.e  "        "would return FALSE
     "  a    " would return TRUE
*/
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

// TODO add desc for func
int wait_for_char_input(void)
{
  sleep(2);
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
