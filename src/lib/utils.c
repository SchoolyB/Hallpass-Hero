#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

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
    fprintf(errorLog, "CRITICAL Error: %s in function: %s()\n", errorMessage, function);
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

void show_current_menu(char *str)
{
  printf("Current Menu: " BOLD "%s" RESET "\n", str);
}

void UTILS_REMOVE_NEWLINE_CHAR(char *param)
{
  size_t len = strlen(param);
  if (len > 0 && param[len - 1] == '\n')
  {
    param[len - 1] = '\0';
  }
}

// Typedefs

/* programs are like courses/degrees/certs/diplomas
basically any over arching program that a student is
a part of that has a goal or achievement at the end*/
typedef struct
{
  char Name[20];
  char Description[100];
  char Start_date[20];
  char End_date[20];
  int Max_students;
  int Current_students;
} Program;

typedef struct
{
  char FirstName[20];
  char LastName[20];
  int StudentID;
  /*
  TODO add members for attendance, grades/performance
   */
  Program CurrentProgram;

} Student;

typedef struct
{
  char firstName[20];
  char lastName[20];
  char title[20];
} Instructor;

/*Rosters a individual classes
i.e 1st period History, Physiology 342,
June Fullstack Cohort, etc. */
typedef struct
{
  Instructor LeadInstructor;
  Student Students[100];
} Roster;
