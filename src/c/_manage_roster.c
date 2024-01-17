/*
===============================================================================
File Name   : _manage_roster.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains all functions used
              to manage rosters.
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

Column rosterColumn;
UserInput manageRosterInput;
static uint8_t mainMenuProccess;
char rosterNameWithPrefix[60];
/************************************************************************************
 * manage_roster(): Handles the logic for managing a roster.
 * Note: see usage in ./main.c
 * Note: Uses the following helper functions from db.hpp:
 * - show_tables() - shows all tables in the database
 * - rename_roster() - renames a roster
 * - drop_table() - deletes a roster
 * - get_table_count() - the return value is used to determine if there are any
 *                       rosters in the database
 * - check_if_table_exists() - checks if a roster exists
 * - add_col_to_roster() - adds a column to a roster
 * - delete_col_from_roster() - deletes a column from a roster
 * - check_if_col_exists() - checks if a column exists in a roster
 *
 ************************************************************************************/

int manage_roster(void)
{
  uint8_t manageRosterMenuIsOpen = TRUE;
  uint8_t showingFoundRosters = FALSE;
  uint8_t showingRosterData = FALSE;
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

    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);

    manageRosterInput.NumInput = atoi(manageRosterInput.StrInput);
    if (manageRosterInput.NumInput == 1 || strcmp(manageRosterInput.StrInput, "view roster") == 0 || strcmp(manageRosterInput.StrInput, "view") == 0)
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
          puts("Enter the name of the roster that you would like to view?");
          puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
          UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
          if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
          {
            system("clear");
            printf(YELLOW "Cancelling operation\n" RESET);
            sleep(1);
            system("clear");
            return 0; // TODO come back to this. might call a function instead
          }

          sprintf(rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
          int rosterExists = check_if_table_exists(rosterNameWithPrefix);
          switch (rosterExists)
          {
          case 0:
            system("clear");
            printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
            sleep(2);
            system("clear");
            showingFoundRosters = TRUE;
            break;
          case 1:
            system("clear");
            showingRosterData = TRUE;

            while (showingRosterData == TRUE)
            {
              show_roster_data_with_warning(rosterNameWithPrefix);
              puts("What would you like to do?");
              printf("Enter the corresponding number.\n\n");
              puts("1.Sort Roster");
              puts("2.Go Back");
              puts("3.Main Menu");
              UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
              manageRosterInput.NumInput = atoi(manageRosterInput.StrInput);
              switch (manageRosterInput.NumInput)
              {
              case 1:
                showingRosterData = FALSE;
                system("clear");
                show_roster_data_without_warning(rosterNameWithPrefix);
                choose_which_col_to_sort(rosterNameWithPrefix);
              case 2:
                showingRosterData = FALSE;
                system("clear");
                puts("Returning to previous menu");
                sleep(1);
                system("clear");
                manage_roster();
              case 3:
                showingRosterData = FALSE;
                system("clear");
                puts("Returning to main menu");
                sleep(1);
                system("clear");
                return 0;
              default:
                system("clear");
                puts("Invalid input please try again.");
                sleep(1);
                system("clear");
                showingRosterData = TRUE; 
              }
            }
            break;
          default:
            system("clear");
            printf(RED "Roster does not exist please try again.\n" RESET);
            wait_for_char_input();
            break;
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
    else if (manageRosterInput.NumInput == 2 || strcmp(manageRosterInput.StrInput, "rename roster") == 0 || strcmp(manageRosterInput.StrInput, "rename") == 0)
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
    else if (manageRosterInput.NumInput == 3 || strcmp(manageRosterInput.StrInput, "delete roster") == 0 || strcmp(manageRosterInput.StrInput, "delete") == 0)
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
    else if (manageRosterInput.NumInput == 4 || strcmp(manageRosterInput.StrInput, "add student") == 0 || strcmp(manageRosterInput.StrInput, "add") == 0)
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
    else if (manageRosterInput.NumInput == 5 || strcmp(manageRosterInput.StrInput, "remove student") == 0 || strcmp(manageRosterInput.StrInput, "remove") == 0)
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
    else if (manageRosterInput.NumInput == 6 || strcmp(manageRosterInput.StrInput, "add column") == 0)
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

    else if (manageRosterInput.NumInput == 7 || strcmp(manageRosterInput.StrInput, "delete a column") == 0)
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

    else if (manageRosterInput.NumInput == 99 || strcmp(manageRosterInput.StrInput, "help") == 0)
    {
      // manageRosterMenuIsOpen = FALSE;
    }
    else if (manageRosterInput.NumInput == 100 || strcmp(manageRosterInput.StrInput, "main menu") == 0 || strcmp(manageRosterInput.StrInput, "main") == 0)
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
/************************************************************************************
 * ask_which_roster_and_preform_action(): Handles the bulk of the logic for managing
 *                                        which action is to be preformed on a roster.
 *
 * Note: see usage in manage_roster()
 ************************************************************************************/
int ask_which_roster_and_preform_action(char *action)
{

  char oldRosterName[30];
  printf("Enter the name of the roster that you would like to %s?\n", action);
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  // TODO handle canceling operation printf(YELLOW "To cancel this operation enter" BOLD " 'cancel'. \n" RESET);
  if (strcmp(action, "rename") == 0)
  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    strcpy(oldRosterName, manageRosterInput.StrInput);
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
      if (INPUT_IS_CANCEL(newRosterName))
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
          printf(YELLOW "The entered roster name: " BOLD " %s" RESET YELLOW " is too short please try again.\n" RESET, newRosterName);
          ask_which_roster_and_preform_action("rename");
        }
      }
    }
  }
  else if (strcmp(action, "delete") == 0)
  {
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
      sleep(2);
      system("clear");
      ask_which_roster_and_preform_action("delete");
    }

    else if (rosterExists == TRUE)
    {
      char confirmation[30];
      system("clear");
      sleep(1);
      printf(RED "WARNING: You are about to delete the roster:" BOLD " %s.\n" RESET, manageRosterInput.StrInput);
      printf(RED "This action is IRREVERSIBLE.\n" RESET);
      printf("Do you understand that this cannot be undone? [y/n]\n");
      UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(confirmation);
      if (INPUT_IS_YES(confirmation))
      {
        int confirmation = confirm_action("delete", manageRosterInput.StrInput);
        switch (confirmation)
        {
        case 1:
          int retValue = drop_table(rosterNameWithPrefix);
          if (retValue == 0)
          {
            system("clear");
            printf(GREEN "Successfully deleted roster:" BOLD " %s\n" RESET, manageRosterInput.StrInput);
            sleep(2);
            system("clear");
          }
          else
          {
            system("clear");
            printf(RED "Failed to delete roster:" BOLD " %s" RESET RED "please try again\n" RESET, manageRosterInput.StrInput);
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

    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
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
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
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
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
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
/************************************************************************************
 * create_col(): Creates a new column in the passed in roster.
 *
 * Note: see usage in ask_which_roster_and_preform_action()
 ************************************************************************************/
int create_col(const char *rosterName, const char *colType)
{
  char colName[20];
  system("clear");
  puts("Enter a name for the new column");
  puts("Note: The column name must be atleast one character long");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
  strcpy(colName, manageRosterInput.StrInput);

  if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
  {
    system("clear");
    printf(YELLOW "Cancelling operation\n" RESET);
    sleep(1);
    system("clear");
    return 0; // TODO come back to this. might call a function instead
  }
  else
  {
    int hasNonSpaceChar = has_one_non_space_char(manageRosterInput.StrInput);
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

/************************************************************************************
 * delete_col(): Deletes a column from the passed in roster.
 *
 * Note: see usage in ask_which_roster_and_preform_action()
 ************************************************************************************/
int delete_col(const char *rosterName)
{
  char colName[20];
  system("clear");
  puts("Enter a name for the column that you would like to delete");
  puts("Note: The column name must be atleast one character long");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
  strcpy(colName, manageRosterInput.StrInput);

  if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
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
      printf(YELLOW "The entered column: " BOLD "%s" RESET " does not exist in roster: " BOLD "%s\n" RESET, colName, rosterName);
      sleep(2);
      system("clear");
      delete_col(rosterName);
    }
  }
}

/************************************************************************************
 * choose_col_type(): Handles the logic for choosing the type of data that a column
 *                    will hold.
 *
 * Note: see usage in create_col()
 ************************************************************************************/
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
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    int numSelection = atoi(manageRosterInput.StrInput);
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

/************************************************************************************
 * check_roster_col_type(): Handles checking the type of data of
 *                          an existing column. A pretty bs function tbh.
 *
 * Note: see usage in
 ************************************************************************************/
const char check_roster_col_type(const char *rosterName, const char *colName)
{

  int colTypeReturnValue = check_col_type(rosterName, colName);
  switch (colTypeReturnValue)
  {
  case 5:
    return colTypeReturnValue;
  case 6:
    return colTypeReturnValue;
  case 7:
    return colTypeReturnValue;
  case 8:
    return colTypeReturnValue;
  case 9:
    return colTypeReturnValue;
  default:
    return 0;
  }
}

/************************************************************************************
  * choose_which_col_to_sort(): Handles the logic for choosing which column to sort.
  *   
  * 
  * 
  * Note: see usage in manage_roster()
 ************************************************************************************/
int choose_which_col_to_sort(const char *rosterName)
{
  puts("Which column would you like to sort?");
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);

  int colExists = check_if_col_exists(rosterName, manageRosterInput.StrInput);
  if (colExists == TRUE)
  {
    int result = check_roster_col_type(rosterName, manageRosterInput.StrInput);
    strcpy(rosterColumn.ColumnName, manageRosterInput.StrInput);
    switch (result)
    {
    case 8: // boolean
      strcpy(rosterColumn.ColumnType, "BOOLEAN");
      handle_col_sort_logic(rosterColumn.ColumnName);
      break;
    case 5: // text
    case 6: // integer
    case 7: // real
    case 9: // date
      strcpy(rosterColumn.ColumnType, "OTHER");
      handle_col_sort_logic(rosterColumn.ColumnName);
      break;
    }
  }
  else if (colExists == FALSE)
  {
    system("clear");
    printf(YELLOW "The entered column: " BOLD "%s" RESET YELLOW " does not exist in roster: " BOLD "%s\n" RESET, manageRosterInput.StrInput, rosterName);
    sleep(2);
    system("clear");
    puts("Please try again");
    show_roster_data_without_warning(rosterName);
    choose_which_col_to_sort(rosterName);
  }
}

/************************************************************************************
 * handle_col_sort_logic(): Handles the logic for sorting a column.
 * 
 * 
 * 
 * Note: see usage in choose_which_col_to_sort()
 ************************************************************************************/
int handle_col_sort_logic(const char *colName)
{
  puts("How would you like to sort this column?");
  puts("Enter the corresponding number for the sorting method.");
  puts(YELLOW "You can cancel this operation by entering 'cancel'" RESET);
  // todo handle canceling operation
  if (strcmp(rosterColumn.ColumnType, "BOOLEAN") == 0)
  {
    puts("1: True First");
    puts("2: False First");
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(rosterColumn.ColumnSortingMethod);
    manageRosterInput.NumInput = atoi(rosterColumn.ColumnSortingMethod);
    if(INPUT_IS_CANCEL(rosterColumn.ColumnSortingMethod))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    switch (manageRosterInput.NumInput)
    {
    case 1:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN" in roster: " BOLD "%s" RESET GREEN" with true values first.\n "RESET, colName, rosterNameWithPrefix);
      sort_roster_col_ascending(rosterNameWithPrefix, colName);
      wait_for_char_input();
      break;
    case 2:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN" in roster: " BOLD "%s" RESET GREEN" with false values first.\n "RESET, colName, rosterNameWithPrefix);
      sort_roster_col_descending(rosterNameWithPrefix, colName);
      wait_for_char_input();
      break;
    default:
      break;
    }
  }
  else if (strcmp(rosterColumn.ColumnType, "OTHER") == 0)
  {
    puts("1: Ascending Order");
    puts("2: Descending Order");
    UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(rosterColumn.ColumnSortingMethod);
    manageRosterInput.NumInput = atoi(rosterColumn.ColumnSortingMethod);
    if(INPUT_IS_CANCEL(rosterColumn.ColumnSortingMethod))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead if i do. make func type void
    }
    switch (manageRosterInput.NumInput)
    {
    case 1:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN" in roster: " BOLD "%s" RESET GREEN" in ascending order.\n "RESET, colName, rosterNameWithPrefix);
      sort_roster_col_ascending(rosterNameWithPrefix, colName);
      wait_for_char_input();
      break;
    case 2:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN" in roster: " BOLD "%s" RESET GREEN" in descending order.\n "RESET, colName, rosterNameWithPrefix);
      sort_roster_col_descending(rosterNameWithPrefix, colName);
      wait_for_char_input();
    default:
      break;
    }
  }
  
}

/************************************************************************************
 * show_roster_data_without_warning(): Displays a rosters data without the warning
 *                                     and without getting user confirmation.

 * Note: see usage in manage_roster()
 ************************************************************************************/
int show_roster_data_without_warning(const char *rosterName)
{
  system("clear");
  show_all_roster_data(rosterName);
}

/************************************************************************************
 * show_roster_data_with_warning(): Warns the user about displaying a rosters data
 *                                  and gets user confirmation to show said data.
 *
 * Note: see usage in manage_roster()
 ************************************************************************************/
int show_roster_data_with_warning(const char *rosterName)
{
  printf(RED "WARNING: You are requesting to show all data in the roster: " BOLD "%s\n" RESET, rosterName);
  puts("Continuing with this operation may have the following affects:" RESET);
  puts("1: Some data may not be visible to you depending on the dimensions of your terminal" RESET);
  printf("2: Take several seconds to load all the data\n\n" RESET);
  puts("Are you sure that you'd like to continue with this operation?[y/n]");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
  if (INPUT_IS_CANCEL(manageRosterInput.StrInput) || INPUT_IS_NO(manageRosterInput.StrInput))
  {
    system("clear");
    printf(YELLOW "Cancelling operation\n" RESET);
    sleep(1);
    system("clear");
    manage_roster();
  }
  else if (INPUT_IS_YES(manageRosterInput.StrInput))
  {
    system("clear");
    check_if_roster_has_data(rosterName);
    
  }
  else
  {
    system("clear");
    printf(YELLOW "Please enter a valid decision\n" RESET);
    sleep(2);
    system("clear");
    show_roster_data_without_warning(rosterName);
  }
}

/************************************************************************************
 * check_if_roster_has_data(): Handles the logic for checking if a roster has data
 *                             and displaying said data if it does.
 * 
 * Note: see usage in show_roster_data_with_warning()
 ************************************************************************************/
int check_if_roster_has_data(const char *rosterName)
{
int rosterHasData = check_for_data_in_roster(rosterNameWithPrefix); 
  if(rosterHasData == TRUE)
  {
    show_all_roster_data(rosterName);
  }
  else if(rosterHasData == FALSE)
  {
    system("clear");
    printf(YELLOW "The roster: " BOLD "%s" RESET YELLOW " does not have any data to display.\n" RESET, rosterName);
    printf("Please add a student to the roster and try again.\n");
    wait_for_char_input();
    manage_roster();
  }
  else
  {
    UTILS_ERROR_LOGGER("Failed to check if roster has data.", "check_if_roster_has_data", MINOR);
    printf(RED "Error: Failed to check if roster has data.\n" RESET);
    //TODO do something here
  }
}
/************************************************************************************
 * confirm_action(): Handles the logic for confirming an action.
 *
 * Note: see usage in ask_which_roster_and_preform_action()
 ************************************************************************************/
int confirm_action(const char *action, ...)
{
  va_list args;
  va_start(args, action);

  char *rosterName = va_arg(args, char *);
  va_end(args);

  system("clear");
  printf(YELLOW "Are you sure that you want to %s" BOLD " %s " RESET YELLOW " [y/n]\n" RESET, action, rosterName);

  UTILS_FGETS_AND_REMOVE_NEWLINE_CHAR(manageRosterInput.StrInput);
  if (INPUT_IS_YES(manageRosterInput.StrInput))
  {
    return 1;
  }
  else if (INPUT_IS_NO(manageRosterInput.StrInput))
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
