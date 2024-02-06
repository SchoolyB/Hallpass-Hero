#ifndef C_FILE_HEADER_h
#define C_FILE_HEADER_h

// Prototypes

/*_add_student.c*/
int add_student_to_student_table(void);
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
int read_from_dir_and_check_extension(const char *directoryPath, const char *extension);
int store_setting(const char *settingName, int settingValue);
int check_if_settings_config_keys_exist(const char *key);
int update_config_key_value(const char *settingName, int newSettingValue);
int load_settings_config(const char *settingName, int settingValue);
int toggle_student_id_auto_generation(void);
int check_and_load_config(void);

#endif
