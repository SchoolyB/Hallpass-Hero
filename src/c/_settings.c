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
    puts("Enter the number of the option you would like to select:");
    puts("1. Enable/Disable Runtime Logging");
    puts("2. Rename Database");
    puts("3. Enable/Disable Color");
    puts("4. Main Menu");
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

  read_from_dir_and_check_extension("../build", ".sqlite");
  system("clear");
  show_current_step("Change database name", 1, 1);
  printf("Currently the database is named: " BOLD "%s" RESET "\n", programSettings.databaseInfo.currentDBName);
  puts("Would you like to rename the database?[y/n]");
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);

  __utils_fgets_and_remove_newline(userInput.StrInput);
  if (INPUT_IS_YES(userInput.StrInput))
  {
    system("clear");
    puts("Enter enter the new name for the database.");
    puts("Note: The name cannot contain any special characters.");
    puts("Note: The file extension will be added automatically.");
    __utils_fgets_and_remove_newline(userInput.StrInput);

    if (strlen(userInput.StrInput) < 1)
    {
      system("clear");
      puts(YELLOW "The entered name is too short. Please try again." RESET);
      sleep(1);
      system("clear");
      handle_rename_db_logic(databaseInfo.currentDBName);
    }
    else if (strlen(userInput.StrInput) > 20)
    {
      system("clear");
      puts(YELLOW "The entered name is too long. Please try again." RESET);
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
          puts("Returning to settings menu...");
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
        puts(YELLOW "The entered name contains special characters. Please try again." RESET);
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
    puts("Returning to settings menu...");
    sleep(1);
    system("clear");
    show_settings_menu();
  }
  else if (INPUT_IS_CANCEL(userInput.StrInput))
  {
    system("clear");
    __utils_runtime_logger("cancelled operation", "handle_rename_db_logic");
    puts(YELLOW "Cancelling operation..." RESET);
    sleep(1);
    system("clear");
    return -1;
  }
  else
  {
    system("clear");
    puts("Please enter a valid input");
    sleep(1);
    system("clear");
    // handle_rename_db_logic(currentDBName);
  }
}

int confirm_db_rename(const char *newDBName)
{
  system("clear");
  printf("Are you sure you want to rename the database to " BOLD "%s" RESET "?[y/n]\n", newDBName);
  puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
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
    puts("Renaming database...");
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
    printf("Currently runtime logging is " BOLD GREEN "enabled" RESET "\n");
    puts("Would you like to disable runtime logging?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
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
    if (INPUT_IS_NO(userInput.StrInput || INPUT_IS_CANCEL(userInput.StrInput)))
    {
      system("clear");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
  }
  else if (programSettings.runtimeLoggingEnabled == FALSE)
  {
    system("clear");
    show_current_step("Enable/Disable Runtime Logging", 1, 1);
    printf("Currently runtime logging is " BOLD RED "disabled" RESET "\n");
    puts("Would you like to enable runtime logging?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
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
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("user chose not to toggle runtime logging", "show_settings_menu");
      puts("Returning to settings menu...");
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
    puts("Colors are currently " BOLD GREEN "enabled" RESET);
    puts("Would you like to disable colors?[y/n]");
    puts(YELLOW "To cancel this operation enter" BOLD "'cancel'" RESET);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      puts("Disabling colors...");
#ifdef RED
#undef RED
#endif

#ifdef BLUE
#undef BLUE
#endif

#ifdef GREEN
#undef GREEN
#endif

#ifdef PURPLE
#undef PURPLE
#endif

      programSettings.colorEnabled = FALSE;
      puts("Colors have been disabled");
      __utils_runtime_logger("disabled colors", "toggle_colors");
      sleep(2);
      system("clear");
      return 0;
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("cancelled operation", "toggle_colors");
      puts(YELLOW "Cancelling operation..." RESET);
      sleep(1);
      system("clear");
      return -1;
    }

// This is here only because of the cancel message above
#ifdef YELLOW
#undef YELLOW
#endif
    else
    {
      system("clear");
      puts("Please enter a valid input");
      sleep(1);
      system("clear");
      toggle_colors();
    }
  }
  else if (programSettings.colorEnabled == FALSE)
  {
    system("clear");
    puts("Colors are currently " BOLD "disabled" RESET);
    puts("Would you like to enable colors?[y/n]");
    puts("To cancel this operation enter" BOLD "'cancel'" RESET);
    __utils_fgets_and_remove_newline(userInput.StrInput);
    if (INPUT_IS_YES(userInput.StrInput))
    {
      system("clear");
      puts("Enabling colors...");
#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define GREEN "\x1B[32m"
#define PURPLE "\x1B[35m"
#define YELLOW "\x1B[33m"
      programSettings.colorEnabled = TRUE;
      puts(GREEN "Colors have been enabled" RESET);
      __utils_runtime_logger("enabled colors", "toggle_colors");
      sleep(2);
      system("clear");
      return 0;
    }
    else if (INPUT_IS_NO(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("user chose not to toggle colors", "toggle_colors");
      puts("Returning to settings menu...");
      sleep(1);
      system("clear");
      show_settings_menu();
    }
    else if (INPUT_IS_CANCEL(userInput.StrInput))
    {
      system("clear");
      __utils_runtime_logger("cancelled operation", "toggle_colors");
      puts("Cancelling operation...");
      sleep(1);
      system("clear");
      return -1;
    }
    else
    {
      system("clear");
      puts("Please enter a valid input");
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