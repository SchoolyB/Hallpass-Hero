/*
===============================================================================
File Name   : _manage_student_db.c
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
    char manageStudentDBMenu[4][50] = {
        "List All Students In Database",
        "Update Student Information",
        "Delete Student From Database",
        "Main Menu"};

    printf("What would you like to do: \n");
    for (int i = 0; i < 4; i++)
    {
      printf("%d. %s\n", i + 1, manageStudentDBMenu[i]);
    }
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "list") == 0)
    {
      manageStudentDBMenuIsRunning = FALSE;
      system("clear");
      list_all_students();
      manageStudentDBMenuIsRunning = TRUE;
    }
    else if (menuInput == 2 || strcmp(buffer, "update") == 0)
    {
      search_for_student();
    }
    else if (menuInput == 3 || strcmp(buffer, "delete") == 0)
    {
      manageStudentDBMenuIsRunning = FALSE;
      system("clear");
      show_current_menu("Delete Student From Database");
      show_students_in_db("../build/db.sqlite");
      puts("Which student would you like to delete from the database?");
      puts(RED "WARNING: This action cannot be undone!" RESET);
      puts(YELLOW "You can cancel this operation by entering 'cancel'." RESET);
      UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);

      if (strcmp(buffer, "cancel") == 0)
      {
        system("clear");
        puts(YELLOW "Cancelling operation" RESET);
        sleep(1);
        system("clear");
        manageStudentDBMenuIsRunning = TRUE;
      }

    }
    else if (menuInput == 4 || strcmp(buffer, "main") == 0)
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

int delete_student_from_db()
{
  
}

int update_student_info(void)
{
  system("clear");
  manageStudentDBMenuIsRunning = FALSE;
  search_for_student();
  // show_current_menu("Update Student Information");
}
