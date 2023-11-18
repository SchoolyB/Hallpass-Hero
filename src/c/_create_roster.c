#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.h"

void create_new_roster()
{
  char buffer[50];
  int mainMenuProccess;
  system("clear");

  int newRosterMenuIsRunning = TRUE;
  int menuWidth = 70;
  int menuHeight = 10;

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
      // todo need to do more work on db stuff
      init_db();
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