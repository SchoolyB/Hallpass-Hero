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

#include "../lib/headers/cpp/cpp_files.hpp"
#include "../lib/headers/c/c_files.h"

static uint8_t mainMenuProccess;
int manageStudentDBMenuIsRunning;
Student CurrentStudent;

int manage_student_db(void)
{
  system("clear");
  manageStudentDBMenuIsRunning = TRUE;
  __utils_runtime_logger("entered manage student db menu", "manage_student_db");
  __utils_check_for_sqlite_db();
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
    __utils_fgets_and_remove_newline(userInput.StrInput);
    userInput.NumInput = atoi(userInput.StrInput);
    if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "list") == 0)
    {
      manageStudentDBMenuIsRunning = FALSE;
      system("clear");
      list_all_students();
      manageStudentDBMenuIsRunning = TRUE;
    }
    else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "update") == 0)
    {
      search_for_student();
    }
    else if (userInput.NumInput == 3 || strcmp(userInput.StrInput, "delete") == 0)
    {
      manageStudentDBMenuIsRunning = FALSE;
      system("clear");
      show_current_menu("Delete Student From Database");
      show_all_students_in_student_db(programSettings.databaseInfo.dbPath);
      printf("Enter the StudentID of the student you would like to delete.\n");
      printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
      __utils_fgets_and_remove_newline(userInput.StrInput);

      if (strcmp(userInput.StrInput, "cancel") == 0)
      {
        __utils_operation_cancelled("manage_student_db");
        manageStudentDBMenuIsRunning = TRUE;
      }
      else
      {
        // TODO NEW: Jan 24th 2024 Need to add a check that says something like "the entered student is also in roster: 'name of roster' preforming this action will also delete the student from the roster. are you sure you want to continue?"
        strcpy(CurrentStudent.StudentID, userInput.StrInput);
        int studentExists = check_if_student_id_exists(CurrentStudent.StudentID, "students");
        if (studentExists == TRUE)
        {
          system("clear");
          printf("%sWARNING: This action cannot be undone%s\n", red.colorCode, reset.colorCode);
          printf("Are you sure you want to delete this student?[y/n]\n");
          __utils_fgets_and_remove_newline(userInput.StrInput);
          if (INPUT_IS_CANCEL(userInput.StrInput) || INPUT_IS_NO(userInput.StrInput))
          {
            __utils_operation_cancelled("manage_student_db");
            manageStudentDBMenuIsRunning = TRUE;
          }
          else if (INPUT_IS_YES(userInput.StrInput))
          {
            delete_student_from_table(CurrentStudent.StudentID, "students");
          }
          else
          {
            system("clear");
            printf("%sInvalid input, please try again.%s\n", red.colorCode, reset.colorCode);
            sleep(1);
            system("clear");
            manageStudentDBMenuIsRunning = TRUE;
          }
        }
        else if (studentExists == FALSE)
        {
          system("clear");
          printf("Student with ID: %s does not exist.\n", CurrentStudent.StudentID);
          sleep(1);
          system("clear");
          printf("Please try again.\n");
          manageStudentDBMenuIsRunning = TRUE;
        }
      }
    }
    else if (userInput.NumInput == 4 || strcmp(userInput.StrInput, "main") == 0)
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

int update_student_info(void)
{
  system("clear");
  manageStudentDBMenuIsRunning = FALSE;
  search_for_student();
  // show_current_menu("Update Student Information");
}
