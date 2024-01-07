/*
===============================================================================
File Name   : _manage_roster.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains the function used
              to manage a roster.
===============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdarg.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/db.hpp"
#include "../lib/headers/c_files.h"

static char buffer[50];
static uint8_t menuInput;
static uint8_t mainMenuProccess;
/************************************************************************************
 * manage_roster(): This is the main function for managing a roster.
 * Note: see usage in ./main.c
 * Note: Uses the following helper functions from db.hpp:
 * - show_tables() - shows all tables in the database
 * - rename_roster() - renames a roster
 * - drop_table() - deletes a roster
 * - get_table_count() - the return value is used to determine if there are any
 *                      rosters in the database
 ************************************************************************************/
int manage_roster(void)
{
  uint8_t manageRosterMenuIsOpen = TRUE;
  uint8_t showingFoundRosters = FALSE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;

  system("clear");
  while (manageRosterMenuIsOpen == TRUE)
  {
    mainMenuProccess = FALSE;
    char manageRosterOptions[10][50] = {
        "1. View roster",
        "2: Rename roster",
        "3: Delete roster",
        "4. Add student to roster",
        "5. Remove student from roster",
        "6. Create a column",
        "7. Delete a column",
        "99. Help",
        "100. Main menu"};

    show_current_menu("Manage Roster");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 8; ++i)
    {
      printf("| %s %-90s\n", manageRosterOptions[i], "");
    }
    for (int i = 0; i < menuHeight; ++i)
    {
      printf("| %-90s\n", "");
    }
    puts("|===========================================================================================");

    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);

    menuInput = atoi(buffer);
    if (menuInput == 1 || strcmp(buffer, "view roster") == 0 || strcmp(buffer, "view") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");

      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
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

          UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
          menuInput = atoi(buffer);
          if (menuInput == 1 || strcmp(buffer, "back") == 0)
          {
            system("clear");
            showingFoundRosters = FALSE;
            manageRosterMenuIsOpen = TRUE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (menuInput == 2 || strcmp(buffer, "rename roster") == 0 || strcmp(buffer, "rename") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("rename");
          showingFoundRosters = FALSE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (menuInput == 3 || strcmp(buffer, "delete roster") == 0 || strcmp(buffer, "delete") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("delete");
          showingFoundRosters = FALSE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (menuInput == 4 || strcmp(buffer, "add student") == 0 || strcmp(buffer, "add") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("add student to roster");
          showingFoundRosters = FALSE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (menuInput == 5 || strcmp(buffer, "remove student") == 0 || strcmp(buffer, "remove") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("remove student from roster");
          showingFoundRosters = FALSE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (menuInput == 6 || strcmp(buffer, "add column") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("Create a column");
          showingFoundRosters = FALSE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }

    else if (menuInput == 7 || strcmp(buffer, "delete a column") == 0)
    {

      manageRosterMenuIsOpen = FALSE;
      system("clear");
      int tableExists = get_table_count("../build/db.sqlite");
      if (tableExists == TRUE)
      {
        showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("delete a column");
          showingFoundRosters = FALSE;
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
        UTILS_ERROR_LOGGER("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }

    else if (menuInput == 99 || strcmp(buffer, "help") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      puts("You selected to to get help.");
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("showing help");
      // do stuff
    }
    else if (menuInput == 100 || strcmp(buffer, "main menu") == 0 || strcmp(buffer, "main") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
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
// action to be preformed on the roster
int ask_which_roster_and_preform_action(char *action)
{
  char rosterNameWithPrefix[30];
  char oldRosterName[30];
  char buffer[30];
  printf("Enter the name of the roster that you would like to %s?\n", action);
  // TODO handle canceling operation printf(YELLOW "To cancel this operation enter" BOLD " 'cancel'. \n" RESET);
  if (strcmp(action, "rename") == 0)
  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    strcpy(oldRosterName, buffer);
    sprintf(rosterNameWithPrefix, "Roster_%s", oldRosterName);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, oldRosterName);
      sleep(2);
      system("clear");
      ask_which_roster_and_preform_action("rename");
    }

    else if (rosterExists == TRUE)
    {
      char newRosterName[30];
      system("clear");
      sleep(1);
      printf("Enter the new name for the roster: %s\n", oldRosterName);
      printf(YELLOW "To cancel this operation enter" BOLD " 'cancel'.\n" RESET);
      UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(newRosterName);
      if (strcmp(newRosterName, "cancel") == 0)
      {
        system("clear");
        printf(YELLOW "Cancelling operation\n" RESET);
        sleep(1);
        system("clear");
        return 0; // TODO come back to this. might call a function instead
      }
      else
      {
        int hasNonSpaceChar = has_one_non_space_char(newRosterName);
        if (hasNonSpaceChar == TRUE)
        {
          int confirmation = confirm_action("rename", oldRosterName, newRosterName);
          switch (confirmation)
          {
          case 1:
            int retValue = rename_roster(rosterNameWithPrefix, newRosterName);
            if (retValue == 0)
            {
              system("clear");
              printf(GREEN "Successfully renamed roster:" BOLD " %s" RESET GREEN " to" BOLD " %s\n" RESET, oldRosterName, newRosterName);
              sleep(2);
              system("clear");
            }
            else
            {
              system("clear");
              printf(RED "Failed to rename roster:" BOLD " %s" RESET RED "to" BOLD " %s" RESET RED "please try again\n" RESET, oldRosterName, newRosterName);
              sleep(2);
              system("clear");
            }
            break;
          case 2:
            printf("Okay lets try again.");
            ask_which_roster_and_preform_action("rename");
            break;
          default:
          }
        }
        else
        {
          system("clear");
          printf(YELLOW "The entered roster name: " BOLD " %s" RESET YELLOW " is too short please try again.\n" RESET);
          ask_which_roster_and_preform_action("rename");
        }
      }
    }
  }
  else if (strcmp(action, "delete") == 0)
  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    sprintf(rosterNameWithPrefix, "Roster_%s", buffer);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, buffer);
      sleep(2);
      system("clear");
      ask_which_roster_and_preform_action("delete");
    }

    else if (rosterExists == TRUE)
    {
      char confirmation[30];
      system("clear");
      sleep(1);
      printf(RED "WARNING: You are about to delete the roster:" BOLD " %s.\n" RESET, buffer);
      printf(RED "This action is IRREVERSIBLE.\n" RESET);
      printf("Do you understand that this cannot be undone? [y/n]\n");
      UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(confirmation);
      if (INPUT_IS_YES(confirmation))
      {
        int confirmation = confirm_action("delete", buffer);
        switch (confirmation)
        {
        case 1:
          int retValue = drop_table(rosterNameWithPrefix);
          if (retValue == 0)
          {
            system("clear");
            printf(GREEN "Successfully deleted roster:" BOLD " %s\n" RESET, buffer);
            sleep(2);
            system("clear");
          }
          else
          {
            system("clear");
            printf(RED "Failed to delete roster:" BOLD " %s" RESET RED "please try again\n" RESET, buffer);
            sleep(2);
            system("clear");
          }
          break;
        case 2:
          printf("Okay lets try again.");
          ask_which_roster_and_preform_action("delete");
          break;
        default:
        }
      }
      else if (INPUT_IS_NO(confirmation))
      {
        // todo handle this
      }
      else
      {
        system("clear");
        printf("Please enter a valid decision.\n");
        sleep(1);
        system("clear");
        ask_which_roster_and_preform_action("delete");
      }
    }
  }
  else if (strcmp(action, "add student to roster") == 0)
  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    sprintf(rosterNameWithPrefix, "Roster_%s", buffer);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, buffer);
      sleep(2);
      system("clear");
      ask_which_roster_and_preform_action("add student to roster");
    }
    else if (rosterExists == TRUE)
    {
      system("clear");
      sleep(1);
      // do stuff
      // todo Marsh, the problem to come back to... there are a couple things that need to happen for this action. 1. We need to search for the student. 2. handle multiple results in the event that there are people with the same name. one way to do that might be to do a for loop. add a number too each entry in the query then let the user enter the number associated with the query.  3. Then take that students firstName, lastName, and studentID and copy it into the new roster. 4. prob more complicated than this idk lol. Have fun
    }
  }
  else if (strcmp(action, "remove a student from a roster") == 0)
  {
    // do stuff
    return 0;
  }
  else if (strcmp(action, "Create a column") == 0)

  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);

    sprintf(rosterNameWithPrefix, "Roster_%s", buffer);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, buffer);
      sleep(2);
      system("clear");
      ask_which_roster_and_preform_action("create a column");
    }
    else if (rosterExists == TRUE)
    {
      system("clear");
      sleep(1);
      choose_col_type(rosterNameWithPrefix);
    }
  }
  else if (strcmp(action, "delete a column") == 0)
  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    sprintf(rosterNameWithPrefix, "Roster_%s", buffer);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, buffer);
      sleep(2);
      system("clear");
      ask_which_roster_and_preform_action("delete a column");
    }
    else if (rosterExists == TRUE)
    {
      system("clear");
      sleep(1);
      delete_col(rosterNameWithPrefix);
    }
  }
  else
  {
    system("clear");
    printf(RED "ERROR: Invalid action: " BOLD "%s\n" RESET, action);
    sleep(1);
    system("clear");
    return 0;
  }
}

int create_col(const char *rosterName, const char *colType)
{
  char colName[20];
  system("clear");
  puts("Enter a name for the new column");
  puts("Note: The column name must be atleast one character long");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
  strcpy(colName, buffer);

  if (strcmp(buffer, "cancel") == 0)
  {
    system("clear");
    printf(YELLOW "Cancelling operation\n" RESET);
    sleep(1);
    system("clear");
    return 0; // TODO come back to this. might call a function instead
  }
  else
  {
    int hasNonSpaceChar = has_one_non_space_char(buffer);
    if (hasNonSpaceChar == TRUE)
    {
      int confirmation = confirm_action("Create a column", rosterName);
      if (confirmation == 1)
      {
        int result = add_col_to_roster(rosterName, colName, colType);
        if (result == 0)
        {
          system("clear");
          printf(GREEN "Successfully added a new column to " BOLD "%s.\n" RESET, rosterName);
        }
        else
        {
          printf(RED "ERROR: Unable to add column: " BOLD "%s" RESET RED " to roster: " BOLD "%s\n" RESET, colName, rosterName);
          puts("Please try again.");
          return 0;
        }
      }
      else if (confirmation == 0)
      {
        system("clear");
        puts("Ok lets try again.");
        sleep(1);
        system("clear");
        create_col(rosterName, colType);
      }
      else
      {
        system("clear");
        puts("Please enter a valid decision.");
        sleep(1);
        system("clear");
        create_col(rosterName, colType);
      }
    }

    else if (hasNonSpaceChar == FALSE)
    {
      system("clear");
      puts(YELLOW "The entered name is too short please try again" RESET);
      sleep(1);
      system("clear");
      create_col(rosterName, colType);
    }
  }
}

int delete_col(const char *rosterName)
{
  char colName[20];
  system("clear");
  puts("Enter a name for the column that you would like to delete");
  puts("Note: The column name must be atleast one character long");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
  strcpy(colName, buffer);

  if (strcmp(buffer, "cancel") == 0)
  {
    system("clear");
    printf(YELLOW "Cancelling operation\n" RESET);
    sleep(1);
    system("clear");
    return 0; // TODO come back to this. might call a function instead
  }
  else
  {
    int hasCol = check_if_col_exists(rosterName, colName);
    if (hasCol == TRUE)
    {
      int confirmation = confirm_action("delete a column", rosterName);
      if (confirmation == 1)
      {
        int result = delete_col_from_roster(rosterName, colName);
        if (result == 0)
        {
          system("clear");
          printf(GREEN "Successfully deleted column: " BOLD "%s" RESET GREEN " from roster: " BOLD "%s\n" RESET, colName, rosterName);
        }
        else
        {
          printf(RED "ERROR: Unable to delete column: " BOLD "%s" RESET RED " from roster: " BOLD "%s\n" RESET, colName, rosterName);
          puts("Please try again.");
          return 0;
        }
      }
      else if (confirmation == 0)
      {
        system("clear");
        puts("Ok lets try again.");
        sleep(1);
        system("clear");
        delete_col(rosterName);
      }
      else
      {
        system("clear");
        puts("Please enter a valid decision.");
        sleep(1);
        system("clear");
        delete_col(rosterName);
      }
    }

    else if (hasCol == FALSE)
    {
      system("clear");
      puts(YELLOW "The entered name is too short please try again" RESET);
      sleep(1);
      system("clear");
      delete_col(rosterName);
    }
  }
}

int choose_col_type(const char *rosterName)
{
  int showingColSelectionMenu = TRUE;
  char colType[20];
  while (showingColSelectionMenu == TRUE)
  {
    system("clear");
    puts("Enter the number for the type of data this column will hold?");
    puts(YELLOW "Enter 'cancel' to cancel this operation" RESET);
    puts("1. Text");
    puts("2. Whole Numbers");
    puts("3. Decimals");
    puts("4. True or False Values");
    puts("5. Dates");
    puts("6. Not sure");
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
    int numSelection = atoi(buffer);
    showingColSelectionMenu = FALSE;
    switch (numSelection)
    {
    case 1:
    case 6: // in the event the user doesn't know. We just store it as TEXT
      strcpy(colType, "TEXT");
      create_col(rosterName, colType);
      break;
    case 2:
      strcpy(colType, "INTEGER");
      create_col(rosterName, colType);
      break;
    case 3:
      strcpy(colType, "REAL"); // the REAL data type is used to store floating-point values
      create_col(rosterName, colType);
      break;
    case 4:
      strcpy(colType, "BOOLEAN");
      create_col(rosterName, colType);
      break;
    case 5:
      strcpy(colType, "DATE");
      create_col(rosterName, colType);
      break;
    default:
      system("clear");
      puts("Please enter a valid decision.");
      sleep(1);
      system("clear");
      showingColSelectionMenu = TRUE;
    }
  }
}

int confirm_action(const char *action, ...)
{
  char buffer[30];
  va_list args;
  va_start(args, action);

  char *rosterName = va_arg(args, char *);
  va_end(args);

  system("clear");
  printf(YELLOW "Are you sure that you want to %s" BOLD " %s " RESET YELLOW " [y/n]\n" RESET, action, rosterName);

  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(buffer);
  if (INPUT_IS_YES(buffer))
  {
    return 1;
  }
  else if (INPUT_IS_NO(buffer))
  {
    return 2;
  }
  else
  {
    system("clear");
    puts("Please enter a valid decision.");
    sleep(1);
    system("clear");
    confirm_action(action);
  }
}

// this handles the action of viewing all of a rosters information
int show_roster_data(const char *rosterName)
{
}
