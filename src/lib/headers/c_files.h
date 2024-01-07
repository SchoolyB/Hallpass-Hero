#ifndef C_FILE_HEADER_h
#define C_FILE_HEADER_h

// Prototypes

/*_add_student.c*/
int add_student_to_db(void);
int get_student_first_name(void);
int get_student_last_name(void);
int ask_about_student_id(void);
void manually_set_student_id(void);
int generate_student_id(char *FirstName, char *LastName);
int confirm_generated_student_id(char *studentID);
int confirm_manually_entered_student_id(char *studentID);
int ask_to_add_to_roster(void);

/*_create_roster.c*/
int create_new_roster(void);
int get_and_confirm_roster_name(void);

/*_manage_roster.c*/
int manage_roster(void);
int ask_which_roster_and_preform_action(char *action);
int confirm_action(const char *action, ...);

/*_manage_student_db.c*/
int manage_student_db(void);
int update_student_info(void);

#endif
