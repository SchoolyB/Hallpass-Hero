#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/c_files.h"

int settingsMenuRunning = TRUE;
int show_settings_menu(void)
{
  settingsMenuRunning = TRUE;
  while (settingsMenuRunning == TRUE)
  {
    show_current_menu("Settings Menu");
    printf("Enter the number of the option you would like to select:\n");
    printf("1. Enable/Disable Runtime Logging\n");
    printf("2. Rename Database\n");
    printf("3. Enable/Disable Color\n");
    printf("4. Main Menu\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);
    userInput.NumInput = atoi(userInput.StrInput);
    settingsMenuRunning = FALSE;
  }
  switch (userInput.NumInput)
  {
  case 1:
    __utils_runtime_logger("user entered the enable/disable runtime logging menu", "show_settings_menu");
    handle_runtime_logging_logic();
    break;
  case 2:
    __utils_runtime_logger("chose to rename the database", "show_settings_menu");
    handle_rename_db_logic(databaseInfo.currentDBName);
    break;
  case 3:
    __utils_runtime_logger("user entered the enable/disable color menu", "show_settings_menu");
    toggle_colors();
    break;
  case 4:
    system("clear");
    printf("Returning to main menu...\n");
    sleep(1);
    system("clear");
    return 0;
    break;
  default:
    system("clear");
    printf("Invalid input\n");
    sleep(1);
    system("clear");
    printf("Please try again.\n");
    show_settings_menu();
  }
}

int handle_rename_db_logic(const char *currentDBName)
{

  read_from_dir_and_check_extension("../build", ".sqlite");
  system("clear");
  show_current_step("Change database name", 1, 1);
  printf("Currently the database is named: " BOLD "%s%s\n", programSettings.databaseInfo.currentDBName, reset.colorCode);
  printf("Would you like to rename the database?[y/n]\n");
  printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);

  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_YES(userInput.StrInput))
  {
    system("clear");
    printf("Enter enter the new name for the database.\n");
    printf("Note: The name cannot contain any special characters.\n");
    printf("Note: The file extension will be added automatically.\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);

    if (strlen(userInput.StrInput) < 1)
    {
      system("clear");
      printf("%sThe entered name is too short. Please try again.%s\n", yellow.colorCode, reset.colorCode);
      sleep(1);
      system("clear");
      handle_rename_db_logic(databaseInfo.currentDBName);
    }
    else if (strlen(userInput.StrInput) > 20)
    {
      system("clear");
      printf("%sThe entered name is too long. Please try again.%s\n", yellow.colorCode, reset.colorCode);
      sleep(1);
      system("clear");
      handle_rename_db_logic(databaseInfo.currentDBName);
    }
    else
    {
      int hasSpecialChars = check_for_special_chars(userInput.StrInput);
      switch (hasSpecialChars)
      {
      case 0: // false
        strcpy(programSettings.databaseInfo.newDBName, userInput.StrInput);
        int confirmed = confirm_db_rename(programSettings.databaseInfo.newDBName);
        if (confirmed == TRUE)
        {
          sprintf(programSettings.databaseInfo.newDBName, "%s.sqlite", programSettings.databaseInfo.newDBName);
          // todo do more here
          rename(programSettings.databaseInfo.currentDBName, programSettings.databaseInfo.newDBName);
          sprintf(programSettings.databaseInfo.dbPath, "../build/%s", programSettings.databaseInfo.newDBName);
          __utils_runtime_logger("confirmed database renaming", "handle_rename_db_logic");
        }
        else if (confirmed == FALSE)
        {
          system("clear");
          __utils_runtime_logger("chose not to rename the database", "handle_rename_db_logic");
          printf("Returning to settings menu...\n");
          sleep(1);
          system("clear");
          show_settings_menu();
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
        printf("%sThe entered name contains special characters. Please try again.%s\n", yellow.colorCode, reset.colorCode);
        sleep(1);
        system("clear");

        handle_rename_db_logic(databaseInfo.currentDBName);
        break;
      default:
        __utils_error_logger("Error occurred when checking for special characters", "handle_rename_db_logic", MODERATE);
        break;
      }
    }
  }
  else if (INPUT_IS_NO(userInput.StrInput))
  {
    system("clear");
    __utils_runtime_logger("chose not to rename the database", "handle_rename_db_logic");
    printf("Returning to settings menu...\n");
    sleep(1);
    system("clear");
    show_settings_menu();
  }
  else if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    __utils_runtime_logger("cancelled operation", "handle_rename_db_logic");
    printf("%sCancelling operation...%s\n", yellow.colorCode, reset.colorCode);
    sleep(1);
    system("clear");
    return -1;
  }
  else
  {
    system("clear");
    printf("Please enter a valid input\n");
    sleep(1);
    system("clear");
    handle_rename_db_logic(currentDBName);
  }
}

int confirm_db_rename(const char *newDBName)
{
  system("clear");
  printf("Are you sure you want to rename the database to " BOLD "%s%s?[y/n]\n", newDBName, reset.colorCode);
  printf("%sTo cancel this operation enter" BOLD "'cancel'" RESET);
  __utils_fgets_and_remove_newline(userInput.StrInput);

  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    // todo finish this
  }
  else if (INPUT_IS_NO(userInput.StrInput))
  {
    __utils_runtime_logger("chose not to rename the database", "confirm_db_rename");
    return 0;
  }
  else if (INPUT_IS_YES(userInput.StrInput))
  {
    system("clear");
    __utils_runtime_logger("confirmed database renaming", "confirm_db_rename");
    printf("Renaming database...\n");

    return 1;
  }

  else
  {
    __utils_error_logger("Error occurred when renaming database", "confirm_db_rename", MODERATE);
  }
}

int handle_runtime_logging_logic(void)
{
  if (programSettings.runtimeLoggingEnabled == TRUE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD "%s enabled %s\n", green.colorCode, reset.colorCode);
    printf("Would you like to disable runtime logging?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Disabling runtime logging...\n");
      programSettings.runtimeLoggingEnabled = FALSE;
      printf("%sRuntime logging has been disabled%s\n", red.colorCode, reset.colorCode);
      __utils_runtime_logger("disabled runtime logging", "show_settings_menu");
      save_setting("runtimeLoggingEnabled", FALSE);
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
    if (INPUT_IS_NO(userInput.StrInput || INPUT_IS_CANCEL(userInput.StrInput)))
    {
      system("clear");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
  }
  else if (programSettings.runtimeLoggingEnabled == FALSE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD "%s disabled" RESET "\n");
    printf("Would you like to enable runtime logging?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Enabling runtime logging...\n");
      programSettings.runtimeLoggingEnabled = TRUE;
      printf("%sRuntime logging has been enabled%s\n", green.colorCode, reset.colorCode);
      __utils_runtime_logger("enabled runtime logging", "show_settings_menu");
      save_setting("runtimeLoggingEnabled", TRUE);
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
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("user chose not to toggle runtime logging", "show_settings_menu");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
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

int toggle_colors(void)
{
  if (programSettings.colorEnabled == TRUE)
  {
    system("clear");
    printf("Colors are currently " BOLD "%s enabled%s\n", green.colorCode, reset.colorCode);
    printf("Would you like to disable colors?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Disabling colors...\n");

      programSettings.colorEnabled = FALSE;
      printf("Colors have been disabled\n");
      // Set the global color codes to empty strings
      red.colorCode = "";
      green.colorCode = "";
      yellow.colorCode = "";
      __utils_runtime_logger("disabled colors", "toggle_colors");
      save_setting("colorEnabled", FALSE);
      sleep(2);
      system("clear");
      return 0;
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("cancelled operation", "toggle_colors");
      printf("%sCancelling operation...%s\n", yellow.colorCode, reset.colorCode);
      sleep(1);
      system("clear");
      return -1;
    }

    // This is here only because of the cancel message above
    else
    {
      system("clear");
      printf("Please enter a valid input\n");
      sleep(1);
      system("clear");
      toggle_colors();
    }
  }
  else if (programSettings.colorEnabled == FALSE)
  {
    system("clear");
    printf("Colors are currently " BOLD "disabled%s\n", reset.colorCode);
    printf("Would you like to enable colors?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Enabling colors...\n");
      programSettings.colorEnabled = TRUE;
      printf("%sColors have been enabled%s\n", green.colorCode, reset.colorCode);
      // Set the global color codes back to their respective values
      red.colorCode = RED;
      green.colorCode = GREEN;
      yellow.colorCode = YELLOW;

      __utils_runtime_logger("enabled colors", "toggle_colors");
      save_setting("colorEnabled", TRUE);
      sleep(2);
      system("clear");
      return 0;
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("user chose not to toggle colors", "toggle_colors");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("cancelled operation", "toggle_colors");
      printf("Cancelling operation...\n");
      sleep(1);
      system("clear");
      return -1;
    }
    else
    {
      system("clear");
      printf("Please enter a valid input");
      sleep(1);
      system("clear");
      toggle_colors();
    }
  }
}
int read_from_dir_and_check_extension(const char *directoryPath, const char *extension)
{
  DIR *dir;
  struct dirent *entry;
  struct stat statbuf;

  dir = opendir(directoryPath);

  if (dir == NULL)
  {
    return -1;
  }

  // Iterate through the directory entries
  while ((entry = readdir(dir)) != NULL)
  {
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, entry->d_name);

    if (stat(filePath, &statbuf) == -1)
    {
      closedir(dir);
      return -1;
    }

    // Check if the entry is a regular file with the specified extension
    if (S_ISREG(statbuf.st_mode) && strstr(entry->d_name, extension) != NULL)
    {
      strcpy(programSettings.databaseInfo.currentDBName, entry->d_name);
      return 1;
    }
  }

  closedir(dir);
  return 0;
}

int save_setting(const char *settingName, int *settingValue)
{
  printf("Would you like to save this setting?[y/n]\n");
  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_YES(userInput.StrInput))
  {
    // do stuff
    store_setting(settingName, settingValue);
  }
  else if (INPUT_IS_NO(userInput.StrInput))
  {
    system("clear");
    printf("Returning to settings menu...\n");
    sleep(1);
    system("clear");
    show_settings_menu();
  }
  else
  {
    system("clear");
    printf("Please enter a valid input\n");
    sleep(1);
    system("clear");
    save_setting(settingName, settingValue);
  }
}
// todo currently this will keep adding the same setting to the file I need to make it so that it only adds the setting if it doesn't already exist
int store_setting(const char *settingName, int settingValue)
{
  strcpy(programSettings.settingKeyName, settingName);
  programSettings.settingKeyValue = settingValue;
  printf("setting name: %s\n", programSettings.settingKeyName);
  printf("setting value: %d\n", programSettings.settingKeyValue);
  sleep(5);

  FILE *settingsFile = fopen("../build/settings.config", "a");
  if (settingsFile == NULL)
  {
    __utils_error_logger("Error opening settings file", "store_setting", MODERATE);
    perror("Error opening settings file");
    return 1;
  }
  fprintf(settingsFile, "%s=%d\n", programSettings.settingKeyName, programSettings.settingKeyValue);
  fclose(settingsFile);
}

int load_settings_from_config()
{
  // here we are going to load the settings from the settings.config file
  FILE *settingsConfig = fopen("../build/settings.config", "r");
  if (settingsConfig == NULL)
  {
    __utils_error_logger("Error opening settings file", "load_settings_from_config", MODERATE);
    perror("Error opening settings file");
    return 1;
  }
  // todo finish this
}