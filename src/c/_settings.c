#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/c_files.h"

UserInput settingsInput;
int settingsMenuRunning = TRUE;

int show_settings_menu(void)
{
  while (settingsMenuRunning == TRUE)
  {
    show_current_menu("Settings Menu");
    puts("Enter the number of the option you would like to select:");
    puts("1. Enable/Disable Runtime Logging");
    puts("2. Rename Database");
    puts("3. Enable/Disable Color");
    puts("4. Main Menu");
    __utils_fgets_and_remove_newline(settingsInput.StrInput);
    settingsInput.NumInput = atoi(settingsInput.StrInput);
    settingsMenuRunning = FALSE;
  }
  switch (settingsInput.NumInput)
  {
  case 1:
    __utils_runtime_logger("user entered the enable/disable runtime logging menu", "show_settings_menu");
    handle_runtime_logging_logic();
    break;
  case 2:
    __utils_runtime_logger("user entered the customize database path menu", "show_settings_menu");
    // do stuff
    break;
  case 3:
    __utils_runtime_logger("user entered the enable/disable color menu", "show_settings_menu");
    // do stuff
    break;
  case 4:
    system("clear");
    puts("Returning to main menu...");
    sleep(1);
    system("clear");
    return 0;
    break;
  default:
    system("clear");
    puts("Invalid input");
    sleep(1);
    system("clear");
    puts("Please try again.");
    show_settings_menu();
  }
}

int handle_rename_db_logic(const char *currentDBName)
{
  system("clear");
  printf("Currently the database is named: " BOLD "%s" RESET "\n", currentDBName);
  puts("Would you like to rename the database?[y/n]");
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);

  __utils_fgets_and_remove_newline(settingsInput.StrInput);
  if (INPUT_IS_YES(settingsInput.StrInput))
  {
    __utils_clear_input_buffer(settingsInput.StrInput);
    system("clear");
    puts("Enter enter the new name for the database.");
    puts("Note: The name cannot contain any special characters.");
    puts("Note: The file extension will be added automatically.");
    __utils_fgets_and_remove_newline(settingsInput.StrInput);
    char newDBName[20];
    if (strlen(settingsInput.StrInput) < 1)
    {
      system("clear");
      puts(YELLOW "The entered name is too short. Please try again." RESET);
      sleep(1);
      system("clear");
      handle_rename_db_logic(currentDBName);
    }
    else if (strlen(settingsInput.StrInput) > 20)
    {
      system("clear");
      puts(YELLOW "The entered name is too long. Please try again." RESET);
      sleep(1);
      system("clear");
      handle_rename_db_logic(currentDBName);
    }
    else
    {
      int hasSpecialChars = check_for_special_chars(settingsInput.StrInput);
      switch (hasSpecialChars)
      {
      case 0: // false
        int confirmed = confirm_db_rename(newDBName);
        if (confirmed == TRUE)
        {
          __utils_runtime_logger("confirmed database renaming", "handle_rename_db_logic");
        }
        else if (confirmed == FALSE)
        {
          system("clear");
          __utils_runtime_logger("chose not to rename the database", "handle_rename_db_logic");
          puts("Returning to settings menu...");
          sleep(1);
          system("clear");
          settingsMenuRunning = TRUE;
        }
        else
        {
          // todo add more here
          __utils_error_logger("Error occurred when confirming database renaming", "handle_rename_db_logic", MODERATE);
        }
        break;
        ;
      case 1: // true
        system("clear");
        __utils_runtime_logger("entered a name with special characters", "handle_rename_db_logic");
        puts("The entered name contains special characters. Please try again.");
        sleep(1);
        system("clear");
        handle_rename_db_logic(currentDBName);
        break;
      default:
        __utils_error_logger("Error occurred when checking for special characters", "handle_rename_db_logic", MODERATE);
        break;
      }
    }
  }
}
int confirm_db_rename()
{
  // too build this func
}
int handle_runtime_logging_logic(void)
{
  if (programSettings.runtimeLoggingEnabled == TRUE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD GREEN "enabled" RESET "\n");
    puts("Would you like to disable runtime logging?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    __utils_fgets_and_remove_newline(settingsInput.StrInput);
    if (INPUT_IS_YES(settingsInput.StrInput))
    {
      system("clear");
      puts("Disabling runtime logging...");
      programSettings.runtimeLoggingEnabled = FALSE;
      puts(RED "Runtime logging has been disabled" RESET);
      __utils_runtime_logger("disabled runtime logging", "show_settings_menu");
      sleep(2);
      system("clear");
      time_t currentTime;
      time(&currentTime);

      FILE *runtimeLogFile = fopen("../logs/runtime.log", "a");
      if (runtimeLogFile == NULL)
      {
        perror("Error creating runtimeLog file");
        return 1;
      }
      fprintf(runtimeLogFile, "Logged @ %s", ctime(&currentTime));
      fprintf(runtimeLogFile, "Runtime logging has been disabled\n");
      fprintf(runtimeLogFile, "======================================================================================\n\n");
      fflush(runtimeLogFile);
      fclose(runtimeLogFile);
      return 0;
    }
    if (INPUT_IS_NO(settingsInput.StrInput || INPUT_IS_CANCEL(settingsInput.StrInput)))
    {
      system("clear");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      settingsMenuRunning = TRUE;
    }
  }
  else if (programSettings.runtimeLoggingEnabled == FALSE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD RED "disabled" RESET "\n");
    puts("Would you like to enable runtime logging?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    __utils_fgets_and_remove_newline(settingsInput.StrInput);
    if (INPUT_IS_YES(settingsInput.StrInput))
    {
      system("clear");
      puts("Enabling runtime logging...");
      programSettings.runtimeLoggingEnabled = TRUE;
      puts(GREEN "Runtime logging has been enabled" RESET);
      __utils_runtime_logger("enabled runtime logging", "show_settings_menu");
      sleep(2);
      system("clear");

      time_t currentTime;
      time(&currentTime);

      // Create the runtime.log file if it doesn't already exist
      FILE *runtimeLogFile = fopen("../logs/runtime.log", "a");
      if (runtimeLogFile == NULL)
      {
        perror("Error creating runtimeLog file");
        return 1;
      }
      fprintf(runtimeLogFile, "Logged @ %s", ctime(&currentTime));
      fprintf(runtimeLogFile, "Runtime logging has been enabled\n");
      fprintf(runtimeLogFile, "======================================================================================\n\n");
      fflush(runtimeLogFile);
      fclose(runtimeLogFile);

      return 0;
    }
    else if (INPUT_IS_NO(settingsInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("user chose not to toggle runtime logging", "show_settings_menu");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      settingsMenuRunning = TRUE;
    }
  }
  else
  {
    system("clear");
    system("Please enter a valid input");
    sleep(1);
    system("clear");
    handle_runtime_logging_logic();
  }
}