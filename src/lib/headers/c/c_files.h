/*
===============================================================================
File Name   : utils.h
-------------------------------------------------------------------------------
Author      : Marshall Burns a.k.a. Schooly
-------------------------------------------------------------------------------
Description : This header file contains all the declarations for the utility
              functions, their declarations, macros and enums.
-------------------------------------------------------------------------------
Notes       : This uses extern "C" linkage to allow the C++ functions to be
              called from C code.
===============================================================================
*/

#ifndef C_FILES_H
#define C_FILES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-START MACROS+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

// Booleans
#define FALSE 0
#define TRUE 1

// Colors

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define RESET "\x1B[0m"
typedef struct
{
  const char *colorCode;
} Color;

extern Color red;
extern Color green;
extern Color yellow;
extern Color reset;

// Error messages
// fonts
#define BOLD "\x1B[1m"
#define UNDERLINE "\x1B[4m"

//--------------------------------------------------------------------------------//

enum ErrorLevel
{
  MINOR,    // 0
  MODERATE, // 1
  CRITICAL  // 2
};
// Handling user confirmation for yes and no and cancel
#define INPUT_IS_YES(param) (strcmp(param, "y") == 0 || strcmp(param, "Y") == 0 || \
                             strcmp(param, "yes") == 0 || strcmp(param, "Yes") == 0)

#define INPUT_IS_NO(param) (strcmp(param, "n") == 0 || strcmp(param, "N") == 0 || \
                            strcmp(param, "no") == 0 || strcmp(param, "No") == 0)

#define INPUT_IS_CANCEL(param) (strcmp(param, "c") == 0 || strcmp(param, "C") == 0 || \
                                strcmp(param, "cancel") == 0 || strcmp(param, "Cancel") == 0)
//--------------------------------------------------------------------------------//

#define __utils_fgets_and_remove_newline(param) (fgets(param, sizeof(param), stdin), __utils_remove_newline_char(param))
//--------------------------------------------------------------------------------//

// Prototypes
/*_add_student.c*/
int add_student_to_student_table(void);
void ask_how_to_add_student();
int get_student_first_name(void);
int get_student_last_name(void);
int ask_about_student_id(void);
void manually_set_student_id(void);
int generate_student_id(char *FirstName, char *LastName);
int confirm_generated_student_id(char *studentID);
int confirm_manually_entered_student_id(char *studentID);
int ask_to_add_new_student_to_roster(void);
int ask_which_roster_to_add_newly_created_student(void);
int handle_last_name_truncated_menu(void);

/*_bulk_data_loader.c*/
int handle_bulk_data_loader_menu(void);
void generate_bulk_data_loader_file(const char *fileName, const char *fileStatus);
int check_if_data_file_is_empty(void);
int handle_non_empty_data_file(void);
int overwrite_existing_data_file(const char *filePath);
int clear_data_file(const char *filePath);
int __bulk_handle_data_entry();

/*_create_roster.c*/
int create_new_roster(void);
int get_and_confirm_roster_name(void);

/*_show_manage_roster_menu.c*/
int show_manage_roster_menu(void);
int ask_which_roster_and_preform_action(char *action);
int create_col(const char *rosterName, const char *colType);
int delete_col(const char *rosterName);
int choose_col_type(const char *rosterName);
int choose_which_col_to_sort(const char *rosterName);
int handle_col_sort_logic(const char *colName);
int show_roster_data_without_warning(const char *rosterName);
int show_roster_data_with_warning(const char *rosterName);
int check_if_roster_has_data(const char *rosterName);
int handle_student_deletion_logic(const char *rosterName);
int confirm_action(const char *action, ...);

/*_manage_student_db.c*/
int manage_student_db(void);
int update_student_info(void);

/*_search.c*/
int search_for_student(void);

/*_settings.c*/
int show_settings_menu(void);
int handle_rename_db_logic(const char *currentDBName);
int confirm_db_rename(const char *newDBName);
int handle_runtime_logging_logic(void);
int toggle_colors(void);
int store_setting(const char *settingName, int settingValue);
int check_if_settings_config_keys_exist(const char *key);
int update_config_key_value(const char *settingName, int newSettingValue);
int load_settings_config(const char *settingName, int settingValue);
int toggle_student_id_auto_generation(void);
int check_and_load_config(void);

// Declaration of utility functions
int __utils_error_logger(char *error_message, char *function, enum ErrorLevel level);
int __utils_runtime_logger(char *action, char *functionName);
void __utils_operation_cancelled(const char *functionName);
int __utils_check_for_sqlite_db(void);
int __utils_check_for_bulk_loader_data_file(void);
int read_from_dir_and_check_extension(const char *directoryPath, const char *extension);
int search_for_prefix_in_file_name(const char *targetWord);
void show_bulk_loader_current_status(char *str);
void show_current_menu(char *str);
void show_current_step(char *str, int currentStep, int totalSteps);
void __utils_remove_newline_char(char *param);
void __utils_clear_input_buffer();
int list_all_students(void);
int has_one_non_space_char(const char *str);
int wait_for_char_input(void);
int search_for_student(void);
int read_and_display_help_docs(const char *helpFile);

// just some variables used when Idk what do name a variable or when I need to pass a variable to a function but I don't need the value
typedef struct
{
  char throwAwayStr[32];
  int throwAwayInt;
} ThrowAways;

// this is only used to set the passed in tableName of the check_if_table_exists() function
typedef struct
{
  char TableName[50];
} DesiredTableName;
// this type allows the same input through each C source file. As opposed to constantly declaring char arrays
typedef struct
{
  int NumInput;
  char StrInput[50];
} UserInput;

// typedef struct
// {
//   char RelationshipToStudent[20];
//   char FirstName[30];
//   char LastName[30];
//   char PhoneNumber[20]; // need to hash this
// } EmergencyContact;

// typedef struct
// {
//   char BirthDate[20]; // need to hash this
//   char Address[50];   // need to hash this
//   EmergencyContact EmergencyContact;
// } PrivateInfo;

typedef struct
{
  char FirstName[30];
  char LastName[30];
  char StudentID[20];
  char Email[50];
  // PrivateInfo PrivateInfo;
  // Program CurrentProgram;

} Student;

typedef struct
{
  int fileNumIota;
  char FileName[32];
  char FilePath[32];
  char FileSizeInBytes[20]; // may not need this
} JSONDataFile;

typedef struct
{
  char ColumnName[50];
  char ColumnType[20];
  char ColumnSortingMethod[20];

} Column;

typedef struct
{
  char rosterName[50];
  char rosterNameWithPrefix[60];
} Roster;

typedef struct
{
  int studentCreationInterrupted;
  int isAddingToStudentsTable; // modifies functions depending on if the user is adding students directly to the "students" table or to a roster

  int isUsingBulkLoader;                // this trigger only affects what information from a directory is stored into the JSONDataFile the programSettings.databaseInfo struct respectively
  int isBulkLoadingDataToStudentsTable; // will modify functions to handle bulk data loading. allowing me to use the same functions for both bulk data loading to a roster and to the students table
} GlobalTrigger;

typedef struct
{
  const char *dbPath;
  char newDBPath[50];
  char currentDBName[20];
  char newDBName[20];
} DatabaseInfo;

typedef struct
{
  char settingKeyName[50];
  int settingKeyValue;
  int autoStudentIDGenerationEnabled;
  int colorEnabled;
  int runtimeLoggingEnabled;
  int skipBulkLoaderInfoConfirmation;
  int skipBulkDataLoaderEntryPrompt; // might delete this
  DatabaseInfo databaseInfo;
} ProgramSettings;

extern JSONDataFile jsonDataFile;       // initialized in main.c
extern Roster roster;                   // initialized in main.c
extern ThrowAways throwAways;           // initialized in main.c
extern UserInput userInput;             // initialized in main.c
extern DatabaseInfo databaseInfo;       // initialized in main.c
extern ProgramSettings programSettings; // initialized in main.c
extern GlobalTrigger globalTrigger;     // initialized in main.c

#endif /* UTILS_H */
