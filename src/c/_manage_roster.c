#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.h"

void manage_roster()
{
  char buffer[50];
  int mainMenuProccess;
  system("clear");
  int manageRosterMenuIsOpen = TRUE;
  int menuWidth = 70;
  int menuHeight = 10;
  while (manageRosterMenuIsOpen == TRUE)
  {
    mainMenuProccess = FALSE;
    char manageRosterOptions[5][50] = {
        "1. View roster",
        "2: Rename roster",
        "3. Add student to roster",
        "4. Remove student from roster",
        "5. Back to main menu"};

    show_current_menu("Manage Roster");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 5; ++i)
    {
      printf("| %s %-90s\n", manageRosterOptions[i], "");
    }
    for (int i = 0; i < menuHeight; ++i)
    {
      printf("| %-90s\n", "");
    }
    puts("|===========================================================================================");

    FGETS(buffer);
    UTILS_REMOVE_NEWLINE_CHAR(buffer);

    int menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "view roster") == 0)
    {
      puts("You selected to view the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("viewing roster");
    }
    else if (menuInput == 2 || strcmp(buffer, "rename roster") == 0)
    {
      puts("You selected to rename the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("renaming roster");
    }
    else if (menuInput == 3 || strcmp(buffer, "add student to roster") == 0)
    {
      puts("You selected to add a student to the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("adding student to roster");
    }
    else if (menuInput == 4 || strcmp(buffer, "remove student from roster") == 0)
    {
      puts("You selected to remove a student from the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("removing student from roster");
    }
    else if (menuInput == 5 || strcmp(buffer, "back") == 0 || strcmp(buffer, "exit") == 0)
    {
      puts("You selected to go back to the main menu.");
      system("clear");
      manageRosterMenuIsOpen = FALSE;
    }
    else
    {
      puts("Sorry, I didn't understand that.");
      puts("Please try again");
      manage_roster();
    }
  }
}