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

UserInput createRosterInput;
static uint8_t menuInput;
static uint8_t mainMenuProccess;
/************************************************************************************
 * create_new_roster() This is the main function for creating new rosters.
 * Note: See usage in ./main.c
 * Note: Uses the following helper functions from db.hpp:
 *  - show_tables() - shows all tables in the database
 *  - get_table_count() - the return value is used to determine if there are any
 ************************************************************************************/
int create_new_roster(void)
{
  uint8_t newRosterMenuIsRunning = TRUE;
  uint8_t showingFoundRosters = FALSE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;

  while (newRosterMenuIsRunning == TRUE)
  {
    mainMenuProccess = FALSE;
    char newRosterOptions[4][50] = {
        "1. Create a new roster",
        "2. View created rosters",
        "3. Help",
        "4. Main Menu"};
    show_current_menu("Create New Roster");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 4; ++i)
    {
      printf("| %s %-90s\n", newRosterOptions[i], "");
    }
    for (int i = 0; i < menuHeight; ++i)
    {
      printf("| %-90s\n", "");
    }
    puts("|===========================================================================================");
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(createRosterInput.StrInput);
    menuInput = atoi(createRosterInput.StrInput);
    if (menuInput == 1 || strcmp(createRosterInput.StrInput, "new roster") == 0 || strcmp(createRosterInput.StrInput, "new") == 0)
    {
      puts("You selected to create a new roster.");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      get_and_confirm_roster_name();
    }
    else if (menuInput == 2 || strcmp(createRosterInput.StrInput, "view rosters") == 0 || strcmp(createRosterInput.StrInput, "view") == 0)
    {
      puts("You selected to view created rosters.");
      newRosterMenuIsRunning = FALSE;
      system("clear");

      int tablesExists = get_table_count("../build/db.sqlite");

      if (tablesExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          system("clear");
          printf(GREEN "Successfully found created roster(s)\n" RESET);
          sleep(1);
          system("clear");
          show_tables();
          showingFoundRosters = FALSE;
          puts("What would you like to do?");
          puts("1: Back");
          puts("2: Main Menu");

          UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(createRosterInput.StrInput);
          menuInput = atoi(createRosterInput.StrInput);
          if (menuInput == 1 || strcmp(createRosterInput.StrInput, "back") == 0)
          {
            showingFoundRosters = FALSE;
            system("clear");
            puts("Going back to create roster menu");
            sleep(1);
            system("clear");
            create_new_roster();
          }
          else if (menuInput == 2 || strcmp(createRosterInput.StrInput, "main") == 0 || strcmp(createRosterInput.StrInput, "main menu") == 0)
          {
            showingFoundRosters = FALSE;
            system("clear");
            puts("Returning to the main menu");
            sleep(1);
            system("clear");

            return 0;
          }
          else
          {
            system("clear");
            puts("Please make a valid decision");
            sleep(1);
            system("clear");
            showingFoundRosters = TRUE;
          }
        }
      }
      else if (tablesExists == FALSE)
      {
        system("clear");
        printf(YELLOW "No rosters found\n" RESET);
        puts("Please create a new roster");
        sleep(1);
        system("clear");
        newRosterMenuIsRunning = TRUE;
      }
      else
      {
        UTILS_ERROR_LOGGER("Failed to get table count", "create_new_roster", MINOR);
        printf(RED "Error: Failed to get table count\n" RESET);
        wait_for_char_input();
      }
    }

    else if (menuInput == 3 || strcmp(createRosterInput.StrInput, "help") == 0)
    {
      puts("You selected to get help.");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      // show_help_menu();
    }
    else if (menuInput == 4 || strcmp(createRosterInput.StrInput, "main") == 0 || strcmp(createRosterInput.StrInput, "main menu") == 0)
    {

      system("clear");
      puts("Returning to main menu.");
      sleep(1);
      system("clear");
      return 0;
    }
    else
    {
      puts("Sorry, I didn't understand that.");
      puts("Please try again");
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
  puts("What would you like to name your new roster?");
  puts("Hint: Roster names can be no less the 1 character and no more then 30 characters.");
  puts("Hint: You can cancel this operation at any time by typing 'cancel'.");

  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(createRosterInput.StrInput);
  strcpy(rosterNameInput, createRosterInput.StrInput);
  if (strlen(rosterNameInput) > 30)
  {
    printf(YELLOW "Sorry that name is too long please try again.\n" RESET);
    sleep(1);
    system("clear");
    UTILS_ERROR_LOGGER("Entered roster name is too long", "get_and_confirm_roster_name", MINOR);
    UTILS_CLEAR_INPUT_BUFFER();
    get_and_confirm_roster_name();
  }
  else if (!has_one_non_space_char(rosterNameInput))
  {
    printf(YELLOW "Sorry that name is too short please try again.\n" RESET);
    sleep(1);
    system("clear");
    UTILS_ERROR_LOGGER("Entered roster name is too short", "get_and_confirm_roster_name", MINOR);
    // No need to clear input createRosterInput.StrInput here because the user didn't enter anything
    get_and_confirm_roster_name();
  }
  else if (strcmp(rosterNameInput, "cancel") == 0)
  {
    system("clear");
    printf(YELLOW "Canceling roster creation\n" RESET);
    sleep(1);
    system("clear");
    create_new_roster();
  }
  else
  {
    system("clear");
    show_current_step("Confirm new roster name", 2, 2);
    printf("You have decided to name your new roster:" BOLD "%s " RESET ".\nIs that correct?[y/n]\n", rosterNameInput);

    // confirming input
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(createRosterInput.StrInput);
    char confirmation[20];
    strcpy(confirmation, createRosterInput.StrInput);
    if (INPUT_IS_YES(confirmation))
    {
      system("clear");
      sprintf(rosterNameWithPrefix, "Roster_%s", rosterNameInput);
      printf("Checking if " BOLD "%s" RESET " exists...\n", rosterNameInput);
      sleep(1);
      int table_exists = check_if_table_exists(rosterNameWithPrefix);
      if (table_exists == FALSE)
      {
        int result = create_new_roster_table(rosterNameWithPrefix);
        if (result == 0)
        {
          printf("Creating new roster:" BOLD " %s.\n" RESET, rosterNameInput);
          printf(GREEN "Successfully created new roster: %s\n" RESET, rosterNameInput);
          sleep(3);
          system("clear");
          create_new_roster();
        }
        else
        {
          printf(RED "Error: Failed to create new roster: %s\n" RESET, rosterNameInput);
          puts("Please try again");
          sleep(3);
          system("clear");
          create_new_roster();
        }
      }
      else if (table_exists == TRUE)
      {
        printf(YELLOW "Roster: %s already exists\n" RESET, rosterNameInput);
        puts("Please try again");
        sleep(3);
        system("clear");
        create_new_roster();
      }
    }
    else if (INPUT_IS_NO(confirmation))
    {
      system("clear");
      show_current_step("Name your new roster", 2, 2);
      printf("The name " BOLD "%s " RESET "was not correct.\n", rosterNameInput);
      puts("What would you like to do?");
      puts("1: Try again");
      puts("2: Help");
      puts("3: Back");
      puts("4: Main Menu");

      UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(createRosterInput.StrInput);
      menuInput = atoi(createRosterInput.StrInput);
      if (menuInput == 1 || strcmp(createRosterInput.StrInput, "try again") == 0)
      {
        system("clear");
        puts("Ok please try again.");
        sleep(1);
        system("clear");
        get_and_confirm_roster_name();
      }
      else if (menuInput == 2 || strcmp(createRosterInput.StrInput, "help") == 0)
      {
        system("clear");
        sleep(1);
        // do stuff with help menu
      }
      else if (menuInput == 3 || strcmp(createRosterInput.StrInput, "back") == 0)
      {
        system("clear");
        puts("Going back to previous menu.");
        sleep(1);
        create_new_roster();
      }
      else if (menuInput == 4 || strcmp(createRosterInput.StrInput, "main") == 0 || strcmp(createRosterInput.StrInput, "main menu") == 0)
      {
        system("clear");
        puts("Returning to main menu.");
        sleep(1);
        system("clear");
        return 0;
      }
      else
      {
        puts("Invalid input please try again.");
        sleep(1);
        UTILS_ERROR_LOGGER("Entered invalid input", "get_and_confirm_roster_name", MINOR);
        system("clear");
        get_and_confirm_roster_name();
      }
    }
    else
    {
      puts("I did'nt understand that please try again");
      sleep(1);
      UTILS_ERROR_LOGGER("Input entered was neither yes nor no", "get_and_confirm_roster_name", MINOR);
      system("clear");
      get_and_confirm_roster_name();
    }
  }
}
