/*
===============================================================================
File Name   : _create_roster.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              for adding rosters in the database
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

/************************************************************************************
 * create_new_roster() This is the main function for creating new rosters.
 * Note: See usage in ./main.c
 * Note: Uses the following helper functions from db.hpp:
 *  - show_tables() - shows all tables in the database
 *  - get_table_count() - the return value is used to determine if there are any
 ************************************************************************************/
int create_new_roster(void)
{
  system("clear");
  uint8_t newRosterMenuIsRunning = TRUE;
  uint8_t showingFoundRosters = FALSE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;
  __utils_runtime_logger("User entered the create new roster menu", "create_new_roster");
  __utils_check_for_sqlite_db();
  while (newRosterMenuIsRunning == TRUE)
  {
    char newRosterOptions[4][50] = {
        "1. Create a new roster",
        "2. View created rosters",
        "3. Help",
        "4. Main Menu"};
    show_current_menu("Create New Roster");
    printf("What would you like to do?\n");
    printf("|===========================================================================================\n");
    for (int i = 0; i < 4; ++i)
    {
      printf("| %s %-90s\n", newRosterOptions[i], "");
    }
    for (int i = 0; i < menuHeight; ++i)
    {
      printf("| %-90s\n", "");
    }
    printf("|===========================================================================================\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);
    userInput.NumInput = atoi(userInput.StrInput);
    if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "new roster") == 0 || strcmp(userInput.StrInput, "new") == 0)
    {
      __utils_runtime_logger("selected to create a new roster", "create_new_roster");
      printf("You selected to create a new roster.\n");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      get_and_confirm_roster_name();
    }
    else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "view rosters") == 0 || strcmp(userInput.StrInput, "view") == 0)
    {
      __utils_runtime_logger("selected to view created rosters", "create_new_roster");
      printf("You selected to view created rosters.\n");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      int tablesExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tablesExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          system("clear");
          ("%sSuccessfully found created roster(s)%s\n", green.colorCode, reset.colorCode);
          sleep(1);
          system("clear");
          show_tables();
          showingFoundRosters = FALSE;
          printf("What would you like to do?\n");
          printf("1: Back\n");
          printf("2: Main Menu\n");

          __utils_fgets_and_remove_newline(userInput.StrInput);
          userInput.NumInput = atoi(userInput.StrInput);
          if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "back") == 0)
          {
            __utils_runtime_logger("selected to go back to create roster menu", "create_new_roster");
            showingFoundRosters = FALSE;
            system("clear");
            printf("Going back to create roster menu\n");
            sleep(1);
            system("clear");
            create_new_roster();
          }
          else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "main") == 0 || strcmp(userInput.StrInput, "main menu") == 0)
          {
            __utils_runtime_logger("selected to go back to main menu", "create_new_roster");
            showingFoundRosters = FALSE;
            system("clear");
            printf("Returning to the main menu\n");
            sleep(1);
            system("clear");

            return 0;
          }
          else
          {
            system("clear");
            printf("Please make a valid decision\n");
            sleep(1);
            system("clear");
            showingFoundRosters = TRUE;
          }
        }
      }
      else if (tablesExists == FALSE)
      {
        system("clear");
        printf("%sNo rosters found%s\n", yellow.colorCode, reset.colorCode);
        printf("Please create a new roster");
        sleep(1);
        system("clear");
        newRosterMenuIsRunning = TRUE;
      }
      else
      {
        __utils_error_logger("Failed to get table count", "create_new_roster", MINOR);
        wait_for_char_input();
      }
    }

    else if (userInput.NumInput == 3 || strcmp(userInput.StrInput, "help") == 0)
    {
      __utils_runtime_logger("Selected to view create roster help menu", "create_new_roster");
      printf("You selected to get help.\n");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      // show_help_menu();
    }
    else if (userInput.NumInput == 4 || strcmp(userInput.StrInput, "main") == 0 || strcmp(userInput.StrInput, "main menu") == 0)
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
      printf("Invalid input please try again.\n");
      sleep(1);
      printf("Please try again\n");
      system("clear");
      create_new_roster();
    }
  }
}

/************************************************************************************
 * add_student_to_db():  . Gets and confirms the roster name.
 * Note: see usage in create_new_roster()
 ************************************************************************************/
int get_and_confirm_roster_name(void)
{
  show_current_step("Name your new roster", 1, 2);
  char rosterNameInput[30];
  char rosterNameWithPrefix[60];
  // getting initial input
  printf("What would you like to name your new roster?\n");
  printf("Roster names can be no less the 1 character and no more then 30 characters.\n");
  printf("%sTo cancel this operation enter" BOLD "'cancel'%s.\n", yellow.colorCode, reset.colorCode);

  __utils_fgets_and_remove_newline(userInput.StrInput);
  strcpy(rosterNameInput, userInput.StrInput);
  if (strlen(rosterNameInput) > 30)
  {
    printf("%sSorry that name%s is too long please try again.\n" RESET);
    sleep(1);
    system("clear");
    __utils_error_logger("Entered roster name is too long", "get_and_confirm_roster_name", MINOR);
    __utils_clear_input_buffer();
    get_and_confirm_roster_name();
  }
  else if (!has_one_non_space_char(rosterNameInput))
  {
    printf("%sSorry that name%s is too short please try again.\n" RESET);
    sleep(1);
    system("clear");
    __utils_error_logger("Entered roster name is too short", "get_and_confirm_roster_name", MINOR);
    // No need to clear input userInput.StrInput here because the user didn't enter anything
    get_and_confirm_roster_name();
  }
  else if (INPUT_IS_CANCEL(rosterNameInput))
  {
    __utils_operation_cancelled("get_and_confirm_roster_name");
    create_new_roster();
  }
  else
  {
    system("clear");
    show_current_step("Confirm new roster name", 2, 2);
    printf("You have decided to name your new roster:" BOLD "%s %s.\nIs that correct?[y/n]\n", rosterNameInput, reset.colorCode);
    // confirming input
    __utils_fgets_and_remove_newline(userInput.StrInput);
    char confirmation[20];
    strcpy(confirmation, userInput.StrInput);
    __utils_runtime_logger("confirmed roster name", "get_and_confirm_roster_name");
    if (INPUT_IS_YES(confirmation))
    {
      system("clear");
      sprintf(rosterNameWithPrefix, "Roster_%s", rosterNameInput);
      int table_exists = check_if_table_exists(rosterNameWithPrefix);
      if (table_exists == FALSE)
      {
        int result = create_new_roster_table(rosterNameWithPrefix);
        if (result == 0)
        {
          printf("Creating new roster:" BOLD " %s.\n" RESET, rosterNameInput);
          printf("%sSuccessfully created new roster: %s %s\n", green.colorCode, rosterNameInput, reset.colorCode);
          sleep(3);
          system("clear");
          create_new_roster();
        }
        else
        {
          printf("%sError: Failed to create new roster: %s\n", red.colorCode, rosterNameInput);
          printf("Please try again\n");
          sleep(3);
          system("clear");
          create_new_roster();
        }
      }
      else if (table_exists == TRUE)
      {
        printf("%sRoster:" BOLD "%s%s%s already exists%s\n", yellow.colorCode, rosterNameInput, reset.colorCode, yellow.colorCode, reset.colorCode);
        printf("Please try again\n");
        sleep(3);
        system("clear");
        create_new_roster();
      }
    }
    else if (INPUT_IS_NO(confirmation))
    {
      __utils_runtime_logger("Did not confirm roster name", "get_and_confirm_roster_name");
      system("clear");
      show_current_step("Name your new roster", 2, 2);
      printf("The name " BOLD "%s was not correct.\n", rosterNameInput, reset.colorCode);
      printf("What would you like to do?\n");
      // todo dont print this out...re-write this
      printf("1: Try again\n");
      printf("2: Help\n");
      printf("3: Back\n");
      printf("4: Main Menu\n");

      __utils_fgets_and_remove_newline(userInput.StrInput);
      userInput.NumInput = atoi(userInput.StrInput);
      if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "try again") == 0)
      {
        system("clear");
        __utils_runtime_logger("Selected to try again", "get_and_confirm_roster_name");
        printf("Ok please try again.\n");
        sleep(1);
        system("clear");
        get_and_confirm_roster_name();
      }
      else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "help") == 0)
      {
        system("clear");
        sleep(1);
        // do stuff with help menu
      }
      else if (userInput.NumInput == 3 || strcmp(userInput.StrInput, "back") == 0)
      {
        system("clear");
        printf("Going back to previous menu.\n");
        sleep(1);
        create_new_roster();
      }
      else if (userInput.NumInput == 4 || strcmp(userInput.StrInput, "main") == 0 || strcmp(userInput.StrInput, "main menu") == 0)
      {
        system("clear");
        printf("Returning to main menu.\n");
        sleep(1);
        system("clear");
        return 0;
      }
      else
      {
        printf("Invalid input please try again.\n");
        sleep(1);
        __utils_error_logger("Entered invalid input", "get_and_confirm_roster_name", MINOR);
        system("clear");
        get_and_confirm_roster_name();
      }
    }
    else
    {
      printf("I did'nt understand that please try again\n");
      sleep(1);
      __utils_error_logger("Input entered was neither yes nor no", "get_and_confirm_roster_name", MINOR);
      system("clear");
      get_and_confirm_roster_name();
    }
  }
}
