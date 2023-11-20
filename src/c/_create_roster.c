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
#include "../lib/headers/db.hh"
#include "../lib/headers/c_files.h"

char buffer[50];
int menuInput;
void create_new_roster()
{

  int mainMenuProccess;
  system("clear");

  int newRosterMenuIsRunning = TRUE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;

  while (newRosterMenuIsRunning == TRUE)
  {
    mainMenuProccess = FALSE;
    char newRosterOptions[2][50] = {
        "1. Create a new roster",
        "2. Back to main menu"};
    show_current_menu("Create New Roster");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 2; ++i)
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
    if (menuInput == 1 || strcmp(buffer, "new roster") == 0)
    {
      puts("You selected to create a new roster.");
      newRosterMenuIsRunning = FALSE;
      system("clear");
      get_and_confirm_roster_name();
    }
    else if (menuInput == 2 || strcmp(buffer, "back") == 0 || strcmp(buffer, "exit") == 0)
    {
      puts("You selected to go back to the main menu.");
      system("clear");
      newRosterMenuIsRunning = FALSE;
    }
    else
    {
      puts("Sorry, I didn't understand that.");
      puts("Please try again");
      create_new_roster();
    }
  }
}

void get_and_confirm_roster_name()
{
  show_current_step("Name your new roster", 1, 2);
  char rosterNameInput[30];
  char newRosterName[30];
  // getting initial input
  puts("What would you like to name your new roster?");
  puts("Roster names can be no less the 1 character and no more then 30 characters.");
  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);
  strcpy(rosterNameInput, buffer);
  if (strlen(rosterNameInput) > 30)
  {
    printf(YELLOW "Sorry that name is too long please try again.\n" RESET);
    sleep(2);
    system("clear");
    UTILS_ERROR_LOGGER("Entered roster name is too long", "get_and_confirm_roster_name", MINOR);
    UTILS_CLEAR_INPUT_BUFFER();
    get_and_confirm_roster_name();
  }
  else if (strlen(rosterNameInput) < 1 || strcmp(rosterNameInput, "") == 0)
  {
    printf(YELLOW "Sorry that name is too short please try again.\n" RESET);
    sleep(2);
    system("clear");
    UTILS_ERROR_LOGGER("Entered roster name is too short", "get_and_confirm_roster_name", MINOR);
    // No need to clear input buffer here because the user didn't enter anything
    get_and_confirm_roster_name();
  }
  show_current_step("Confirm new roster name", 2, 2);
  printf("You have decided to name your new roster:" BOLD "%s " RESET ".\nIs that correct?(y/n)\n", rosterNameInput);

  // confirming input
  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);
  if (INPUT_IS_YES(buffer))
  {

    strcpy(newRosterName, rosterNameInput);
    printf("Ok lets name the new roster:" BOLD " %s" RESET ".\n", rosterNameInput);
    create_new_roster_table(rosterNameInput);
  }
  else if (INPUT_IS_NO(buffer))
  {
    puts("Ok since that isn't correct what would you like to do?");
    puts("1: Try again - Enter 1 or 'try again'.");
    puts("2: Get help - Enter 2 or 'help'. ");
    puts("3: Go back to the previous menu");
    puts("4: Exit to the main menu");

    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);

    int menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "try again") == 0)
    {
      puts("Ok lets try again.");
      get_and_confirm_roster_name();
    }
    else if (menuInput == 2 || strcmp(buffer, "help") == 0)
    {
      // do stuff with help menu
      puts("You asked for help....");
    }
    else if (menuInput == 3 || strcmp(buffer, "back") == 0)
    {
      create_new_roster();
    }
    else if (menuInput == 4 || strcmp(buffer, "exit") == 0)
    {
      puts("You selected to go back to the main menu.");
      system("clear");
    }
    else
    {
      puts("I did'nt understand that please try again");
      get_and_confirm_roster_name();
    }
  }
  else
  {
    puts("I did'nt understand that please try again");
    get_and_confirm_roster_name();
  }
}