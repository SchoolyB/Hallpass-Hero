/*
===============================================================================
File Name   : _add_student.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to add a student to the student database.
-------------------------------------------------------------------------------
Helper functions from db.hpp:

===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../lib/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

int add_student_to_db(void)
{
  char buffer[50];
  int menuInput;
  int mainMenuProccess;
  int addStudentMenuIsRunning = TRUE;

  while (addStudentMenuIsRunning == TRUE)
  {
    mainMenuProccess = FALSE;
    char addStudentOptions[4][50] = {
        "1. Add a student",
        "2. View students", // TODO unsure how this will work
        "3. Help",
        "4. Main Menu"};
    system("clear");
    show_current_menu("Add Student To Database");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 4; ++i)
    {
      printf("| %s %-90s\n", addStudentOptions[i], "");
    }
    puts("|===========================================================================================");
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    menuInput = atoi(buffer);

    if (menuInput == 1 || strcmp(buffer, "add student") == 0 || strcmp(buffer, "add") == 0)
    {
      // TODO add student to database
    }
    else if (menuInput == 2 || strcmp(buffer, "view students") == 0 || strcmp(buffer, "view") == 0)
    {
      // TODO view students
    }
    else if (menuInput == 3 || strcmp(buffer, "help") == 0)
    {
      // TODO help
    }
    else if (menuInput == 4 || strcmp(buffer, "main menu") == 0 || strcmp(buffer, "main") == 0)
    {
      system("clear");
      puts("Returning to main menu.");
      sleep(1);
      system("clear");
      return 0;
    }
    else
    {
      system("clear");
      addStudentMenuIsRunning = FALSE;
      puts("Invalid input. Please try again.");
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
    }
  }
  return 0;
}