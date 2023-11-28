#ifndef C_FILE_HEADER_h
#define C_FILE_HEADER_h
int create_new_roster(void);
int manage_roster(void);
int add_student_to_db(void);
int search_for_student(void);
int show_help_menu(void);
int get_and_confirm_roster_name(void);
int get_student_first_name(void);
int get_student_last_name(void);
int ask_about_student_id(void);
void manually_set_student_id(void);

// int generate_student_id(char *FirstName, char *LastName);
int confirm_student_id(char *StudentID);
#endif