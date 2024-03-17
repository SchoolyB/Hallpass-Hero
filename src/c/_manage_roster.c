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
#include "../lib/headers/cpp/cpp_files.hpp"
#include "../lib/headers/c/c_files.h"

Column rosterColumn;
static uint8_t mainMenuProccess;

/************************************************************************************
 * show_manage_roster_menu(): Handles the logic for managing a roster.
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
int show_manage_roster_menu(void)
{
  uint8_t manageRosterMenuIsOpen = TRUE;
  uint8_t showingFoundRosters = FALSE;
  uint8_t showingRosterData = FALSE;
  uint8_t menuWidth = 70;
  uint8_t menuHeight = 10;

  system("clear");
  __utils_runtime_logger("entered roster management menu", "show_manage_roster_menu");
  __utils_check_for_sqlite_db();
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

    __utils_fgets_and_remove_newline(userInput.StrInput);

    userInput.NumInput = atoi(userInput.StrInput);
    if (userInput.NumInput == 1 || strcmp(userInput.StrInput, "view roster") == 0 || strcmp(userInput.StrInput, "view") == 0)
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
          __utils_fgets_and_remove_newline(userInput.StrInput);
          if (INPUT_IS_CANCEL(userInput.StrInput))
          {
            __utils_operation_cancelled("show_manage_roster_menu");
            show_manage_roster_menu();
          }

          sprintf(roster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);
          int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
          switch (rosterExists)
          {
          case 0:
            system("clear");
            printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, userInput.StrInput);
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
              puts("2. Update a student's information");
              puts("3.Go Back");
              puts("4.Main Menu");
              __utils_fgets_and_remove_newline(userInput.StrInput);
              userInput.NumInput = atoi(userInput.StrInput);
              switch (userInput.NumInput)
              {
              case 1:
                showingRosterData = FALSE;
                system("clear");
                show_roster_data_without_warning(roster.rosterNameWithPrefix);
                choose_which_col_to_sort(roster.rosterNameWithPrefix);
              case 2:
                showingRosterData = FALSE;
                system("clear");
                show_current_step("Update a student's information", 1, 2);
                printf("Enter the Sequence Number of the student whose information you would like to update.\n");
                puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
                __utils_fgets_and_remove_newline(userInput.StrInput);
                if (INPUT_IS_CANCEL(userInput.StrInput))
                {
                  __utils_operation_cancelled("show_manage_roster_menu");
                  show_manage_roster_menu();
                }
                int sequenceNumber = atoi(userInput.StrInput);
                int sequenceNumberExists = check_if_sequence_number_exists(sequenceNumber);
                if (sequenceNumberExists == FALSE)
                {
                  system("clear");
                  printf(YELLOW "The entered sequence number: " BOLD "%d" RESET YELLOW " does not exist please try again. \n" RESET, sequenceNumber);
                  sleep(2);
                  system("clear");
                  showingRosterData = TRUE;
                }
                else if (sequenceNumberExists == TRUE)
                {
                  system("clear");
                  show_current_step("Update a student's information", 2, 2);
                  printf("Which column would you like to update?\n");
                  puts(YELLOW "To cancel this operation enter 'cancel'" RESET);
                  __utils_fgets_and_remove_newline(userInput.StrInput);
                  if (INPUT_IS_CANCEL(userInput.StrInput))
                  {
                    __utils_operation_cancelled("show_manage_roster_menu");
                    show_manage_roster_menu();
                  }
                  else
                  {
                    const char *colName = userInput.StrInput;
                    strcpy(rosterColumn.ColumnName, colName);
                    int colExists = check_if_col_exists(roster.rosterNameWithPrefix, rosterColumn.ColumnName);
                    if (colExists == FALSE)
                    {
                      printf(YELLOW "The entered column: " BOLD "%s" RESET YELLOW " does not exist please try again.\n" RESET, rosterColumn.ColumnName);
                      sleep(2);
                      system("clear");
                      showingRosterData = TRUE;
                    }
                    else if (colExists == TRUE)
                    {
                      int getColDataType = check_col_type(roster.rosterNameWithPrefix, rosterColumn.ColumnName);
                      const char *newData;
                      int retValue;
                      switch (getColDataType)
                      {
                      case 5:
                        printf("Column: %s is of type: %s\n", rosterColumn.ColumnName, "TEXT");
                        printf("This means that you can only enter text into this column.\n");
                        printf("Enter the new value for the column: %s\n", rosterColumn.ColumnName);
                        __utils_fgets_and_remove_newline(userInput.StrInput);
                        newData = userInput.StrInput;
                        retValue = update_row_data(roster.rosterNameWithPrefix, rosterColumn.ColumnName, newData, sequenceNumber);
                        if (retValue == 0)
                        {
                          system("clear");
                          printf(GREEN "Successfully updated column: " BOLD "%s" RESET GREEN " for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        else
                        {
                          system("clear");
                          printf(RED "Failed to update column: " BOLD "%s" RESET RED "for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        break;
                      case 6:
                        printf("Column: %s is of type: %s\n", rosterColumn.ColumnName, "INTEGER");
                        printf("This means that you can only enter numbers into this column.\n");
                        printf("Enter the new value for the column: %s\n", rosterColumn.ColumnName);
                        __utils_fgets_and_remove_newline(userInput.StrInput);
                        newData = userInput.StrInput;
                        retValue = update_row_data(roster.rosterNameWithPrefix, rosterColumn.ColumnName, newData, sequenceNumber);
                        if (retValue == 0)
                        {
                          system("clear");
                          printf(GREEN "Successfully updated column: " BOLD "%s" RESET GREEN " for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        else
                        {
                          system("clear");
                          printf(RED "Failed to update column: " BOLD "%s" RESET RED "for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        break;
                      case 7:
                        printf("Column: %s is of type: %s\n", rosterColumn.ColumnName, "REAL");
                        printf("This means that you can only enter numbers with decimal points into this column.\n");
                        printf("Enter the new value for the column: %s\n", rosterColumn.ColumnName);
                        __utils_fgets_and_remove_newline(userInput.StrInput);
                        newData = userInput.StrInput;
                        retValue = update_row_data(roster.rosterNameWithPrefix, rosterColumn.ColumnName, newData, sequenceNumber);
                        if (retValue == 0)
                        {
                          system("clear");
                          printf(GREEN "Successfully updated column: " BOLD "%s" RESET GREEN " for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        else
                        {
                          system("clear");
                          printf(RED "Failed to update column: " BOLD "%s" RESET RED "for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        break;
                      case 8:
                        printf("Column: %s is of type: %s\n", rosterColumn.ColumnName, "BOOLEAN");
                        printf("Boolean columns can be entered as 1 for true or 0 for false.\n");
                        printf("You can also enter true or false.\n");
                        __utils_fgets_and_remove_newline(userInput.StrInput);
                        retValue = update_row_data(roster.rosterNameWithPrefix, rosterColumn.ColumnName, newData, sequenceNumber);
                        if (retValue == 0)
                        {
                          system("clear");
                          printf(GREEN "Successfully updated column: " BOLD "%s" RESET GREEN " for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        else
                        {
                          system("clear");
                          printf(RED "Failed to update column: " BOLD "%s" RESET RED "for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        break;
                      case 9:
                        printf("Column: %s is of type: %s\n", rosterColumn.ColumnName, "DATE");
                        printf("This means that you can only enter dates into this column.\n");
                        printf("Dates can be entered in the following formats: YYYY-MM-DD, MM-DD-YYYY, DD-MM-YYYY\n");
                        printf("Enter the new value for the column: %s\n", rosterColumn.ColumnName);
                        __utils_fgets_and_remove_newline(userInput.StrInput);
                        newData = userInput.StrInput;
                        retValue = update_row_data(roster.rosterNameWithPrefix, rosterColumn.ColumnName, newData, sequenceNumber);
                        if (retValue == 0)
                        {
                          system("clear");
                          printf(GREEN "Successfully updated column: " BOLD "%s" RESET GREEN " for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        else
                        {
                          system("clear");
                          printf(RED "Failed to update column: " BOLD "%s" RESET RED "for student: " BOLD "%d\n" RESET, rosterColumn.ColumnName, sequenceNumber);
                          sleep(2);
                          system("clear");
                        }
                        break;
                      }
                    }
                  }
                }
              // todo do stuff
              case 3:
                showingRosterData = FALSE;
                system("clear");
                puts("Returning to previous menu");
                sleep(1);
                system("clear");
                show_manage_roster_menu();
              case 4:
                showingRosterData = FALSE;
                system("clear");
                puts("Returning to main menu");
                sleep(1);
                system("clear");
                handle_main_menu();
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
        __utils_error_logger("Failed to get accurate roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get accurate.\n" RESET);
        system("clear");
        sleep(2);
        printf(RED "Please try again.\n" RESET);
        show_manage_roster_menu();
      }
    }
    else if (userInput.NumInput == 2 || strcmp(userInput.StrInput, "rename roster") == 0 || strcmp(userInput.StrInput, "rename") == 0)
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
        __utils_error_logger("Failed to get roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (userInput.NumInput == 3 || strcmp(userInput.StrInput, "delete roster") == 0 || strcmp(userInput.StrInput, "delete") == 0)
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
        __utils_error_logger("Failed to get roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (userInput.NumInput == 4 || strcmp(userInput.StrInput, "add student") == 0 || strcmp(userInput.StrInput, "add") == 0)
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
        __utils_error_logger("Failed to get roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (userInput.NumInput == 5 || strcmp(userInput.StrInput, "remove student from roster") == 0 || strcmp(userInput.StrInput, "remove") == 0)
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
        __utils_error_logger("Failed to get roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }
    else if (userInput.NumInput == 6 || strcmp(userInput.StrInput, "add column") == 0)
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
        __utils_error_logger("Failed to get roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }

    else if (userInput.NumInput == 7 || strcmp(userInput.StrInput, "delete a column") == 0)
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
        __utils_error_logger("Failed to get roster table count.", "show_manage_roster_menu", MINOR);
        printf(RED "Error: Failed to get roster table count.\n" RESET);
        wait_for_char_input();
      }
    }

    else if (userInput.NumInput == 8 || strcmp(userInput.StrInput, "help") == 0)
    {
      __utils_runtime_logger("chose to view the manage roster menus help documentation", "show_manage_roster_menu");
      system("clear");
      read_and_display_help_docs("../src/docs/help/_help_manage_roster.txt");
    }
    else if (userInput.NumInput == 9 || strcmp(userInput.StrInput, "main menu") == 0 || strcmp(userInput.StrInput, "main") == 0)
    {
      manageRosterMenuIsOpen = FALSE;
      system("clear");
      puts("Returning to main menu.");
      sleep(1);
      system("clear");
      handle_main_menu();
    }
    else
    {
      puts("Sorry, I didn't understand that.\n");
      puts("Please try again\n");
      show_manage_roster_menu();
    }
  }
}
/************************************************************************************
 * ask_which_roster_and_preform_action(): Handles the bulk of the logic for managing
 *                                        which action is to be preformed on a roster.
 *
 * Note: see usage in show_manage_roster_menu()
 ************************************************************************************/
int ask_which_roster_and_preform_action(char *action)
{

  char oldRosterName[30];
  printf("Enter the name of the roster that you would like to %s?\n", action);
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  if (strcmp(action, "rename") == 0)
  {
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("ask_which_roster_and_preform_action");
      show_manage_roster_menu();
    }
    strcpy(oldRosterName, userInput.StrInput);
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
        __utils_operation_cancelled("ask_which_roster_and_preform_action");
        show_manage_roster_menu();
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
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("ask_which_roster_and_preform_action");
      show_manage_roster_menu();
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, userInput.StrInput);
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
      printf(RED "WARNING: You are about to delete the roster:" BOLD " %s.\n" RESET, userInput.StrInput);
      printf(RED "This action is IRREVERSIBLE.\n" RESET);
      printf("Do you understand that this cannot be undone? [y/n]\n");
      puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
      __utils_fgets_and_remove_newline(confirmation);
      if (INPUT_IS_YES(confirmation))
      {
        int confirmation = confirm_action("delete", userInput.StrInput);
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
            printf(RED "Failed to delete roster:" BOLD " %s" RESET RED "please try again\n" RESET, userInput.StrInput);
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
        __utils_operation_cancelled("ask_which_roster_and_preform_action");
        show_manage_roster_menu();
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

    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("ask_which_roster_and_preform_action");
      show_manage_roster_menu();
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, userInput.StrInput);
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

      strcpy(throwAways.throwAwayStr, roster.rosterNameWithPrefix);
      system("clear");
      sleep(1);
      // do stuff
      // todo OLD:  Marsh, the problem to come back to... there are a couple things that need to happen for this action. 1. We need to search for the student. 2. handle multiple results in the event that there are people with the same name. one way to do that might be to do a for loop. add a number too each entry in the query then let the user enter the number associated with the query.  3. Then take that students firstName, lastName, and studentID and copy it into the new roster. 4. prob more complicated than this idk lol. Have fun
      globalTrigger.isAddingToStudentsTable = FALSE;
      ask_how_to_add_student();
    }
  }
  else if (strcmp(action, "remove student from roster") == 0)
  {
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("ask_which_roster_and_preform_action");
      show_manage_roster_menu();
    }

    sprintf(roster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);

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
      globalTrigger.isAddingToStudentsTable = TRUE;

      show_current_step("Remove student from roster", 2, 2);
      handle_student_deletion_logic(roster.rosterNameWithPrefix);
    }
  }
  else if (strcmp(action, "Create a column") == 0)
  {
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("ask_which_roster_and_preform_action");
      show_manage_roster_menu();
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, userInput.StrInput);
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
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("ask_which_roster_and_preform_action");
      show_manage_roster_menu();
    }
    sprintf(roster.rosterNameWithPrefix, "Roster_%s", userInput.StrInput);
    int rosterExists = check_if_table_exists(roster.rosterNameWithPrefix);
    if (rosterExists == FALSE)
    {
      system("clear");
      sleep(1);
      printf(YELLOW "The entered roster: " BOLD "%s" RESET YELLOW " does not exist please try again. \n" RESET, userInput.StrInput);
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
  __utils_fgets_and_remove_newline(userInput.StrInput);
  strcpy(colName, userInput.StrInput);

  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    __utils_operation_cancelled("create_col");
    show_manage_roster_menu();
  }
  else
  {
    int hasNonSpaceChar = has_one_non_space_char(userInput.StrInput);
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
  __utils_fgets_and_remove_newline(userInput.StrInput);
  strcpy(colName, userInput.StrInput);

  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    __utils_operation_cancelled("delete_col");
    show_manage_roster_menu();
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
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("choose_col_type");
      show_manage_roster_menu();
    }
    int numSelection = atoi(userInput.StrInput);
    showingColSelectionMenu = FALSE;
    switch (numSelection)
    {
    case 1:
    case 6: // in the event the user doesn't know. We just store it as TEXT
      strcpy(colType, "TEXT");
      system("clear");
      show_current_step("Create new column", 3, 3);
      create_col(rosterName, colType);
      break;
    case 2:
      strcpy(colType, "INTEGER");
      system("clear");
      show_current_step("Create new column", 3, 3);
      create_col(rosterName, colType);
      break;
    case 3:
      strcpy(colType, "REAL"); // the REAL data type is used to store floating-point values
      system("clear");
      show_current_step("Create new column", 3, 3);
      create_col(rosterName, colType);
      break;
    case 4:
      strcpy(colType, "BOOLEAN");
      system("clear");
      show_current_step("Create new column", 3, 3);
      create_col(rosterName, colType);
      break;
    case 5:
      strcpy(colType, "DATE");
      system("clear");
      show_current_step("Create new column", 3, 3);
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
 * Note: see usage in choose_which_col_to_sort()
 ************************************************************************************/
int check_roster_col_type(const char *rosterName, const char *colName)
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
 * Note: see usage in show_manage_roster_menu()
 ************************************************************************************/
int choose_which_col_to_sort(const char *rosterName)
{
  puts("Which column would you like to sort?");
  __utils_fgets_and_remove_newline(userInput.StrInput);

  int colExists = check_if_col_exists(rosterName, userInput.StrInput);
  if (colExists == TRUE)
  {
    int result = check_roster_col_type(rosterName, userInput.StrInput);
    strcpy(rosterColumn.ColumnName, userInput.StrInput);
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
    printf(YELLOW "The entered column: " BOLD "%s" RESET YELLOW " does not exist in roster: " BOLD "%s\n" RESET, userInput.StrInput, rosterName);
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
 * Note: see usage in choose_which_col_to_sort()
 ************************************************************************************/
int handle_col_sort_logic(const char *colName)
{
  puts("How would you like to sort this column?");
  puts("Enter the corresponding number for the sorting method.");
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  if (strcmp(rosterColumn.ColumnType, "BOOLEAN") == 0)
  {
    puts("1: True First");
    puts("2: False First");
    __utils_fgets_and_remove_newline(rosterColumn.ColumnSortingMethod);
    userInput.NumInput = atoi(rosterColumn.ColumnSortingMethod);
    if (INPUT_IS_CANCEL(rosterColumn.ColumnSortingMethod))
    {
      __utils_operation_cancelled("handle_col_sort_logic");
      show_manage_roster_menu();
    }
    switch (userInput.NumInput)
    {
    case 1:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN " in roster: " BOLD "%s" RESET GREEN " with true values first.\n " RESET, colName, roster.rosterNameWithPrefix);
      sort_roster_col_ascending(roster.rosterNameWithPrefix, colName); // sql interprets true as 1 and false as 0 so we can just sort ascending to show true first
      wait_for_char_input();
      printf("Returning to main menu.\n");
      sleep(1);
      system("clear");
      handle_main_menu();
    case 2:
      system("clear");
      printf(GREEN "Now sorting column: " BOLD "%s" RESET GREEN " in roster: " BOLD "%s" RESET GREEN " with false values first.\n " RESET, colName, roster.rosterNameWithPrefix);
      sort_roster_col_descending(roster.rosterNameWithPrefix, colName); // sql interprets true as 1 and false as 0 so we can just sort descending to show false first
      wait_for_char_input();
      printf("Returning to main menu.\n");
      sleep(1);
      handle_main_menu();
    default:
      break;
    }
  }
  else if (strcmp(rosterColumn.ColumnType, "OTHER") == 0)
  {
    puts("1: Ascending Order");
    puts("2: Descending Order");
    __utils_fgets_and_remove_newline(rosterColumn.ColumnSortingMethod);
    userInput.NumInput = atoi(rosterColumn.ColumnSortingMethod);
    if (INPUT_IS_CANCEL(rosterColumn.ColumnSortingMethod))
    {
      __utils_operation_cancelled("handle_col_sort_logic");
      show_manage_roster_menu();
    }
    switch (userInput.NumInput)
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
      break;
    default:
      break;
    }
  }
}

/************************************************************************************
 * show_roster_data_without_warning(): Displays a rosters data without the warning
 *                                     and without getting user confirmation.

 * Note: see usage in show_manage_roster_menu()
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
 * Note: see usage in show_manage_roster_menu()
 ************************************************************************************/
int show_roster_data_with_warning(const char *rosterName)
{
  printf(RED "WARNING: You are requesting to show all data in the roster: " BOLD "%s\n" RESET, rosterName);
  puts("Continuing with this operation may have the following affects:" RESET);
  puts("1: Some data may not be visible to you depending on the dimensions of your terminal" RESET);
  printf("2: Take several seconds to load all the data\n\n" RESET);
  puts("Are you sure that you'd like to continue with this operation?[y/n]");
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_CANCEL(userInput.StrInput) || INPUT_IS_NO(userInput.StrInput))
  {
    __utils_operation_cancelled("show_roster_data_with_warning");
    show_manage_roster_menu();
  }
  else if (INPUT_IS_YES(userInput.StrInput))
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
    show_manage_roster_menu();
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
  __utils_fgets_and_remove_newline(userInput.StrInput);

  Student student;
  strcpy(student.StudentID, userInput.StrInput);
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
    globalTrigger.isAddingToStudentsTable = TRUE;
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
        show_manage_roster_menu();
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

  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_YES(userInput.StrInput))
  {
    return 1;
  }
  else if (INPUT_IS_NO(userInput.StrInput))
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

// // sequence number refers to the chronological id numbers of the rows in the table
// int update_row_value(int sequenceNumber, const char *colName, const char *colType)
// {
//   __utils_fgets_and_remove_newline(userInput.StrInput);
// }