/*
===============================================================================
File Name   : _settings.c
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This source file contains all functions related to the settings menu
===============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../lib/headers/utils.h"
#include "../lib/headers/c_files.h"

int settingsMenuRunning = TRUE;

/************************************************************************************
 * show_settings_menu(): Displays the settings menu and handles all logic for
 *                       the settings menu.
 *
 * See usage in: main.c
 ************************************************************************************/
int show_settings_menu(void)
{
  settingsMenuRunning = TRUE;
  __utils_runtime_logger("entered settings menu", "show_settings_menu");
  __utils_check_for_sqlite_db();
  while (settingsMenuRunning == TRUE)
  {
    show_current_menu("Settings Menu");
    printf("Enter the number of the option you would like to select:\n");
    printf("1. Enable/Disable Runtime Logging\n");
    printf("2. Rename Database\n");
    printf("3. Enable/Disable Color\n");
    printf("4. Enable/Disable Student ID Generation\n");
    printf("5. Main Menu\n");
    __utils_fgets_and_remove_newline(userInput.StrInput);
    userInput.NumInput = atoi(userInput.StrInput);
    settingsMenuRunning = FALSE;
  }
  switch (userInput.NumInput)
  {
  case 1:
    __utils_runtime_logger("user entered the enable/disable runtime logging menu", "show_settings_menu");
    check_and_load_config();
    handle_runtime_logging_logic();
    break;
  case 2:
    __utils_runtime_logger("chose to rename the database", "show_settings_menu");
    handle_rename_db_logic(databaseInfo.currentDBName);
    break;
  case 3:
    __utils_runtime_logger("user entered the enable/disable color menu", "show_settings_menu");

    check_and_load_config();
    toggle_colors();
    break;
  case 4:
    __utils_runtime_logger("user entered the enable/disable automatic student ID generation menu", "show_settings_menu");
    check_and_load_config();
    toggle_student_id_auto_generation();
    break;
  case 5:
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

/************************************************************************************
 * handle_rename_db_logic(): Handles the logic for renaming the database.
 *
 * See usage in: show_settings_menu()
 ************************************************************************************/
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
    __utils_operation_cancelled("handle_rename_db_logic");
    sleep(1);
    system("clear");
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

/************************************************************************************
 * confirm_db_rename(): Prompts the user to confirm the database renaming operation.
 *
 * See usage in: handle_rename_db_logic()
 ************************************************************************************/
int confirm_db_rename(const char *newDBName)
{
  system("clear");
  printf("Are you sure you want to rename the database to " BOLD "%s%s?[y/n]\n", newDBName, reset.colorCode);
  printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
  __utils_fgets_and_remove_newline(userInput.StrInput);

  if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    __utils_operation_cancelled("confirm_db_rename");
    __utils_runtime_logger("cancelled operation", "confirm_db_rename");
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

/************************************************************************************
 * handle_runtime_logging_logic(): Handles the logic for enabling/disabling runtime
 *                                 logging....duh
 *
 * See usage in: show_settings_menu()
 ************************************************************************************/
int handle_runtime_logging_logic(void)
{
  if (programSettings.runtimeLoggingEnabled == TRUE)
  {
    // TODO this is crashing somewhere around here....need to fix
    system("clear");
    show_current_step("Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD "%senabled %s\n", green.colorCode, reset.colorCode);
    printf("Would you like to disable runtime logging?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);

    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("handle_runtime_logging_logic");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Disabling runtime logging...\n");
      programSettings.runtimeLoggingEnabled = FALSE;
      printf("%sRuntime logging has been disabled%s\n", red.colorCode, reset.colorCode);
      __utils_runtime_logger("disabled runtime logging", "show_settings_menu");
      store_setting("runtimeLoggingEnabled", FALSE);
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
    else
    {
      system("clear");
      printf("Please enter a valid input\n");
      sleep(1);
      system("clear");
      handle_runtime_logging_logic();
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
    show_current_step("Enable runtime logging", 1, 1);
    printf("Currently runtime logging is " BOLD "%sdisabled%s\n", red.colorCode, reset.colorCode);
    printf("Would you like to enable runtime logging?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("handle_runtime_logging_logic");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Enabling runtime logging...\n");
      programSettings.runtimeLoggingEnabled = TRUE;
      printf("%sRuntime logging has been enabled%s\n", green.colorCode, reset.colorCode);
      __utils_runtime_logger("enabled runtime logging", "show_settings_menu");
      store_setting("runtimeLoggingEnabled", TRUE);
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
    else
    {
      system("clear");
      printf("Please enter a valid input\n");
      sleep(1);
      system("clear");
      handle_runtime_logging_logic();
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

/************************************************************************************
 * toggle_colors(): Handles the logic for toggling the colorEnabled setting.
 *
 * See usage in: show_settings_menu()
 ************************************************************************************/
int toggle_colors(void)
{
  if (programSettings.colorEnabled == TRUE)
  {
    system("clear");
    printf("Colors are currently " BOLD "%senabled%s\n", green.colorCode, reset.colorCode);
    printf("Would you like to disable colors?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      printf("Disabling colors...\n");

      programSettings.colorEnabled = FALSE;
      printf("Colors have been %sdisabled%s\n", red.colorCode, reset.colorCode);
      // Set the global color codes to empty strings
      red.colorCode = "";
      green.colorCode = "";
      yellow.colorCode = "";
      __utils_runtime_logger("disabled colors", "toggle_colors");
      store_setting("colorEnabled", FALSE);
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
      __utils_operation_cancelled("toggle_colors");
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
    red.colorCode = "";
    green.colorCode = "";
    yellow.colorCode = "";
    system("clear");
    printf("%sColors are currently " BOLD "disabled%s\n", red.colorCode, reset.colorCode);
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
      store_setting("colorEnabled", TRUE);
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

      __utils_operation_cancelled("toggle_colors");
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

/************************************************************************************
 * read_from_dir_and_check_extension(): Reads from the directory and checks if the
 *                                      file has the specified extension.
 *
 * See usage in: handle_rename_db_logic() & main.c
 ************************************************************************************/
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

/************************************************************************************
 * store_setting(): Used to store the passed in settings value into the key-value pair
 *                  in the settings.config file.
 *
 ************************************************************************************/
int store_setting(const char *settingName, int settingValue)
{
  strcpy(programSettings.settingKeyName, settingName);
  programSettings.settingKeyValue = settingValue;
  FILE *settingsFile = fopen("../build/settings.config", "a");
  if (settingsFile == NULL)
  {
    __utils_error_logger("Error opening settings file", "store_setting", MODERATE);
    perror("Error opening settings file");
    return 1;
  }
  int settingExists = check_if_settings_config_keys_exist(programSettings.settingKeyName);
  int runtimeLoggingEnabledKeyExists = check_if_settings_config_keys_exist("runtimeLoggingEnabled");
  if (settingExists == TRUE)
  {
    update_config_key_value(programSettings.settingKeyName, programSettings.settingKeyValue);
    fclose(settingsFile);
  }
  else if (settingExists == FALSE)
  {
    fprintf(settingsFile, "%s=%d\n", programSettings.settingKeyName, programSettings.settingKeyValue);
    fclose(settingsFile);
  }
}

/************************************************************************************
 * check_if_settings_config_keys_exist(): Checks if the passed in key exists in the
 *                                        settings.config file.
 * See usage in: store_setting()
 ************************************************************************************/
int check_if_settings_config_keys_exist(const char *key)
{
  FILE *settingsFile = fopen("../build/settings.config", "r");
  if (settingsFile == NULL)
  {
    __utils_error_logger("Error opening settings file", "store_setting", MODERATE);
    perror("Error opening settings file");
    return 1;
  }
  /*Will need to change the bytes in this array in
  the even that I add more settings options but since
  there are only 2 for now, this works fine.
  Marshall Burns Feb 1st 2024*/
  char line[100];
  int keyLength = strlen(key);
  int keyExists = FALSE; // initialize to false before checking

  // while the file is able to be read, read the lines upto the end of the file
  while (fgets(line, sizeof(line), settingsFile) != NULL)
  {
    // if the lines read from the file contain BOTH 2 keys then return true.
    if (strncmp(line, key, keyLength) == 0)
    {
      keyExists = TRUE;
      break;
    }
  }
  fclose(settingsFile);

  if (keyExists)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/************************************************************************************
 * update_config_key_value(): Updates the value of the passed in key in the
 *                            settings.config file.
 * See usage in: store_setting()
 ************************************************************************************/
int update_config_key_value(const char *settingName, int newSettingValue)
{
  const char *filename = "../build/settings.config";
  FILE *settingsFile = fopen(filename, "r+");
  if (settingsFile == NULL)
  {
    __utils_error_logger("Error opening settings file", "update__config_key_value", MODERATE);
    perror("Error opening settings file");
    return 0;
  }

  char line[100];
  int keyLength = strlen(settingName);
  int keyFound = FALSE;

  // Read and write to the same file
  while (fgets(line, sizeof(line), settingsFile) != NULL)
  {
    if (strncmp(line, settingName, keyLength) == 0)
    {
      // Found the key, update the line with the new value
      fseek(settingsFile, -strlen(line), SEEK_CUR); // Move the cursor back to the beginning of the line
      fprintf(settingsFile, "%s=%d\n", settingName, newSettingValue);
      keyFound = TRUE;
      break; // No need to continue searching
    }
  }
  fclose(settingsFile);
  if (keyFound)
  {
    return TRUE;
  }
  else
  {

    return FALSE;
  }
}
/************************************************************************************
 * load_settings_config(): Loads the settings.config file by extracting the value of
 *                         the passed in key. and stores the key and value in the
 *                         programSettings struct.
 * See usage in: main.c
 ************************************************************************************/
int load_settings_config(const char *settingName, int settingValue)
{
  const char *filename = "../build/settings.config";
  FILE *settingsFile = fopen(filename, "r");
  if (settingsFile == NULL)
  {
    perror("Error opening settings file");
    return FALSE;
  }

  char line[100];
  int keyLength = strlen(settingName);
  int keyFound = FALSE;

  // Read the file
  while (fgets(line, sizeof(line), settingsFile) != NULL)
  {
    if (strncmp(line, settingName, keyLength) == 0)
    {
      printf("Scanning line: %s\n", line);
      sscanf(line, "%*[^=]=%d", &settingValue); // idk how this works but it does shoutout ChatGPT
      strcpy(programSettings.settingKeyName, settingName);
      programSettings.settingKeyValue = settingValue;
      keyFound = TRUE;
      break; // No need to continue searching
    }
  }
  fclose(settingsFile);

  if (keyFound)
  {
    return programSettings.settingKeyValue;
  }
  else
  {
    return -1;
  }
}

/************************************************************************************
 * toggle_student_id_auto_generation(): Handles the logic for toggling the
 *                                      autoStudentIDGenerationEnabled setting.
 *
 * See usage in: show_settings_menu(),  _add_student.c. and _show_manage_roster_menus.c
 ************************************************************************************/
int toggle_student_id_auto_generation(void)
{
  if (programSettings.autoStudentIDGenerationEnabled == TRUE)
  {
    system("clear");
    show_current_step("Disable Student ID Generation", 1, 1);
    printf("Currently automatic student ID generation is " BOLD "%senabled%s\n", green.colorCode, reset.colorCode);
    printf("Would you like to disable automatic student ID generation?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);

    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("toggle_student_id_auto_generation");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      programSettings.autoStudentIDGenerationEnabled = FALSE;
      store_setting("autoStudentIDGenerationEnabled", FALSE);
      printf("Automatic student ID generation has been %s disabled%s\n", red.colorCode, reset.colorCode);
      __utils_runtime_logger("disabled automatic student ID generation", "toggle_student_id_auto_generation");
      sleep(2);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
  }
  else if (programSettings.autoStudentIDGenerationEnabled == FALSE)
  {
    system("clear");
    show_current_step("Enable Student ID Generation", 1, 1);
    printf("Currently automatic student ID generation is " BOLD "%sdisabled%s\n", red.colorCode, reset.colorCode);
    printf("Would you like to enable automatic student ID generation?[y/n]\n");
    printf("%sTo cancel this operation enter" BOLD "'cancel'%s\n", yellow.colorCode, reset.colorCode);
    __utils_fgets_and_remove_newline(userInput.StrInput);

    if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      __utils_operation_cancelled("toggle_student_id_auto_generation");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      programSettings.autoStudentIDGenerationEnabled = TRUE;
      store_setting("autoStudentIDGenerationEnabled", TRUE);
      printf("Automatic student ID generation has been %s enabled%s\n", green.colorCode, reset.colorCode);
      __utils_runtime_logger("enabled automatic student ID generation", "toggle_student_id_auto_generation");
      sleep(2);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      printf("Returning to settings menu...\n");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
  }
}

/************************************************************************************
 * check_and_load_config(): Checks if the settings.config file exists and loads the
 *                          settings.config file.
 *
 * See usage in: show_settings_menu() and main.c
 ************************************************************************************/
int check_and_load_config(void)
{
  int foundSettingsConfig = read_from_dir_and_check_extension("../build", ".config");

  if (foundSettingsConfig == TRUE)
  {
    int runtimeLoggingLoaded = load_settings_config("runtimeLoggingEnabled", programSettings.runtimeLoggingEnabled);
    switch (runtimeLoggingLoaded)
    {
    case 0:
      programSettings.runtimeLoggingEnabled = FALSE;
      break;
    case 1:
      programSettings.runtimeLoggingEnabled = TRUE;
      break;
    }

    load_settings_config("colorEnabled", programSettings.colorEnabled);
    int colorEnabledLoaded = load_settings_config("colorEnabled", programSettings.colorEnabled);
    switch (colorEnabledLoaded)
    {
    case 0:
      programSettings.colorEnabled = FALSE;
      break;
    case 1:
      programSettings.colorEnabled = TRUE;
      break;
    }

    load_settings_config("autoStudentIDGenerationEnabled", programSettings.autoStudentIDGenerationEnabled);
    int autoStudentIDGenerationEnabledLoaded = load_settings_config("autoStudentIDGenerationEnabled", programSettings.autoStudentIDGenerationEnabled);

    switch (autoStudentIDGenerationEnabledLoaded)
    {
    case 0:
      programSettings.autoStudentIDGenerationEnabled = FALSE;
      break;
    case 1:
      programSettings.autoStudentIDGenerationEnabled = TRUE;
      break;
    }
  }
  else if (foundSettingsConfig == FALSE)
  {
    FILE *settingsConfig = fopen("../build/settings.config", "a");
    if (settingsConfig == NULL)
    {
      perror("Error creating settingsConfig file");
      return 1;
    }
    // setting the default values for the settings.config file if it doesn't already exist
    programSettings.runtimeLoggingEnabled = TRUE;
    programSettings.colorEnabled = TRUE;
    programSettings.autoStudentIDGenerationEnabled = TRUE;
    fprintf(settingsConfig, "# This config file has been generated by the Hallpass Hero CLI tool.\n# It is unadvised to edit this file manually.\n# To change the settings of your Hallpass Hero CLI tool, please use the settings menu.\n\n\n");
    fprintf(settingsConfig, "runtimeLoggingEnabled=%d\n", programSettings.runtimeLoggingEnabled);
    fprintf(settingsConfig, "colorEnabled=%d\n", programSettings.colorEnabled);
    fclose(settingsConfig);
  }
}