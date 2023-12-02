/*
===============================================================================
File Name   : _manage_student.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to manage a student.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

static char buffer[50];
static uint8_t menuInput;
static uint8_t mainMenuProccess;
int manageStudentDBMenuIsRunning;
Student CurrentStudent;

int manage_student_db(void)
{
  system("clear");
  manageStudentDBMenuIsRunning = TRUE;
  while (manageStudentDBMenuIsRunning == TRUE)
  {
    show_current_menu("Manage Student Database");
    mainMenuProccess = FALSE;
    char manageStudentDBMenu[5][50] = {
        "List All Students In Database",
        "View Student Information",
        "Update Student Information",
        "Delete Student From Database",
        "Main Menu"};

    printf("What would you like to do: \n");
    for (int i = 0; i < 5; i++)
    {
      printf("%d. %s\n", i + 1, manageStudentDBMenu[i]);
    }
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "list") == 0)
    {
      system("clear");
      manageStudentDBMenuIsRunning = FALSE;
      // view_student_info();
    }
    else if (menuInput == 2 || strcmp(buffer, "view") == 0)
    {
      list_all_students();
    }
    else if (menuInput == 3 || strcmp(buffer, "update") == 0)
    {
      // do stuff
    }
    else if (menuInput == 3 || strcmp(buffer, "update") == 0)
    {
      // do stuff
    }
    else if (menuInput == 4 || strcmp(buffer, "delete") == 0)
    {
      // do stuff
    }
    else if (menuInput == 5 || strcmp(buffer, "main") == 0)
    {
      system("clear");
      manageStudentDBMenuIsRunning = FALSE;
      // Not going to lie... I dont remember how this works but it does.
      mainMenuProccess = TRUE;
    }
    else
    {
      system("clear");
      printf("Invalid input, please try again.\n");
      sleep(1);
      system("clear");
      manage_student_db();
    }
  }
  return 0;
}
