/*
===============================================================================
File Name   : _manage_roster.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to manage a roster.
-------------------------------------------------------------------------------
Helper functions from db.hh:
  - show_tables() - shows all tables in the database
  - rename_roster() - renames a roster
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "../lib/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

int manage_roster()
{

  char buffer[50];
  int menuInput;
  int mainMenuProccess;
  int manageRosterMenuIsOpen = TRUE;
  int showingFoundRosters = FALSE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;

  system("clear");
  while (manageRosterMenuIsOpen == TRUE)
  {
    mainMenuProccess = FALSE;
    char manageRosterOptions[7][50] = {
        "1. View roster",
        "2: Rename roster",
        "3: Delete roster",
        "4. Add student to roster",
        "5. Remove student from roster",
        "6. Help",
        "7. Main menu"};

    show_current_menu("Manage Roster");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 7; ++i)
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
    if (menuInput == 1 || strcmp(buffer, "view roster") == 0 || strcmp(buffer, "view") == 0)
    {
      puts("You selected to view the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");

      int tableExists = get_table_count();
      if (tableExists == TRUE)
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
      else if (tableExists == FALSE)
      {
        system("clear");
        printf(YELLOW "No rosters found\n" RESET);
        puts("Please create a new roster");
        sleep(1);
        system("clear");
        manageRosterMenuIsOpen = TRUE;
      }
      else
      {
        UTILS_ERROR_LOGGER("Failed to get table count", "create_new_roster", MINOR);
        printf(RED "Error: Failed to get table count\n" RESET);
      }
    }
    else if (menuInput == 2 || strcmp(buffer, "rename roster") == 0 || strcmp(buffer, "rename") == 0)
    {
      puts("You selected to rename the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("renaming roster");
      puts("Which roster would you like to rename?");
      show_tables();
      rename_roster();
    }
    else if (menuInput == 3 || strcmp(buffer, "delete roster") == 0 || strcmp(buffer, "delete") == 0)
    {
      puts("You selected to add a student to the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("adding student to roster");
    }
    else if (menuInput == 4 || strcmp(buffer, "add student") == 0 || strcmp(buffer, "add") == 0)
    {
      puts("You selected to add a student to the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("adding student to roster");
    }
    else if (menuInput == 5 || strcmp(buffer, "remove student") == 0 || strcmp(buffer, "remove") == 0)
    {
      puts("You selected to remove a student from the roster.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("removing student from roster");
    }
    else if (menuInput == 6 || strcmp(buffer, "help") == 0)
    {
      puts("You selected to to get help.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("showing help");
    }

    else if (menuInput == 7 || strcmp(buffer, "main menu") == 0 || strcmp(buffer, "main") == 0)
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
      manage_roster();
    }
  }
}