#ifndef DB_HH
#define DB_HH

#include <sqlite3.h>

// Declaration of C++ functions with extern "C" linkage
#ifdef __cplusplus
extern "C"
{
#endif

  // C++ function declarations
  int init_db();
  int create_new_roster_table(const char *rosterName);

#ifdef __cplusplus
} // extern "C"
#endif /* DB_HH */
#endif
