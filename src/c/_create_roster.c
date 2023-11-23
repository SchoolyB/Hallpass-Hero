/*
===============================================================================
File Name   : _create_roster.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              for creating a new roster.
-------------------------------------------------------------------------------
Helper function from db.hh:
  - create_new_roster_table() - creates a new roster table
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../lib/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

char buffer[50];
int menuInput;

int create_new_roster(void)
{

  int mainMenuProccess;
  int newRosterMenuIsRunning = TRUE;
  int showingFoundRosters = FALSE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;

  system("clear");
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
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    int menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "new roster") == 0 || strcmp(buffer, "new") == 0)
    {
      puts("You selected to create a new roster.");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      get_and_confirm_roster_name();
    }
    else if (menuInput == 2 || strcmp(buffer, "view rosters") == 0 || strcmp(buffer, "view") == 0)
    {
      puts("You selected to view created rosters.");
      newRosterMenuIsRunning = FALSE;
      system("clear");

      int tablesExists = get_table_count();

      if (tablesExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          system("clear");
          printf(GREEN "Successfully found created roster(s)\n" RESET);
          printf("==========================================================================================\n");
          printf(BOLD "Created rosters:\n" RESET);
          puts("------------------------------------------------------------------------------------------");
          show_tables();
          printf("==========================================================================================\n");
          showingFoundRosters = FALSE;
          puts("What would you like to do?");
          puts("1: Back");
          puts("2: Main Menu");

          FGETS(buffer);
          UTILS_REMOVE_NEWLINE_CHAR(buffer);
          menuInput = atoi(buffer);
          if (menuInput == 1 || strcmp(buffer, "back") == 0)
          {
            showingFoundRosters = FALSE;
            puts("Going back to create roster menu");
            create_new_roster();
          }
          else if (menuInput == 2 || strcmp(buffer, "main") == 0 || strcmp(buffer, "main menu") == 0)
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
      }
    }

    else if (menuInput == 3 || strcmp(buffer, "help") == 0)
    {
      puts("You selected to get help.");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      // show_help_menu();
    }
    else if (menuInput == 4 || strcmp(buffer, "main") == 0 || strcmp(buffer, "main menu") == 0)
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

int get_and_confirm_roster_name(void)
{
  show_current_step("Name your new roster", 1, 2);
  char rosterNameInput[30];
  char newRosterName[30];
  // getting initial input
  puts("What would you like to name your new roster?");
  puts("Hint: Roster names can be no less the 1 character and no more then 30 characters.");
  puts("Hint: You can cancel this operation at any time by typing 'cancel'.");

  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);
  strcpy(rosterNameInput, buffer);
  if (strlen(rosterNameInput) > 30)
  {
    printf(YELLOW "Sorry that name is too long please try again.\n" RESET);
    sleep(1);
    system("clear");
    UTILS_ERROR_LOGGER("Entered roster name is too long", "get_and_confirm_roster_name", MINOR);
    UTILS_CLEAR_INPUT_BUFFER();
    get_and_confirm_roster_name();
  }
  else if (strlen(rosterNameInput) < 1 || strcmp(rosterNameInput, "") == 0)
  {
    printf(YELLOW "Sorry that name is too short please try again.\n" RESET);
    sleep(1);
    system("clear");
    UTILS_ERROR_LOGGER("Entered roster name is too short", "get_and_confirm_roster_name", MINOR);
    // No need to clear input buffer here because the user didn't enter anything
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
    printf("You have decided to name your new roster:" BOLD "%s " RESET ".\nIs that correct?(y/n)\n", rosterNameInput);

    // confirming input
    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);
    char confirmation[20];
    strcpy(confirmation, buffer);
    if (INPUT_IS_YES(confirmation))
    {
      system("clear");
      strcpy(newRosterName, rosterNameInput);
      printf("Creating new roster:" BOLD " %s.\n" RESET, rosterNameInput);
      create_new_roster_table(rosterNameInput);
      sleep(1);
      int result = create_new_roster_table(rosterNameInput);
      if (result == 0)
      {
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

      FGETS(buffer);
      UTILS_REMOVE_NEWLINE_CHAR(buffer);

      int menuInput = atoi(buffer);
      if (menuInput == 1 || strcmp(buffer, "try again") == 0)
      {
        system("clear");
        puts("Ok please try again.");
        sleep(1);
        system("clear");
        get_and_confirm_roster_name();
      }
      else if (menuInput == 2 || strcmp(buffer, "help") == 0)
      {
        system("clear");
        sleep(1);
        // do stuff with help menu
      }
      else if (menuInput == 3 || strcmp(buffer, "back") == 0)
      {
        system("clear");
        puts("Going back to previous menu.");
        sleep(1);
        create_new_roster();
      }
      else if (menuInput == 4 || strcmp(buffer, "main") == 0 || strcmp(buffer, "main menu") == 0)
      {
        system("clear");
        puts("Returning to main menu.");
        sleep(1);
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
