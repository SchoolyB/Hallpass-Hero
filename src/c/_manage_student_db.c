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

static uint8_t mainMenuProccess;
int manageStudentDBMenuIsRunning;
UserInput manageStudentDBInput;
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
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageStudentDBInput.StrInput);
    manageStudentDBInput.NumInput = atoi(manageStudentDBInput.StrInput);
    if (manageStudentDBInput.NumInput == 1 || strcmp(manageStudentDBInput.StrInput, "list") == 0)
    {
      manageStudentDBMenuIsRunning = FALSE;
      system("clear");
      list_all_students();
      manageStudentDBMenuIsRunning = TRUE;
    }
    else if (manageStudentDBInput.NumInput == 2 || strcmp(manageStudentDBInput.StrInput, "update") == 0)
    {
      search_for_student();
    }
    else if (manageStudentDBInput.NumInput == 3 || strcmp(manageStudentDBInput.StrInput, "delete") == 0)
    {
      manageStudentDBMenuIsRunning = FALSE;
      system("clear");
      show_current_menu("Delete Student From Database");
      show_students_in_db("../build/db.sqlite");
      printf("Enter the StudentID of the student you would like to delete.\n");
      printf(YELLOW "Enter 'cancel' to cancel this operation.\n" RESET);
      UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageStudentDBInput.StrInput);

      if (strcmp(manageStudentDBInput.StrInput, "cancel") == 0)
      {
        system("clear");
        puts(YELLOW "Cancelling operation" RESET);
        sleep(1);
        system("clear");
        manageStudentDBMenuIsRunning = TRUE;
      }
      else
      {
        // TODO NEW: Jan 24th 2024 Need to add a check that says something like "the entered student is aldo in roster 'name of roster' preforming this action will also delete the student from the roster. are you sure you want to continue?"
        strcpy(CurrentStudent.StudentID, manageStudentDBInput.StrInput);
        int studentExists = check_if_student_id_exists(CurrentStudent.StudentID, "students");
        if (studentExists == TRUE)
        {
          system("clear");
          printf(RED "WARNING: This action cannot be undone\n" RESET);
          puts("Are you sure you want to delete this student?[y/n]");
          UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageStudentDBInput.StrInput);
          if (INPUT_IS_CANCEL(manageStudentDBInput.StrInput) || INPUT_IS_NO(manageStudentDBInput.StrInput))
          {
            system("clear");
            puts(YELLOW "Cancelling operation" RESET);
            sleep(1);
            system("clear");
            manageStudentDBMenuIsRunning = TRUE;
          }
          else if (INPUT_IS_YES(manageStudentDBInput.StrInput))
          {
            delete_student_from_table(CurrentStudent.StudentID, "students");
          }
          else
          {
            system("clear");
            puts(RED "Invalid input, please try again." RESET);
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
          puts("Please try again.");
          manageStudentDBMenuIsRunning = TRUE;
        }
      }
    }
    else if (manageStudentDBInput.NumInput == 4 || strcmp(manageStudentDBInput.StrInput, "main") == 0)
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
