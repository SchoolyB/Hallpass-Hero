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
Student NewStudent;
Roster destinationRoster;
DesiredTableName desiredTableName;

/************************************************************************************
 * add_student_to_db(): This is the main function for adding a student
 * to the database.
 * Note: see usage in ./main.c
 ************************************************************************************/
int add_student_to_db(void)
{
  globalTrigger.isTriggered = TRUE;
  addStudentMenuIsRunning = TRUE;
  while (addStudentMenuIsRunning == TRUE)
  {
    mainMenuProccess = FALSE;
    char addStudentOptions[4][50] = {
        "1. Add a student",
        "2. View students",
        "3. Help",
        "4. Main Menu"};
    system("clear");
    show_current_menu("Add Student To Database");
    printf("What would you like to do?\n");
    printf("|===========================================================================================\n");
    for (int i = 0; i < 4; ++i)
    {
      printf("| %s %-90s\n", addStudentOptions[i], "");
    }
    printf("|===========================================================================================\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);
    userInput.NumInput = atoi(userInput.StrInput);

    if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "add student") == 0 || strcmp(userInput.StrInput, "add") == 0)
    {
      system("clear");
      addStudentMenuIsRunning = FALSE;
      create_student_db_and_table();
      get_student_first_name();
    }
    else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "view students") == 0 || strcmp(userInput.StrInput, "view") == 0)
    {
      addStudentMenuIsRunning = FALSE;
      system("clear");
      list_all_students();
    }
    else if (userInput.NumInput == 3 || strcmp(userInput.StrInput, "help") == 0)
    {
      // TODO help
    }
    else if (userInput.NumInput == 4 || strcmp(userInput.StrInput, "main menu") == 0 || strcmp(userInput.StrInput, "main") == 0)
    {
      system("clear");
      printf("Returning to main menu.");
      sleep(1);
      system("clear");
      return 0;
    }
    else
    {
      system("clear");
      addStudentMenuIsRunning = FALSE;
      printf("Invalid input. Please try again.");
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
  printf("Please enter the students first name.\n");
  printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
  __utils_fgets_and_remove_newline(userInput.StrInput);
  __utils_runtime_logger("entered a students first name", "get_student_first_name");
  if (!has_one_non_space_char(userInput.StrInput))
  {
    printf("%sThe entered first name is too short. Please try again.%s\n", yellow.colorCode, reset.colorCode);
    sleep(1);
    system("clear");
    __utils_error_logger("The entered first name is too short", "get_student_first_name", MINOR);
    get_student_first_name();
  }
  else if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    printf("%sCancelling operation.%s\n", yellow.colorCode, reset.colorCode);
    __utils_runtime_logger("Cancelled operation", "get_student_first_name");
    globalTrigger.studentCreationInterrupted = TRUE;
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }
  else
  {
    system("clear");
    show_current_step("Confirm Student First Name", 2, 7);
    printf("You entered: " BOLD "%s%s is that correct[y/n]\n", userInput.StrInput, reset.colorCode);

    /*Taking the entered value and
    assigning it to a variable.*/
    char setFirstName[30];
    strcpy(setFirstName, userInput.StrInput);

    /*Getting the confirmation*/
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      /*Confirming the entered value and
      setting the struct members value to it */
      if (setFirstName[0] != '\0')
      {
        /* Ensuring the first letter of the first name
        is capitalized when it hits the database */
        setFirstName[0] = toupper(setFirstName[0]);
        strcpy(NewStudent.FirstName, setFirstName);
        __utils_runtime_logger("Confirmed students first name", "get_student_first_name");
        get_student_last_name();
        return 0;
      }
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      printf("Ok lets try again.");
      sleep(1);
      system("clear");
      __utils_runtime_logger("did not confirm students first name", "get_student_first_name");
      get_student_first_name();
      return 0;
    }
    else
    {
      system("clear");
      printf("Im sorry I didn't understand that please try again.");
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

  printf("Please enter the students last name.\n");
  printf("If the student does not have a last name please enter 'none'.\n");
  printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
  __utils_fgets_and_remove_newline(userInput.StrInput);
  __utils_runtime_logger("entered a students last name", "get_student_last_name");

  if (!has_one_non_space_char(userInput.StrInput))
  {
    printf("%sThe entered last name is too short. Please try again%s\n", yellow.colorCode, reset.colorCode);
    printf("%sIf the student does not have a last name please enter 'none'.%s\n", yellow.colorCode, reset.colorCode);
    sleep(2);
    system("clear");
    __utils_error_logger("The entered last name is too short", "get_student_last_name", MINOR);
    get_student_last_name();
  }
  else if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    printf("%sCancelling operation.%s\n", yellow.colorCode, reset.colorCode);
    globalTrigger.studentCreationInterrupted = TRUE;
    __utils_runtime_logger("Cancelled operation", "get_student_last_name");
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }
  else if (strcmp(userInput.StrInput, "none") == 0)
  {
    __utils_runtime_logger("Entered 'none' for students last name", "get_student_last_name");
    system("clear");
    show_current_step("Confirm Student Last Name", 4, 7);
    printf("%sThis student does not have a last name. is that correct?[y/n]%s\n", yellow.colorCode, reset.colorCode);

    /*Taking the entered value and
    assigning it to a variable.*/
    strcpy(setLastName, "");

    /*Getting the confirmation*/
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      /*Confirming the entered value and
      setting the struct members value to it */
      strcpy(NewStudent.LastName, setLastName);
      __utils_runtime_logger("Confirmed student does not have last name", "get_student_last_name");
      system("clear");
      ask_about_student_id();
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      printf("Ok lets try again.");
      __utils_runtime_logger("Did not confirm student does not have last name", "get_student_last_name");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
    else
    {
      system("clear");
      printf("Im sorry I didn't understand that please try again.");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
  }
  else
  {
    strcpy(setLastName, userInput.StrInput);
    system("clear");
    show_current_step("Confirm Student Last Name", 4, 7);
    printf("You entered: " BOLD "%s%s is that correct?[y/n]\n", setLastName, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      if (setLastName[0] != '\0')
      {
        /* Ensuring the first letter of the last name
        is capitalized when it hits the database */
        setLastName[0] = toupper(setLastName[0]);
        strcpy(NewStudent.LastName, setLastName);
        system("clear");
        __utils_runtime_logger("Confirmed students last name", "get_student_last_name");
        ask_about_student_id();
      }
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      printf("Ok lets try again.");
      sleep(1);
      system("clear");
      get_student_last_name();
    }
    else
    {
      system("clear");
      printf("Im sorry I didn't understand that please try again.");
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
  printf("How would you like to assign the student ID?\n");
  printf("Please enter either '1' or '2'.\n");
  printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
  printf("1. Automatically generate an ID \n");
  printf("2. Create your own\n");

  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.NumInput = atoi(userInput.StrInput);

  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    printf("%sCancelling operation.%s\n", yellow.colorCode, reset.colorCode);
    __utils_runtime_logger("Cancelled operation", "ask_about_student_id");
    globalTrigger.studentCreationInterrupted = TRUE;
    sleep(1);
    system("clear");
    addStudentMenuIsRunning = TRUE;
    return 0;
  }

  if (userInput.NumInput == 1)
  {
    system("clear");
    __utils_runtime_logger("Chose to automatically generate an ID", "ask_about_student_id");
    generate_student_id(NewStudent.FirstName, NewStudent.LastName);
  }
  else if (userInput.NumInput == 2)
  {
    system("clear");
    __utils_runtime_logger("Chose to manually set an ID", "ask_about_student_id");
    manually_set_student_id();
  }

  else
  {
    system("clear");
    printf("Please make a valid decision.");
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
    printf("Please enter the students id.\n");
    printf("%sNOTE: The id can be no MORE than 15 characters and LESS than 2.%s\n", yellow.colorCode, reset.colorCode);
    printf("%sIMPORTANT: It is not recommended to use private information such as social security numbers or birth dates as an id.%s\n", red.colorCode, reset.colorCode);
    printf("%sYou can cancel this operation by entering 'cancel.%s\n", yellow.colorCode, reset.colorCode);
    gettingStudentId = FALSE;
  }
  __utils_fgets_and_remove_newline(userInput.StrInput);
  __utils_runtime_logger("entered a students id", "manually_set_student_id");

  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    printf("%sCancelling operation.%s\n", yellow.colorCode, reset.colorCode);
    __utils_runtime_logger("Cancelled operation", "manually_set_student_id");
    globalTrigger.studentCreationInterrupted = TRUE;
    sleep(1);
    system("clear");
    gettingStudentId == FALSE;
    addStudentMenuIsRunning = TRUE;
  }
  else if (strlen(userInput.StrInput) < 2)
  {
    gettingStudentId == FALSE;
    printf("%sThe id you entered is too short. Please try again.%s\n", yellow.colorCode, reset.colorCode);
    sleep(1);
    system("clear");
    manually_set_student_id();
  }
  else if (strlen(userInput.StrInput) > 15)
  {
    gettingStudentId == FALSE;
    printf("%sThe id you entered is too long. Please try again.%s\n", yellow.colorCode, reset.colorCode);
    sleep(1);
    system("clear");
    manually_set_student_id();
  }
  else
  {
    gettingStudentId == FALSE;
    system("clear");
    char studentID[15];
    strcpy(studentID, userInput.StrInput);
    int result = check_if_student_id_exists(studentID, desiredTableName.TableName);
    if (result == FALSE)
    {
      confirm_manually_entered_student_id(studentID);
    }
    else if (result == TRUE)
    {
      system("clear");
      printf("%sDuplicate ID detected. Please try again.%s\n", yellow.colorCode, reset.colorCode);
      __utils_error_logger("Duplicate ID detected", "manually_set_student_id", MINOR);
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

  printf("Generating student ID");
  sleep(1);
  size_t lastNameLength = strlen(NewStudent.LastName);
  // if the student last name is empty
  if (strcmp(LastName, "") == 0)
  {

    snprintf(setStudentID, sizeof(setStudentID), "%c%c%d%d%d", toupper(FirstName[0]), toupper(FirstName[1]), digit1, digit2, digit3);
    printf("%sID successfully generated!%s\n", green.colorCode, reset.colorCode);
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

    printf("%sEntered last name:" BOLD " %s%s%s"
           " longer than 10 characters, this will be shortened to" BOLD " %s in the student ID %s\n",
           yellow.colorCode,
           NewStudent.LastName, reset.colorCode, yellow.colorCode, truncatedLastName);
    printf("Are you sure you want to continue with this students last name?[y/n]%s\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("Acknowledged student last name trucation", "generate_student_id");
      snprintf(setStudentID, sizeof(setStudentID), "%c%c%s%d%d%d", toupper(FirstName[0]), toupper(FirstName[1]), truncatedLastName, digit1, digit2, digit3);
      // TODO need to find a way to allow the second argument in the check_if_student_id_exists() function to be a passed in roster name or the name of the 'students' table. Need to have it so either or can be passed in when needed. Maybe a global variable that can be set to either or depending on the situation.
      int result = check_if_student_id_exists(setStudentID, desiredTableName.TableName);
      if (result == FALSE)
      {
        printf("%sID successfully generated!%s", green.colorCode, reset.colorCode);
        sleep(1);
        system("clear");
        sleep(2);
        confirm_generated_student_id(setStudentID);
      }
      else if (result == TRUE)
      {
        system("clear");
        sleep(1);
        printf("%sDuplicate ID detected. Regenerating....%s\n", yellow.colorCode, reset.colorCode);
        sleep(1);
        generate_student_id(FirstName, LastName);
      }
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      handle_last_name_truncated_menu();
    }
    else
    {
      system("clear");
      printf("Please make a valid decision.");
      sleep(1);
      system("clear");
      generate_student_id(FirstName, LastName);
    }
  }
  else
  {
    snprintf(setStudentID, sizeof(setStudentID), "%c%c%s%d%d%d", toupper(FirstName[0]), toupper(FirstName[1]), LastName, digit1, digit2, digit3);
    ;
    int result = check_if_student_id_exists(setStudentID, desiredTableName.TableName);
    if (result == FALSE)
    {
      printf("%sID successfully generated!%s\n", green.colorCode, reset.colorCode);
      sleep(1);
      system("clear");
      confirm_generated_student_id(setStudentID);
    }
    else if (result == TRUE)
    {
      system("clear");
      sleep(1);
      printf("%sDuplicate ID detected. Regenerating....%s\n", yellow.colorCode, reset.colorCode);
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
    printf("Is the ID: " BOLD "%s%s for" BOLD " %s %s%s satisfactory?[y/n]\n", studentID, reset.colorCode, NewStudent.FirstName, NewStudent.LastName, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    char setStudentID[15];
    strcpy(setStudentID, studentID);

    if (INPUT_IS_YES(userInput.StrInput))
    {
      confirmingStudentId = FALSE;
      show_current_step("Confirm Student Id", 6, 7);
      strcpy(NewStudent.StudentID, setStudentID);
      __utils_runtime_logger("Confirmed auto generated student id", "confirm_generated_student_id");
      system("clear");

      /*Doing this allows me to use the full or part of
      functions from this file in other places in the project.
      I could modify what I Wanted to show depending on the
      global trigger value. - Marshall Burns Jan 24 2024*/
      if (globalTrigger.isTriggered == FALSE)
      {
        ask_to_add_new_student_to_roster();
      }
    }

    else if (INPUT_IS_NO(userInput.StrInput))
    {
      confirmingStudentId = FALSE;
      system("clear");
      printf("Ok lets try again.");
      __utils_runtime_logger("Did not confirm auto generated student id", "confirm_generated_student_id");
      sleep(1);
      system("clear");
      generate_student_id(NewStudent.FirstName, NewStudent.LastName);
    }
    else
    {
      confirmingStudentId = FALSE;
      printf("Im sorry I didn't understand that please try again.");
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
    printf("You've entered: " BOLD "%s%s is that correct?[y/n]\n", studentID, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    char setStudentID[15];
    strcpy(setStudentID, studentID);

    if (INPUT_IS_YES(userInput.StrInput))
    {
      confirmingStudentId = FALSE;
      show_current_step("Confirm Student Id", 6, 7);
      strcpy(NewStudent.StudentID, setStudentID);
      __utils_runtime_logger("Confirmed manually entered student id", "confirm_manually_entered_student_id");
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
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      confirmingStudentId = FALSE;
      system("clear");
      printf("Ok lets try again.");
      __utils_runtime_logger("Did not confirm manually entered student id", "confirm_manually_entered_student_id");
      sleep(1);
      system("clear");
      manually_set_student_id();
    }
    else
    {
      confirmingStudentId = FALSE;
      printf("Im sorry I didn't understand that please try again.");
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
  __utils_runtime_logger("Asked to add new student to a roster", "ask_to_add_new_student_to_roster");
  show_current_step("Add Student To Roster", 7, 7);
  printf("Would you like to add this student to a roster?\n");
  printf("%sNOTE: You can choose to do this later as well%s\n", yellow.colorCode, reset.colorCode);
  printf("1: Yes");
  printf("2: No");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.NumInput = atoi(userInput.StrInput);
  if (userInput.NumInput == 1 || INPUT_IS_YES(userInput.StrInput))
  {
    __utils_runtime_logger("Chose to add new student to a roster", "ask_to_add_new_student_to_roster");
    ask_which_roster_to_add_newly_created_student();
    printf("%sAdding new student to database...%s\n", green.colorCode, reset.colorCode);
    sleep(2);
    system("clear");
    int result = insert_student_into_db(NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);

    if (result == 0)
    {
      printf("%sStudent successfully added to database.%s\n", green.colorCode, reset.colorCode);
      __utils_runtime_logger("Successfully added new student to database", "ask_to_add_new_student_to_roster");
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    else if (result == 1)
    {
      printf("Please try again.\n");
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
  }
  else if (userInput.NumInput == 2 || INPUT_IS_NO(userInput.StrInput))
  {
    __utils_runtime_logger("Chose not to add new student to a roster", "ask_to_add_new_student_to_roster");

    int result = insert_student_into_db(NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);
    if (result == 0)
    {
      printf("%sStudent successfully added to database.%s\n", green.colorCode, reset.colorCode);
      __utils_runtime_logger("Successfully added new student to database", "ask_to_add_new_student_to_roster");
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    else if (result == 1)
    {
      printf("Please try again.\n");
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
  }
  else
  {
    printf("Sorry, I didn't understand that.");
    printf("Please try again");
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
  __utils_runtime_logger("Asked which roster to add new student to", "ask_which_roster_to_add_newly_created_student");
  printf("Please enter the name of the roster you would like to add this student to.");
  system("clear");
  show_tables();
  __utils_fgets_and_remove_newline(userInput.StrInput);
  __utils_runtime_logger("Entered a roster name", "ask_which_roster_to_add_newly_created_student");
  sprintf(destinationRoster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);
  int rosterExists = check_if_table_exists(destinationRoster.rosterNameWithPrefix);
  if (rosterExists == TRUE)
  {
    int addedToRoster = add_student_to_roster(destinationRoster.rosterNameWithPrefix, NewStudent.FirstName, NewStudent.LastName, NewStudent.StudentID);
    if (addedToRoster == TRUE)
    {
      system("clear");
      printf("%sStudent successfully added to roster.%s\n", green.colorCode, reset.colorCode);
      sleep(1);
      system("clear");
      return 0;
    }
    else
    {
      system("clear");
      printf("%sFailed to add student to roster. Please try again.%s\n", red.colorCode, reset.colorCode);
      __utils_error_logger("Failed to add student to roster", "ask_which_roster_to_add_newly_created_student", MINOR);
    }
  }
  else if (rosterExists == FALSE)
  {
    system("clear");
    printf("%sThe entered name:" BOLD "%s%s does not exist. Please try again.\n", yellow.colorCode, userInput.StrInput, reset.colorCode);
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
  printf("Please enter the number of an option listed below?");
  printf("1. Enter a new last name");
  printf("2. Manually set an ID");
  printf("3. Cancel");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  userInput.NumInput = atoi(userInput.StrInput);
  if (userInput.NumInput == 1)
  {
    system("clear");
    get_student_last_name();
  }
  else if (userInput.NumInput == 2)
  {
    system("clear");
    manually_set_student_id();
  }
  else if (userInput.NumInput == 3)
  {
    system("clear");
    printf("%sCancelling operation%s\n", yellow.colorCode, reset.colorCode);
    globalTrigger.studentCreationInterrupted = TRUE;
    sleep(1);
    system("clear");
    add_student_to_db();
  }
  else
  {
    system("clear");
    printf("Please enter a valid decision.");
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
      printf("%sStudent successfully added to roster" BOLD " %s.%s\n", green.colorCode, rosterName, reset.colorCode);
      sleep(1);
      printf("%sStudent successfully added to database.%s\n", green.colorCode, reset.colorCode);
      sleep(1);
      system("clear");
      addStudentMenuIsRunning = TRUE;
      return 0;
    }
    else if (addedToStudentDB == 1)
    {
      printf("Please try again.\n");
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
    printf("%sFailed to add student to roster. Please try again.%s\n", red.colorCode, reset.colorCode);
    __utils_error_logger("Failed to add student to roster", "skip_and_add_to_roster", MINOR);
  }
}
