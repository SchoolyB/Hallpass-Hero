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
#include "../lib/utils.h"
#include "../lib/headers/db.hh"

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

  char rosterNameInput[30];
  char newRosterName[30];
  // getting initial input
  puts("What would you like to name your new roster?");
  FGETS(buffer);
  UTILS_REMOVE_NEWLINE_CHAR(buffer);
  printf("You have decided to name your new roster:" BOLD "%s " RESET ".\nIs that correct?(y/n)\n", buffer);
  strcpy(rosterNameInput, buffer);
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