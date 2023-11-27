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
#include "utils.h"

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
    fprintf(errorLog, "Minor Error: %s in function: %s()\n", errorMessage, function);
    fprintf(errorLog, "=============================================================\n");
    fflush(errorLog);
    return 0;
    break;
  case MODERATE:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "Moderate Error: %s in function: %s()\n", errorMessage, function);
    fprintf(errorLog, "=============================================================\n");
    fflush(errorLog);
    return 1;
  case CRITICAL:
    fprintf(errorLog, "Logged @ %s", ctime(&currentTime));
    fprintf(errorLog, "CRITICAL ERROR: %s in function: %s()\n", errorMessage, function);
    fprintf(errorLog, "=============================================================\n");
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
// Typedefs

/* programs are like courses/degrees/certs/diplomas
basically any over arching program that a student is
a part of that has a goal or achievement at the end*/
// typedef struct
// {
//   char Name[20];
//   char Description[100];
//   char Start_date[20];
//   char End_date[20];
//   int Max_students;
//   int Current_students;
// } Program;

// typedef struct
// {
//   char FirstName[20];
//   char LastName[20];
//   int StudentID;
//   /*
//   TODO add members for attendance, grades/performance
//    */
//   // Program CurrentProgram;

// } Student;

// typedef struct
// {
//   char firstName[20];
//   char lastName[20];
//   char title[20];
// } Instructor;

/*Rosters a individual classes
i.e 1st period History, Physiology 342,
// June Fullstack Cohort, etc. */
// typedef struct
// {
//   Instructor LeadInstructor;
//   Student Students[100];
// } Roster;
