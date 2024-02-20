#===============================================================================
#File Name   : startup.py
#-------------------------------------------------------------------------------
#Author      : Marshall Burns a.k.a. Schooly
#-------------------------------------------------------------------------------
#Description : This small script is ran in main.c during start up.
#              It does the following:
#1. Ensures that the settings.config file has been updated with bulk data loader settings
#2. Checks if the stats file exists and if it doesn't, it creates it and populates it.
#3. Refreshes the following stats from the stats file:
#   - "CurrentSessionStartTime"
#   - "CurrentSessionEndTime"
#   - "studentsProcessedInCurrentSession"
#4. Starts the session time and logs the start time in the stats file
#5. Increments the "CurrentSessionNumber" in the stats file
#6. Calls the bulk_data_loader.main() function which handles the bulk data loading
#7. Ends the session time and logs the end time in the stats file
#===============================================================================
import sys
import os
# Import the utils module from the lib/utils/python directory 
sys.path.append("../lib/utils/python")
import utils
import settings
import bulk_data_loader


def main():
  settings.main()
  utils.check_and_populate_stats_file()
  utils.clear_current_session_stat("CurrentSessionStartTime")
  utils.clear_current_session_stat("CurrentSessionEndTime")
  utils.clear_current_session_stat("studentsProcessedInCurrentSession")
  utils.start_and_log_session_time()
  utils.increment_stat_value("CurrentSessionNumber")
  bulk_data_loader.main()
  utils.end_and_log_session_time()
  utils.transfer_session_time_to_previous()
  utils.clear_current_session_stat("CurrentSessionStartTime")
  utils.clear_current_session_stat("CurrentSessionEndTime")
  utils.clear_current_session_stat("studentsProcessedInCurrentSession")

  

if __name__ == "__main__":
  main()