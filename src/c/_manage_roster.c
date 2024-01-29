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
Roster roster;
static uint8_t mainMenuProccess;

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
        "8. Help",
        "9. Main menu"};

    show_current_menu("Manage Existing Rosters");
    puts("What would you like to do?");
    puts("|===========================================================================================");
    for (int i = 0; i < 9; ++i)
    {
      printf("| %s %-90s\n", manageRosterOptions[i], "");
    }
    for (int i = 0; i < menuHeight; ++i)
    {
      printf("| %-90s\n", "");
    }
    puts("|===========================================================================================");

    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);

    manageRosterInput.NumInput = atoi(manageRosterInput.StrInput);
    if (manageRosterInput.NumInput == 1 || strcmp(manageRosterInput.StrInput, "view roster") == 0 || strcmp(manageRosterInput.StrInput, "view") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
          __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
          if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
          {
            system("clear");
            printf(YELLOW "Cancelling operation\n" RESET);
            sleep(1);
            system("clear");
            return 0; // TODO come back to this. might call a function instead
          }

          sprintf(roster.rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
          int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
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
              show_roster_data_with_warning(roster.rosterNameWithPrefix);
              puts("What would you like to do?");
              printf("Enter the corresponding number.\n\n");
              puts("1.Sort Roster");
              puts("2.Go Back");
              puts("3.Main Menu");
              __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
              manageRosterInput.NumInput = atoi(manageRosterInput.StrInput);
              switch (manageRosterInput.NumInput)
              {
              case 1:
                showingRosterData = FALSE;
                system("clear");
                show_roster_data_without_warning(roster.rosterNameWithPrefix);
                choose_which_col_to_sort(roster.rosterNameWithPrefix);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (manageRosterInput.NumInput == 2 || strcmp(manageRosterInput.StrInput, "rename roster") == 0 || strcmp(manageRosterInput.StrInput, "rename") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      show_current_step("Rename roster", 1, 2);

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (manageRosterInput.NumInput == 3 || strcmp(manageRosterInput.StrInput, "delete roster") == 0 || strcmp(manageRosterInput.StrInput, "delete") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      show_current_step("Delete roster", 1, 2);

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (manageRosterInput.NumInput == 4 || strcmp(manageRosterInput.StrInput, "add student") == 0 || strcmp(manageRosterInput.StrInput, "add") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      show_current_step("Add student to roster", 1, 2);
      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (manageRosterInput.NumInput == 5 || strcmp(manageRosterInput.StrInput, "remove student from roster") == 0 || strcmp(manageRosterInput.StrInput, "remove") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      show_current_step("Remove students from roster", 1, 2);
      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (manageRosterInput.NumInput == 6 || strcmp(manageRosterInput.StrInput, "add column") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      show_current_step("Create a column", 1, 3);
      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }

    else if (manageRosterInput.NumInput == 7 || strcmp(manageRosterInput.StrInput, "delete a column") == 0)
    {

      manageRosterMenuIsOpen = FALSE;
      system("clear");
      show_current_step("Delete a column", 1, 2);
      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
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
        __utils_error_logger("Failed to get roster table count.", "manage_roster", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }

    else if (manageRosterInput.NumInput == 8 || strcmp(manageRosterInput.StrInput, "help") == 0)
    {
      // manageRosterMenuIsOpen = FALSE;
    }
    else if (manageRosterInput.NumInput == 9 || strcmp(manageRosterInput.StrInput, "main menu") == 0 || strcmp(manageRosterInput.StrInput, "main") == 0)
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
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  if (strcmp(action, "rename") == 0)
  {
    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    strcpy(oldRosterName, manageRosterInput.StrInput);
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", oldRosterName);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, oldRosterName);
      sleep(2);
      system("clear");

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tableExists == TRUE)
      {
        int showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("rename");
          showingFoundRosters = FALSE;
        }
      }
    }
    else if (rosterExists == TRUE)
    {
      char newRosterName[30];
      system("clear");
      sleep(1);
      show_current_step("Rename roster", 2, 2);
      printf("Enter the new name for the roster: %s\n", oldRosterName);
      puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
      __utils_fgets_and_remove_newline(newRosterName);
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
            int retValue = rename_roster(roster.rosterNameWithPrefix, newRosterName);
            if (retValue == 0)
            {
              system("clear");
              printf(GREEN "Successfully renamed roster:" BOLD " %s" RESET GREEN " to" BOLD " %s\n" RESET, oldRosterName, roster.rosterNameWithPrefix);
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
          sleep(2);
          system("clear");
        }
      }
    }
  }
  else if (strcmp(action, "delete") == 0)
  {
    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
      sleep(2);
      system("clear");

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tableExists == TRUE)
      {
        int showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("delete");
          showingFoundRosters = FALSE;
        }
      }
    }

    else if (rosterExists == TRUE)
    {
      char confirmation[30];
      system("clear");
      sleep(1);
      show_current_step("Rename roster", 2, 2);
      printf(RED "WARNING: You are about to delete the roster:" BOLD " %s.\n" RESET, manageRosterInput.StrInput);
      printf(RED "This action is IRREVERSIBLE.\n" RESET);
      printf("Do you understand that this cannot be undone? [y/n]\n");
      puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
      __utils_fgets_and_remove_newline(confirmation);
      if (INPUT_IS_YES(confirmation))
      {
        int confirmation = confirm_action("delete", manageRosterInput.StrInput);
        switch (confirmation)
        {
        case 1:
          int retValue = drop_table(roster.rosterNameWithPrefix);
          if (retValue == 0)
          {
            system("clear");
            printf(GREEN "Successfully deleted roster:" BOLD " %s\n" RESET, roster.rosterNameWithPrefix);
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
      else if (INPUT_IS_NO(confirmation) || INPUT_IS_CANCEL(confirmation))
      {
        system("clear");
        printf(YELLOW "Cancelling operation\n" RESET);
        sleep(1);
        system("clear");
        return 0; // TODO come back to this. might call a function instead
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

    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
      sleep(2);
      system("clear");

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tableExists == TRUE)
      {
        int showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("add student to roster");
          showingFoundRosters = FALSE;
        }
      }
    }
    else if (rosterExists == TRUE)
    {

      system("clear");
      sleep(1);
      // do stuff
      // todo OLD:  Marsh, the problem to come back to... there are a couple things that need to happen for this action. 1. We need to search for the student. 2. handle multiple results in the event that there are people with the same name. one way to do that might be to do a for loop. add a number too each entry in the query then let the user enter the number associated with the query.  3. Then take that students firstName, lastName, and studentID and copy it into the new roster. 4. prob more complicated than this idk lol. Have fun
      int showingHowToAddStudentMenu = TRUE;

      while (showingHowToAddStudentMenu == TRUE)
      {
        create_student_db_and_table();
        show_current_step("Add student to roster", 2, 2);
        printf("How would you like to add a student to roster: " BOLD "%s?\n" RESET, roster.rosterNameWithPrefix);
        puts("Enter the corresponding number.");
        puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
        printf("\n");
        puts("1. Search the student database for a student");
        puts("2. Manually add a student");
        puts("3. Use the bulk data loader to add multiple students");
        __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
        manageRosterInput.NumInput = atoi(manageRosterInput.StrInput);
        if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
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
          // todo use search function from _search_student.c
          break;
        case 2:
          system("clear");
          globalTrigger.isTriggered = TRUE;
          globalTrigger.studentCreationInterrupted = FALSE;
          get_student_first_name();
          if (globalTrigger.studentCreationInterrupted == FALSE)
          {
            skip_and_add_to_roster(roster.rosterNameWithPrefix);
            globalTrigger.isTriggered = FALSE;
            break;
          }
          else
          {
            system("clear");
            printf(RED "Failed to add student to roster: " BOLD "%s\n" RESET, roster.rosterNameWithPrefix);
            sleep(1);
            system("clear");
            break;
          }
          // todo need to create a trigger of some sort that will not run the next function if the previous function does not finish executing
        case 3:
          // todo add bulk data loader stuff
          system("clear");
          puts(YELLOW "THIS FEATURE IS NOT YET IMPLEMENTED" RESET);
          sleep(2);
          system("clear");
          exit(0);
          break;
        default:
          break;
        }
        showingHowToAddStudentMenu == FALSE;
      }
    }
  }
  else if (strcmp(action, "remove student from roster") == 0)
  {
    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }

    sprintf(roster.rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);

    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, roster.rosterNameWithPrefix);
      sleep(2);
      system("clear");
      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tableExists == TRUE)
      {
        int showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("remove student from roster");
          showingFoundRosters = FALSE;
        }
      }
    }
    else if (rosterExists == TRUE)
    {
      system("clear");
      sleep(1);
      // do stuff
      globalTrigger.isTriggered = TRUE;

      show_current_step("Remove student from roster", 2, 2);
      handle_student_deletion_logic(roster.rosterNameWithPrefix);
    }
  }
  else if (strcmp(action, "Create a column") == 0)
  {
    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
      sleep(2);
      system("clear");

      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tableExists == TRUE)
      {
        int showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("Create a column");
          showingFoundRosters = FALSE;
        }
      }
    }
    else if (rosterExists == TRUE)
    {
      system("clear");
      sleep(1);

      choose_col_type(roster.rosterNameWithPrefix);
    }
  }
  else if (strcmp(action, "delete a column") == 0)
  {
    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
    if (INPUT_IS_CANCEL(manageRosterInput.StrInput))
    {
      system("clear");
      printf(YELLOW "Cancelling operation\n" RESET);
      sleep(1);
      system("clear");
      return 0; // TODO come back to this. might call a function instead
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", manageRosterInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, manageRosterInput.StrInput);
      sleep(2);
      system("clear");
      int tableExists = get_table_count(programSettings.databaseInfo.dbPath);
      if (tableExists == TRUE)
      {
        int showingFoundRosters = TRUE;
        while (showingFoundRosters == TRUE)
        {
          ask_which_roster_and_preform_action("delete a column");
          showingFoundRosters = FALSE;
        }
      }
    }
    else if (rosterExists == TRUE)
    {
      system("clear");
      sleep(1);
      delete_col(roster.rosterNameWithPrefix);
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
  puts("Enter a name for the new column");
  puts("Note: The column name must be atleast one character long");
  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
  __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
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
          sleep(2);
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
// TODO need to build a function that lists all the columns in a roster except for the ones that cannot be deleted: FirstName, LastName, StudentID
int delete_col(const char *rosterName)
{
  char colName[20];
  system("clear");
  show_current_step("Delete a column", 2, 2);
  puts("Enter the name of the column that you would like to delete");
  puts(YELLOW "You CANNOT delete the FirstName, LastName, or StudentID columns" RESET);
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
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
          sleep(2);
        }
        else
        {
          printf(RED "ERROR: Unable to delete column: " BOLD "%s" RESET RED " from roster: " BOLD "%s\n" RESET, colName, rosterName);
          puts("Please try again.");
          sleep(2);
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
    show_current_step("Create a column", 2, 3);
    puts("Enter the corresponding number for the type of data this column will hold?");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    puts("1. Text");
    puts("2. Whole Numbers");
    puts("3. Decimals");
    puts("4. True or False Values");
    puts("5. Dates");
    puts("6. Not sure");
    __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
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
      system("clear");
      show_current_step("Add student to roster", 3, 3);
      create_col(rosterName, colType);
      break;
    case 2:
      strcpy(colType, "INTEGER");
      system("clear");
      show_current_step("Add student to roster", 3, 3);
      create_col(rosterName, colType);
      break;
    case 3:
      strcpy(colType, "REAL"); // the REAL data type is used to store floating-point values
      system("clear");
      show_current_step("Add student to roster", 3, 3);
      create_col(rosterName, colType);
      break;
    case 4:
      strcpy(colType, "BOOLEAN");
      system("clear");
      show_current_step("Add student to roster", 3, 3);
      create_col(rosterName, colType);
      break;
    case 5:
      strcpy(colType, "DATE");
      system("clear");
      show_current_step("Add student to roster", 3, 3);
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
// todo make this function better. make type an int
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
  __utils_fgets_and_remove_newline(manageRosterInput.StrInput);

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
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  // todo handle canceling operation
  if (strcmp(rosterColumn.ColumnType, "BOOLEAN") == 0)
  {
    puts("1: True First");
    puts("2: False First");
    __utils_fgets_and_remove_newline(rosterColumn.ColumnSortingMethod);
    manageRosterInput.NumInput = atoi(rosterColumn.ColumnSortingMethod);
    if (INPUT_IS_CANCEL(rosterColumn.ColumnSortingMethod))
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
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN " in roster: " BOLD "%s" RESET GREEN " with true values first.\n " RESET, colName, roster.rosterNameWithPrefix);
      sort_roster_col_ascending(roster.rosterNameWithPrefix, colName); // sql interprets true as 1 and false as 0 so we can just sort ascending to show true first
      wait_for_char_input();
      break;
    case 2:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN " in roster: " BOLD "%s" RESET GREEN " with false values first.\n " RESET, colName, roster.rosterNameWithPrefix);
      sort_roster_col_descending(roster.rosterNameWithPrefix, colName); // sql interprets true as 1 and false as 0 so we can just sort descending to show false first
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
    __utils_fgets_and_remove_newline(rosterColumn.ColumnSortingMethod);
    manageRosterInput.NumInput = atoi(rosterColumn.ColumnSortingMethod);
    if (INPUT_IS_CANCEL(rosterColumn.ColumnSortingMethod))
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
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN " in roster: " BOLD "%s" RESET GREEN " in ascending order.\n " RESET, colName, roster.rosterNameWithPrefix);
      sort_roster_col_ascending(roster.rosterNameWithPrefix, colName);
      wait_for_char_input();
      break;
    case 2:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN " in roster: " BOLD "%s" RESET GREEN " in descending order.\n " RESET, colName, roster.rosterNameWithPrefix);
      sort_roster_col_descending(roster.rosterNameWithPrefix, colName);
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
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
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
    show_roster_data_with_warning(rosterName);
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
  int rosterHasData = check_for_data_in_roster(roster.rosterNameWithPrefix);
  if (rosterHasData == TRUE)
  {
    show_all_roster_data(rosterName);
  }
  else if (rosterHasData == FALSE)
  {
    system("clear");
    printf(YELLOW "The roster: " BOLD "%s" RESET YELLOW " does not have any data to display.\n" RESET, rosterName);
    printf("Please add a student to the roster and try again.\n");
    wait_for_char_input();
    manage_roster();
  }
  else
  {
    __utils_error_logger("Failed to check if roster has data.", "check_if_roster_has_data", MINOR);
    printf(RED "Error: Failed to check if roster has data.\n" RESET);
    // TODO do something here
  }
}

/************************************************************************************
 * handle_student_deletion_logic(): Handles the logic for deleting a student from a
 *                                  roster.
 *
 * Note: see usage in ask_which_roster_and_preform_action()
 ************************************************************************************/
int handle_student_deletion_logic(const char *rosterName)
{
  show_all_roster_data(rosterName);
  puts("Enter the StudentID of the student that you would like to remove from the roster.");
  __utils_fgets_and_remove_newline(manageRosterInput.StrInput);

  Student student;
  strcpy(student.StudentID, manageRosterInput.StrInput);
  int studentExists = check_if_student_id_exists(student.StudentID, rosterName);
  if (studentExists == FALSE)
  {
    system("clear");
    printf(YELLOW "The entered studentID: " BOLD "%s" RESET YELLOW " does not exist in roster: " BOLD "%s\n" RESET, student.StudentID, rosterName);
    sleep(2);
    system("clear");
    puts("Please try again");
    sleep(1);
    system("clear");
    globalTrigger.isTriggered = TRUE;
    show_roster_data_without_warning(rosterName);
    system("clear"); // Super hacky way to not show data twice
    show_current_step("Remove student from roster", 2, 2);
    handle_student_deletion_logic(rosterName);
  }
  else if (studentExists == TRUE)
  {
    int confirmation = confirm_action("remove student from roster", rosterName);
    if (confirmation == 1)
    {
      int result = delete_student_from_table(student.StudentID, rosterName);
      if (result == 0)
      {
        system("clear");
        printf(GREEN "Successfully deleted student: " BOLD "%s" RESET GREEN " from roster: " BOLD "%s\n" RESET, student.StudentID, rosterName);
        sleep(1);
        system("clear");
        manage_roster();
      }
      else
      {
        printf(RED "ERROR: Unable to delete student: " BOLD "%s" RESET RED " from roster: " BOLD "%s\n" RESET, student.StudentID, rosterName);
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
      handle_student_deletion_logic(rosterName);
    }
    else
    {
      system("clear");
      puts("Please enter a valid decision.");
      sleep(1);
      system("clear");
      handle_student_deletion_logic(rosterName);
    }
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

  __utils_fgets_and_remove_newline(manageRosterInput.StrInput);
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
