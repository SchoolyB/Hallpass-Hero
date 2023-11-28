/*
===============================================================================
File Name   : _add_student.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to add a student to the student database.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../lib/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"
#include "../lib/Cuazar/lib/Cuazar.h"

static char buffer[50];
static uint8_t menuInput;
static uint8_t mainMenuProccess;
int addStudentMenuIsRunning;
Student NewStudent;
/************************************************************************************
 * add_student_to_db(): This is the main function for adding a student
 * to the database.
 * Note: see usage in ./main.c
 ************************************************************************************/
int add_student_to_db(void)
{
  addStudentMenuIsRunning = TRUE;
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
      system("clear");
      addStudentMenuIsRunning = FALSE;
      create_student_db_and_table();
      get_student_first_name();
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

/************************************************************************************
 * get_student_first_name(): Self explanitory. Gets and confirms the students first name.
 * Note: see usage in add_student_to_db()
 ************************************************************************************/
int get_student_first_name(void)
{
  show_current_step("Enter Student First Name", 1, 6);
  char buffer[50];

  puts("Please enter the students first name.");
  puts(YELLOW "You can cancel this operation by entering 'cancel." RESET);
  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);

  if (strcmp(buffer, "cancel") == 0)
  {
    system("clear");
    puts(YELLOW "Canceling operation." RESET);
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }
  else
  {
    system("clear");
    show_current_step("Confirm Student First Name", 2, 6);
    printf("You entered: " BOLD "%s" RESET " is that correct(y/n)\n", buffer);

    /*Taking the entered value and
    assigning it to a variable.*/
    char *setFirstName[30];
    strcpy(setFirstName, buffer);

    /*Getting the confirmation*/
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    if (INPUT_IS_YES(buffer))
    {
      /*Confirming the entered value and
      setting the struct members value to it */
      strcpy(NewStudent.FirstName, setFirstName);
      get_student_last_name();
    }
    else if (INPUT_IS_NO(buffer))
    {
      system("clear");
      puts("Ok lets try again.");
      sleep(1);
      system("clear");
      get_student_first_name();
    }
    else
    {
      system("clear");
      puts("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      get_student_first_name();
    }
  }
}

/************************************************************************************
 * get_student_last_name(): Self explanitory. Gets and confirms the students last name.
 * Note: In the event that the student does not have a last name an empty string will be
                                                                  added to the database.
 * Note: see usage in add_student_to_db()
 ************************************************************************************/
int get_student_last_name(void)
{
  system("clear");
  show_current_step("Enter Student Last Name", 3, 6);
  char buffer[50];

  puts("Please enter the students last name.");
  puts("If the student does not have a last name please enter 'none'.");
  puts(YELLOW "You can cancel this operation by entering 'cancel." RESET);
  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);

  if (strcmp(buffer, "cancel") == 0)
  {
    system("clear");
    puts(YELLOW "Canceling operation." RESET);
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }
  else if (strcmp(buffer, "none") == 0)
  {
    system("clear");
    show_current_step("Confirm Student Last Name", 4, 6);
    puts(YELLOW "This student does not have a last name. is that correct?(y/n)" RESET);

    /*Taking the entered value and
    assigning it to a variable.*/
    char *setLastName[30];
    strcpy(setLastName, buffer);

    /*Getting the confirmation*/
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    if (INPUT_IS_YES(buffer))
    {
      /*Confirming the entered value and
      setting the struct members value to it */
      strcpy(NewStudent.LastName, setLastName);
      system("clear");
      return 0;
    }
    else if (INPUT_IS_NO(buffer))
    {
      system("clear");
      puts("Ok lets try again.");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
    else
    {
      system("clear");
      puts("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
  }
  else
  {
    system("clear");
    show_current_step("Confirm Student Last Name", 4, 6);
    printf("You entered: " BOLD "%s" RESET " is that correct?(y/n)\n", buffer);
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    if (INPUT_IS_YES(buffer))
    {
      system("clear");
      // TODO add student to db
    }
    else if (INPUT_IS_NO(buffer))
    {
      system("clear");
      puts("Ok lets try again.");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
    else
    {
      system("clear");
      puts("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
  }
}
/************************************************************************************
 * ask_about_student_id: Asks if the user wants to give a student an id.
 * Note: see usage in add_student_to_db()
 ************************************************************************************/
// TODO might make an option in the future to enable or disable the need to do this
int ask_about_student_id()
{
  char buffer[50];
  printf("Would you like to assign an id to " BOLD "%s  %s" RESET " ?(y/n) \n", NewStudent.FirstName, NewStudent.LastName);
  puts(YELLOW "You can assign an id later by choosing option #4 from the main menu." RESET);
  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);

  if (INPUT_IS_YES(buffer))
  {
    puts("How would you like to assign the id?");
    puts("Please enter either '1' or '2'.");
    puts("1. Automatically generate and ID ");
    puts("2. Enter your own");

    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    menuInput = atoi(buffer);

    if (menuInput == 1)
    {
      system("clear");
      puts("Okay I will generate an id for this student");
      // TODO  generate_student_id();
    }
    else if (menuInput == 2)
    {
      system("clear");
      // TODO manually_set_student_id()
    }
    else
    {
      system("clear");
      puts("Please make a valid decision.");
      ask_about_student_id();
    }
  }
  else if (INPUT_IS_NO(buffer))
  {
    printf("You've decided not to assign an id to " BOLD "%s  %s" RESET ".\n This can be done later on if you change your mind", NewStudent.FirstName, NewStudent.LastName);
    return 0;
  }
}

/************************************************************************************
 * manually_set_student_id(): Allows the user to manually set a students id.
 * Note: see usage in add_student_to_db()
 ************************************************************************************/
void manually_set_student_id()
{
}

/************************************************************************************
 * generate_student_id(): This takes the proposed students first and last name and
 *                        and generates a unique id
 * Note: see usage in add_student_to_db()
 ************************************************************************************/
// int generate_student_id()
// {
// }
