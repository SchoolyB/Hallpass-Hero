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
#include <time.h>
#include <ctype.h>
#include "../lib/headers/c_files.h"
#include "../lib/headers/utils.h"
#include "../lib/headers/db.hpp"

static uint8_t mainMenuProccess;
int addStudentMenuIsRunning;
UserInput addStudentInput;
Student NewStudent;
Roster destinationRoster;
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
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    addStudentInput.NumInput = atoi(addStudentInput.StrInput);

    if (addStudentInput.NumInput == 1 || strcmp(addStudentInput.StrInput, "add student") == 0 || strcmp(addStudentInput.StrInput, "add") == 0)
    {
      system("clear");
      addStudentMenuIsRunning = FALSE;
      create_student_db_and_table();
      get_student_first_name();
    }
    else if (addStudentInput.NumInput == 2 || strcmp(addStudentInput.StrInput, "view students") == 0 || strcmp(addStudentInput.StrInput, "view") == 0)
    {
      addStudentMenuIsRunning = FALSE;
      system("clear");
      list_all_students();
    }
    else if (addStudentInput.NumInput == 3 || strcmp(addStudentInput.StrInput, "help") == 0)
    {
      // TODO help
    }
    else if (addStudentInput.NumInput == 4 || strcmp(addStudentInput.StrInput, "main menu") == 0 || strcmp(addStudentInput.StrInput, "main") == 0)
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
 * get_student_first_name():  . Gets and confirms the students first name.
 * Note: see usage in add_student_to_db()
 ************************************************************************************/
int get_student_first_name(void)
{
  show_current_step("Enter Student First Name", 1, 7);

  puts("Please enter the students first name.");
  puts(YELLOW "You can cancel this operation by entering 'cancel." RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
  if (!has_one_non_space_char(addStudentInput.StrInput))
  {
    printf(YELLOW "The entered first name is too short. Please try again\n" RESET);
    sleep(1);
    system("clear");
    UTILS_ERROR_LOGGER("The entered first name is too short", "get_student_first_name", MINOR);
    get_student_first_name();
  }
  else if (strcmp(addStudentInput.StrInput, "cancel") == 0)
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
    show_current_step("Confirm Student First Name", 2, 7);
    printf("You entered: " BOLD "%s" RESET " is that correct[y/n]\n", addStudentInput.StrInput);

    /*Taking the entered value and
    assigning it to a variable.*/
    char setFirstName[30];
    strcpy(setFirstName, addStudentInput.StrInput);

    /*Getting the confirmation*/
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    if (INPUT_IS_YES(addStudentInput.StrInput))
    {
      /*Confirming the entered value and
      setting the struct members value to it */
      if (setFirstName[0] != '\0')
      {
        /* Ensuring the first letter of the first name
        is capitalized when it hits the database */
        setFirstName[0] = toupper(setFirstName[0]);
        strcpy(NewStudent.FirstName, setFirstName);
        get_student_last_name();
        return 0;
      }
    }
    else if (INPUT_IS_NO(addStudentInput.StrInput))
    {
      system("clear");
      puts("Ok lets try again.");
      sleep(1);
      system("clear");
      get_student_first_name();
      return 0;
    }
    else
    {
      system("clear");
      puts("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      get_student_first_name();
      return 0;
    }
  }
}

/************************************************************************************
 * get_student_last_name():  . Gets and confirms the students last name.
 * Note: In the event that the student does not have a last name an empty string will be
                                                                  added to the database.
 * Note: see usage in get_student_first_name()
 ************************************************************************************/
int get_student_last_name(void)
{
  char setLastName[30];
  system("clear");
  show_current_step("Enter Student Last Name", 3, 7);

  puts("Please enter the students last name.");
  puts("If the student does not have a last name please enter 'none'.");
  puts(YELLOW "You can cancel this operation by entering 'cancel." RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
  if (!has_one_non_space_char(addStudentInput.StrInput))
  {
    printf(YELLOW "The entered last name is too short. Please try again\n" RESET);
    printf(YELLOW "If the student does not have a last name please enter 'none'.\n" RESET);
    sleep(2);
    system("clear");
    UTILS_ERROR_LOGGER("The entered last name is too short", "get_student_last_name", MINOR);
    get_student_last_name();
  }
  else if (strcmp(addStudentInput.StrInput, "cancel") == 0)
  {
    system("clear");
    puts(YELLOW "Canceling operation." RESET);
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }
  else if (strcmp(addStudentInput.StrInput, "none") == 0)
  {
    system("clear");
    show_current_step("Confirm Student Last Name", 4, 7);
    puts(YELLOW "This student does not have a last name. is that correct?[y/n]" RESET);

    /*Taking the entered value and
    assigning it to a variable.*/
    strcpy(setLastName, "");

    /*Getting the confirmation*/
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    if (INPUT_IS_YES(addStudentInput.StrInput))
    {
      /*Confirming the entered value and
      setting the struct members value to it */
      strcpy(NewStudent.LastName, setLastName);
      system("clear");
      ask_about_student_id();
    }
    else if (INPUT_IS_NO(addStudentInput.StrInput))
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
    strcpy(setLastName, addStudentInput.StrInput);
    system("clear");
    show_current_step("Confirm Student Last Name", 4, 7);
    printf("You entered: " BOLD "%s" RESET " is that correct?[y/n]\n", setLastName);
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    if (INPUT_IS_YES(addStudentInput.StrInput))
    {
      if (setLastName[0] != '\0')
      {
        /* Ensuring the first letter of the last name
        is capitalized when it hits the database */
        setLastName[0] = toupper(setLastName[0]);
        strcpy(NewStudent.LastName, setLastName);
        system("clear");
        ask_about_student_id();
      }
    }
    else if (INPUT_IS_NO(addStudentInput.StrInput))
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
 * Note: see usage in get_student_last_name()
 ************************************************************************************/
// TODO might make an option in the future to enable or disable the need to do this
int ask_about_student_id(void)
{
  system("clear");
  puts("How would you like to assign the student ID?");
  puts("Please enter either '1' or '2'.");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  puts("1. Automatically generate an ID ");
  puts("2. Create your own");

  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
  addStudentInput.NumInput = atoi(addStudentInput.StrInput);

  if (strcmp(addStudentInput.StrInput, "cancel") == 0)
  {
    system("clear");
    puts(YELLOW "Canceling operation." RESET);
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }

  if (addStudentInput.NumInput == 1)
  {
    system("clear");
    generate_student_id(NewStudent.FirstName, NewStudent.LastName);
  }
  else if (addStudentInput.NumInput == 2)
  {
    system("clear");
    manually_set_student_id();
  }

  else
  {
    system("clear");
    puts("Please make a valid decision.");
    ask_about_student_id();
  }
}

/************************************************************************************
 * manually_set_student_id(): Allows the user to manually set a students id.
 * Note: see usage in ask_about_student_id()
 ************************************************************************************/
void manually_set_student_id(void)
{
  int gettingStudentId = TRUE;

  while (gettingStudentId == TRUE)
  {
    show_current_step("Set Student Id", 5, 7);
    puts("Please enter the students id.");
    puts(YELLOW "NOTE: The id can be no MORE than 15 characters and LESS than 2." RESET);
    puts(RED "IMPORTANT: It is not recommended to use private information such as social security numbers or birth dates as an id." RESET);
    puts(YELLOW "You can cancel this operation by entering 'cancel." RESET);
    gettingStudentId = FALSE;
  }
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);

  if (strcmp(addStudentInput.StrInput, "cancel") == 0)
  {
    system("clear");
    puts(YELLOW "Canceling operation." RESET);
    sleep(1);
    system("clear");
    gettingStudentId == FALSE;
    addStudentMenuIsRunning = TRUE;
  }
  else if (strlen(addStudentInput.StrInput) < 2)
  {
    gettingStudentId == FALSE;
    puts(YELLOW "The id you entered is too short. Please try again." RESET);
    sleep(1);
    system("clear");
    manually_set_student_id();
  }
  else if (strlen(addStudentInput.StrInput) > 15)
  {
    gettingStudentId == FALSE;
    puts(YELLOW "The id you entered is too long. Please try again." RESET);
    sleep(1);
    system("clear");
    manually_set_student_id();
  }
  else
  {
    gettingStudentId == FALSE;
    system("clear");
    char studentID[15];
    strcpy(studentID, addStudentInput.StrInput);
    int result = check_if_student_id_exists(studentID);
    if (result == FALSE)
    {
      confirm_manually_entered_student_id(studentID);
    }
    else if (result == TRUE)
    {
      system("clear");
      printf(YELLOW "Duplicate ID detected. Please try again.\n" RESET);
      sleep(3);
      system("clear");
      manually_set_student_id();
    }
  }
}

/************************************************************************************
 * generate_student_id(): This takes the proposed students first and last
 *                        name and 3 random digits to create a student id.
 *
 * Note: see usage in ask_about_student_id()
 ************************************************************************************/
int generate_student_id(char *FirstName, char *LastName)
{
  char arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  char *setStudentID[15];

  srand(time(NULL));

  int digit1 = arr[rand() % 10];
  int digit2 = arr[rand() % 10];
  int digit3 = arr[rand() % 10];

  puts("Generating student ID");
  sleep(1);
  size_t lastNameLength = strlen(NewStudent.LastName);
  // if the student last name is empty
  if (strcmp(LastName, "") == 0)
  {

    snprintf(setStudentID, sizeof(setStudentID), "%c%c%d%d%d", toupper(FirstName[0]), toupper(FirstName[1]), digit1, digit2, digit3);
    puts(GREEN "ID successfully generated!" RESET);
    sleep(1);
    system("clear");
    confirm_generated_student_id(setStudentID);
  }
  /*In the event that the last name is to0 long
  the last name will be truncated to 10 characters
  */
  else if (strlen(NewStudent.LastName) > 10)
  {
    char truncatedLastName[11];
    strncpy(truncatedLastName, NewStudent.LastName, 10);
    truncatedLastName[10] = '\0';

    printf(YELLOW "Entered last name:" BOLD " %s" RESET YELLOW
                  " longer than 10 characters, this will be shortened to" BOLD " %s in the student ID \n" RESET,
           NewStudent.LastName, truncatedLastName);
    printf("Are you sure you want to continue with this students last name?[y/n]\n");
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    if (INPUT_IS_YES(addStudentInput.StrInput))
    {
      system("clear");
      snprintf(setStudentID, sizeof(setStudentID), "%c%c%s%d%d%d", toupper(FirstName[0]), toupper(FirstName[1]), truncatedLastName, digit1, digit2, digit3);
      int result = check_if_student_id_exists(setStudentID);
      if (result == FALSE)
      {
        puts(GREEN "ID successfully generated!" RESET);
        sleep(1);
        system("clear");
        sleep(2);
        confirm_generated_student_id(setStudentID);
      }
      else if (result == TRUE)
      {
        system("clear");
        sleep(1);
        printf(YELLOW "Duplicate ID detected. Regenerating....\n" RESET);
        sleep(1);
        generate_student_id(FirstName, LastName);
      }
    }
    else if (INPUT_IS_NO(addStudentInput.StrInput))
    {
      handle_last_name_truncated_menu();
    }
    else
    {
      system("clear");
      puts("Please make a valid decision.");
      sleep(1);
      system("clear");
      generate_student_id(FirstName, LastName);
    }
  }
  else
  {
    snprintf(setStudentID, sizeof(setStudentID), "%c%c%s%d%d%d", toupper(FirstName[0]), toupper(FirstName[1]), LastName, digit1, digit2, digit3);
    // printf("%s\n", setStudentID);
    int result = check_if_student_id_exists(setStudentID);
    if (result == FALSE)
    {
      puts(GREEN "ID successfully generated!" RESET);
      sleep(1);
      system("clear");
      confirm_generated_student_id(setStudentID);
    }
    else if (result == TRUE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "Duplicate ID detected. Regenerating....\n" RESET);
      sleep(1);
      generate_student_id(FirstName, LastName);
    }
  }
}

/************************************************************************************
 * confirm_generated_student_id(): Confirms the auto generated students id.
 *
 * Note: see usage in generate_student_id()
 ************************************************************************************/
int confirm_generated_student_id(char *studentID)
{
  system("clear");
  int confirmingStudentId = TRUE;
  while (confirmingStudentId == TRUE)
  {
    show_current_step("Confirm Student Id", 6, 7);
    printf("Is the ID: " BOLD "%s" RESET " for" BOLD " %s %s" RESET " satisfactory?[y/n]\n", studentID, NewStudent.FirstName, NewStudent.LastName);
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    char setStudentID[15];
    strcpy(setStudentID, studentID);

    if (INPUT_IS_YES(addStudentInput.StrInput))
    {
      confirmingStudentId = FALSE;
      show_current_step("Confirm Student Id", 6, 7);
      strcpy(NewStudent.StudentID, setStudentID);
      system("clear");
      /*Doing this allows me to use the full or part of
      functions from this file in other places in the project.
      I could modify what I Wanted to show depending on the
      global trigger value. - Marshall Burns Jan 24 2024*/
      if (globalTrigger.isTriggered == FALSE)
      {
        ask_to_add_new_student_to_roster();
      }
      else
      {
        return 0;
      }
    }

    else if (INPUT_IS_NO(addStudentInput.StrInput))
    {
      confirmingStudentId = FALSE;
      system("clear");
      puts("Ok lets try again.");
      sleep(1);
      system("clear");
      generate_student_id(NewStudent.FirstName, NewStudent.LastName);
    }
    else
    {
      confirmingStudentId = FALSE;
      puts("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      confirm_generated_student_id(studentID);
    }
  }
}

/************************************************************************************
 * confirm_manually_entered_student_id(): Confirms the students id. if
 *                                         manually entered.
 * Note: see usage in manually_set_student_id()
 ************************************************************************************/
int confirm_manually_entered_student_id(char *studentID)
{
  system("clear");
  int confirmingStudentId = TRUE;
  while (confirmingStudentId == TRUE)
  {
    show_current_step("Confirm Student Id", 6, 7);
    printf("You've entered: " BOLD "%s" RESET "is that correct?[y/n]\n", studentID);
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
    char setStudentID[15];
    strcpy(setStudentID, studentID);

    if (INPUT_IS_YES(addStudentInput.StrInput))
    {
      confirmingStudentId = FALSE;
      show_current_step("Confirm Student Id", 6, 7);
      strcpy(NewStudent.StudentID, setStudentID);
      system("clear");

      /*Doing this allows me to use the full or part of
      functions from this file in other places in the project.
      I could modify what I Wanted to show depending on the
      global trigger value. - Marshall Burns Jan 24 2024*/
      if (globalTrigger.isTriggered == FALSE)
      {
        ask_to_add_new_student_to_roster();
      }
      else
      {
        return 0;
      }
    }
    else if (INPUT_IS_NO(addStudentInput.StrInput))
    {
      confirmingStudentId = FALSE;
      system("clear");
      puts("Ok lets try again.");
      sleep(1);
      system("clear");
      manually_set_student_id();
    }
    else
    {
      confirmingStudentId = FALSE;
      puts("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      confirm_manually_entered_student_id(studentID);
    }
  }
}

/************************************************************************************
 * ask_to_add_new_student_to_roster(): Once Students first name, last name, and ID is created
 *                              asks user if they want to add the newly created student to DB.
 *
 * Note: see usage in confirm_manually_entered_student_id()
 *                    confirm_generated_student_id()
 ************************************************************************************/
int ask_to_add_new_student_to_roster(void)
{

  system("clear");
  show_current_step("Add Student To Roster", 7, 7);
  puts("Would you like to add this student to a roster?");
  puts(YELLOW "NOTE: You can choose to do this later as well" RESET);

  puts("1: Yes");
  puts("2: No");
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
  addStudentInput.NumInput = atoi(addStudentInput.StrInput);
  if (addStudentInput.NumInput == 1 || INPUT_IS_YES(addStudentInput.StrInput))
  {
    ask_which_roster_to_add_newly_created_student();
    puts(GREEN "Adding new student to database..." RESET);
    sleep(2);
    system("clear");
    int result = insert_student_into_db(NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);

    if (result == 0)
    {
      puts(GREEN "Student successfully added to database." RESET);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    else if (result == 1)
    {
      puts(RED "Please try again." RESET);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
  }
  else if (addStudentInput.NumInput == 2 || INPUT_IS_NO(addStudentInput.StrInput))
  {
    int result = insert_student_into_db(NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);
    if (result == 0)
    {
      puts(GREEN "Student successfully added to database." RESET);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    else if (result == 1)
    {
      puts(RED "Please try again." RESET);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
  }
  else
  {
    puts("Sorry, I didn't understand that.");
    puts("Please try again");
    ask_to_add_new_student_to_roster();
  }
}

/************************************************************************************
 * ask_which_roster_to_add_newly_created_student(): Gets the name of the roster that
 *                                                  the user wants to add the newly
 *                                                  created student to
 *
 * Note: see usage ask_to_add_new_student_to_roster()
 ************************************************************************************/
int ask_which_roster_to_add_newly_created_student(void)
{
  puts("Please enter the name of the roster you would like to add this student to.");
  system("clear");
  show_tables();
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
  sprintf(destinationRoster.rosterNameWithPrefix, "Roster_%s", addStudentInput.StrInput);
  int rosterExists = check_if_table_exists(destinationRoster.rosterNameWithPrefix);
  if (rosterExists == TRUE)
  {
    puts("Table exists");
    int addedToRoster = add_student_to_roster(destinationRoster.rosterNameWithPrefix, NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);
    if (addedToRoster == TRUE)
    {
      system("clear");
      puts(GREEN "Student successfully added to roster." RESET);
      sleep(1);
      system("clear");
      return 0;
    }
    else
    {
      system("clear");
      printf(RED "Failed to add student to roster. Please try again.\n" RESET);
      UTILS_ERROR_LOGGER("Failed to add student to roster", "ask_which_roster_to_add_newly_created_student", MINOR);
    }
  }
  else if (rosterExists == FALSE)
  {
    system("clear");
    printf(YELLOW "The entered name:" BOLD RESET YELLOW "%s does not exist. Please try again.\n" RESET, addStudentInput.StrInput);
    sleep(1);
    system("clear");
    ask_which_roster_to_add_newly_created_student();
  }
}

/************************************************************************************
 * handle_last_name_truncated_menu(): Shows a menu and handles decision in the event
 *                                    that the entered students last name is over 10 char
 *
 * Note: see usage in generate_student_id()
 ************************************************************************************/
int handle_last_name_truncated_menu(void)
{
  sleep(1);
  system("clear");
  puts("Please enter the number of an option listed below?");
  puts("1. Enter a new last name");
  puts("2. Manually set an ID");
  puts("3. Cancel");
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(addStudentInput.StrInput);
  addStudentInput.NumInput = atoi(addStudentInput.StrInput);
  if (addStudentInput.NumInput == 1)
  {
    system("clear");
    get_student_last_name();
  }
  else if (addStudentInput.NumInput == 2)
  {
    system("clear");
    manually_set_student_id();
  }
  else if (addStudentInput.NumInput == 3)
  {
    system("clear");
    puts(YELLOW "Canceling operation." RESET);
    sleep(1);
    system("clear");
    add_student_to_db();
  }
  else
  {
    system("clear");
    puts("Please enter a valid decision.");
    handle_last_name_truncated_menu();
  }
  return 0;
}

/************************************************************************************
 * skip_and_add_to_roster(): Helper used to skip the process of adding a student
 *                           to a roster.
 *
 * Note: see usage in _manage_roster.c
 ************************************************************************************/
int skip_and_add_to_roster(const char *rosterName)
{
  int result = add_student_to_roster(rosterName, NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);
  if (result == TRUE)
  {
    int addedToStudentDB = insert_student_into_db(NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);
    if (addedToStudentDB == 0)
    {
      printf(GREEN "Student successfully added to roster" BOLD " %s.\n" RESET, rosterName);
      sleep(1);
      puts(GREEN "Student successfully added to database." RESET);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    else if (addedToStudentDB == 1)
    {
      puts(RED "Please try again." RESET);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    return 0;
  }
  else
  {
    system("clear");
    printf(RED "Failed to add student to roster. Please try again.\n" RESET);
    UTILS_ERROR_LOGGER("Failed to add student to roster", "skip_and_add_to_roster", MINOR);
  }
}
