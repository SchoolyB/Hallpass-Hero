#===============================================================================
#File Name   : utils.py
#-------------------------------------------------------------------------------
#Author      : Marshall Burns a.k.a. Schooly
#-------------------------------------------------------------------------------
#Description : This file contains several utility functions, variables and 
#              enums that are used throughout the 'Python'
#              portion of the source code.
#===============================================================================

import os
import time
import csv
import sys
from enum import Enum

dataFilePath = "../build/data/active_data.csv"
settingsConfigPath = "../../build/settings.config"
statsLogFilePath = "../../logs/stats.log"
errorLogFilePath = "../../logs/errors.log"
runtimeLogFilePath = "../../logs/runtime.log"

sys.path.append("../../../python/")
import settings
import bulk_data_loader

class ErrorLevel(Enum):
    MINOR = 0
    MODERATE = 1
    CRITICAL = 2

class Font:
    BOLD = "\033[1m"
    UNDERLINE = "\033[4m"
    END = "\033[0m"
######################################################################
# clear(): clears the terminal screen
#
######################################################################
def clear():
    # this only works on unix based systems
    os.system("clear")


######################################################################
# check_if_data_file_exists():Checks if the data file exists
#                             and if it doesn't, it creates it
# 
# see usage in check_and_populate_stats_file()                          
######################################################################
def check_if_stats_file_exists():
    if os.path.exists(statsLogFilePath):
        return True
    else:
        file = open(statsLogFilePath, "x")
        file.close() 


######################################################################
# get_and_set_stat_value(): Reads the stats.log file and sets the
#                           values of the statistics in the bulk_data_loader
#                           module
#
# see usage in bulk_data_loader.py
######################################################################
def get_and_set_stat_value(keyName):
    __utils_runtime_logger("attempted to get and set statistics values", "get_and_set_stat_value")
    statsFileHasNames = check_and_populate_stats_file()
    if(statsFileHasNames == True):
        try:
            with open(statsLogFilePath, "r") as file:
                for line in file:
                    keyName, value = map(str.strip, line.split("=", 1))
                    if keyName == "TotalStudentsProcessed":
                        bulk_data_loader.Statistic.TotalStudentsProcessed = int(value)
                        return bulk_data_loader.Statistic.TotalStudentsProcessed
                    elif keyName == "TotalSessionsCompleted":
                        bulk_data_loader.Statistic.TotalSessionsCompleted = int(value)
                        return bulk_data_loader.Statistic.TotalSessionsCompleted
                    elif keyName == "CurrentSessionNumber":
                        bulk_data_loader.Statistic.CurrentSessionNumber = int(value)
                        return bulk_data_loader.Statistic.CurrentSessionNumber
                    elif keyName == "studentsProcessedInCurrentSession":
                        bulk_data_loader.Statistic.studentsProcessedInCurrentSession = int(value)
                        return bulk_data_loader.Statistic.studentsProcessedInCurrentSession
                    elif keyName == "CurrentSessionStartTime":
                        bulk_data_loader.Statistic.CurrentSessionStartTime = value
                        return bulk_data_loader.Statistic.CurrentSessionStartTime
                    elif keyName == "CurrentSessionEndTime":
                        bulk_data_loader.Statistic.CurrentSessionEndTime = value
                        return bulk_data_loader.Statistic.CurrentSessionEndTime      
                    elif keyName == "PreviousSessionStartTime":
                        bulk_data_loader.Statistic.PreviousSessionStartTime = value
                        return bulk_data_loader.Statistic.PreviousSessionStartTime
                    elif keyName == "PreviousSessionEndTime":
                        bulk_data_loader.Statistic.PreviousSessionEndTime = value
                        return bulk_data_loader.Statistic.PreviousSessionEndTime  
        except FileNotFoundError as error:
            __utils_error_logger(f"File Not Found: {statsLogFilePath}", "get_and_set_stat_value", ErrorLevel.MINOR)
            print(f"File Not Found: {statsLogFilePath}")
            return -1
    elif(statsFileHasNames == False):
        __utils_error_logger(f"File Not Found: {statsLogFilePath}", "get_and_set_stat_value", ErrorLevel.MINOR)
        print(f"File Not Found: {statsLogFilePath}")
        return -1

######################################################################
# start_and_log_session_time(): Logs the start time of the current session
#                               in the stats.log file
#
# see usage in startup.py
######################################################################
def start_and_log_session_time():
    __utils_runtime_logger("attempted to start and log session start time", "start_and_log_session_time")
    try:
        with open(statsLogFilePath, "r") as file:
            lines = file.readlines()
        for i, line in enumerate(lines):
            if "CurrentSessionStartTime" in line:
                lines[i] = f"CurrentSessionStartTime={time.ctime()}\n"
                with open(statsLogFilePath, "w") as file:
                    file.writelines(lines)
                return time.ctime()
    except FileNotFoundError as error:
        __utils_error_logger(f"File Not Found: {statsLogFilePath}", "start_and_log_session_time", ErrorLevel.MINOR)
        print(f"File Not Found: {statsLogFilePath}")
        return -1
    
######################################################################
# end_and_log_session_time(): Logs the end time of the current session
#                              in the stats.log file
#
# see usage in startup.py
######################################################################
def end_and_log_session_time():
    __utils_runtime_logger("attempted to end and log session end time", "end_and_log_session_time")
    try:
        with open(statsLogFilePath, "r") as file:
            lines = file.readlines()
        for i, line in enumerate(lines):
            if "CurrentSessionEndTime" in line:
                lines[i] = f"CurrentSessionEndTime={time.ctime()}\n"
                with open(statsLogFilePath, "w") as file:
                    file.writelines(lines)
                return time.ctime()
    except FileNotFoundError as error:
        __utils_error_logger(f"File Not Found: {statsLogFilePath}", "end_and_log_session_time", ErrorLevel.MINOR)
        print(f"File Not Found: {statsLogFilePath}")
        return -1


######################################################################
# clear_current_session_stat(): Clears the value of the passed in key
#                               in the stats.log file
#
# see usage in startup.py
######################################################################
def clear_current_session_stat(keyName):
    __utils_runtime_logger("attempted to clear current session stat", "clear_current_session_stat")
    try:
        with open(statsLogFilePath, "r") as file:
            lines = file.readlines()
        for i, line in enumerate(lines):
            if keyName in line:
                lines[i] = f"{keyName}=0\n"
                with open(statsLogFilePath, "w") as file:
                    file.writelines(lines)
                return 0
    except FileNotFoundError as error:
        __utils_error_logger(f"File Not Found: {statsLogFilePath}", "clear_current_session_stat", ErrorLevel.MINOR)
        print(f"File Not Found: {statsLogFilePath}")
        return -1

######################################################################
# get_session_time(): Gets the value of the passed from key in the stats.log
#                     file
#
# Note: I only use this to get the session start and end times
#       But technically it can be used to get any value from the stats.log file
#
# see usage in transfer_session_time_to_previous()
######################################################################
def get_session_time(keyName):
    __utils_runtime_logger("attempted to get session time", "get_session_time")
    try:
        with open(statsLogFilePath, "r") as file:
            for line in file:
                key, value = map(str.strip, line.split("=", 1))
                if keyName in line:
                    return value
    except FileNotFoundError as error:
        __utils_error_logger(f"File Not Found: {statsLogFilePath}", "get_session_time", ErrorLevel.MINOR)
        print(f"File Not Found: {statsLogFilePath}")
        return -1

######################################################################
# transfer_session_time_to_previous(): Transfers the time values from the
#                                      current session to the previous session
#                                      in the stats.log file
#
# see usage in startup.py
######################################################################
def transfer_session_time_to_previous():
    __utils_runtime_logger("attempted to transfer current session time to previous session time", "transfer_session_time_to_previous")
    try:
        with open(statsLogFilePath, "r") as file:
            lines = file.readlines()
        for i, line in enumerate(lines):
            if "PreviousSessionStartTime" in line:
                bulk_data_loader.Statistic.CurrentSessionEndTime = get_session_time("CurrentSessionEndTime")
                bulk_data_loader.Statistic.CurrentSessionStartTime = get_session_time("CurrentSessionStartTime")
                lines[i] = f"PreviousSessionStartTime={bulk_data_loader.Statistic.CurrentSessionStartTime}\n"
            elif "PreviousSessionEndTime" in line:
                lines[i] = f"PreviousSessionEndTime={bulk_data_loader.Statistic.CurrentSessionEndTime}\n"
            with open(statsLogFilePath, "w") as file:
                file.writelines(lines)
        return 0
    except FileNotFoundError as error:
        __utils_error_logger(f"File Not Found: {statsLogFilePath}", "transfer_session_time_to_previous", ErrorLevel.MINOR)
        print(f"File Not Found: {statsLogFilePath}")
        return -1

######################################################################
# check_and_populate_stats_file(): Checks if the stats.log file exists
#                                  and if it doesn't, it creates it and
#                                  populates it with the following stats:
#                                  - TotalStudentsProcessed
#                                  - TotalSessionsCompleted
#                                  - CurrentSessionNumber
#                                  - studentsProcessedInCurrentSession
#                                  - CurrentSessionStartTime
#                                  - CurrentSessionEndTime
#                                  - PreviousSessionStartTime
#                                  - PreviousSessionEndTime
#
# see usage in startup.py
######################################################################
def check_and_populate_stats_file():
    fileExists = check_if_stats_file_exists()
    if(fileExists == True):
        pass
    else:
        try:
            with open(statsLogFilePath, "r") as file:
                content = file.read()
                if ("TotalStudentsProcessed" in content 
                and "TotalSessionsCompleted" in content 
                and "CurrentSessionNumber" in content
                and "studentsProcessedInCurrentSession" in content
                and "CurrentSessionStartTime" in content
                and "CurrentSessionEndTime" in content
                and "PreviousSessionStartTime" in content
                and "PreviousSessionEndTime" in content):
                    return True
                else:
                    try:
                        with open(statsLogFilePath, "w") as file:
                            file.write("TotalStudentsProcessed=0\n")
                            file.write("TotalSessionsCompleted=0\n")
                            file.write("CurrentSessionNumber=0\n")
                            file.write("studentsProcessedInCurrentSession=0\n")
                            file.write("CurrentSessionStartTime=0\n")
                            file.write("CurrentSessionEndTime=0\n")
                            file.write("PreviousSessionStartTime=0\n")
                            file.write("PreviousSessionEndTime=0\n")
                            return True
                    except error as e:
                        __utils_error_logger(f"Error: {e}", "check_and_populate_stats_file()", ErrorLevel.CRITICAL)
                        return False 
        except FileNotFoundError as error:
            print(f"File Not Found: {settingsConfigPath}")
            return -1
    

######################################################################
# increment_stat_value(): Increments the value of the passed in key
#                         in the stats.log file
#
# see usage in bulk_data_loader.py & startup.py
######################################################################
def increment_stat_value(keyName):
    try:
        with open(statsLogFilePath, "r") as file:
            lines = file.readlines()
        for i, line in enumerate(lines):
            if keyName in line:
                key, value = map(str.strip, line.split("=", 1))
                value = int(value)
                value += 1
                lines[i] = f"{key}={value}\n"
                found = True
                break
        with open(statsLogFilePath, "w") as file:
            file.writelines(lines)
        __utils_runtime_logger(f"incremented {keyName} in stats.log", "increment_stat_value")

    except FileNotFoundError as error:
        print(f"File Not Found: {statsLogFilePath}")
        return -1
   
######################################################################
# check_file_for_valid_csv(): Helper function to make sure the passed
#                              in file contains valid csv
#
# see usage in 
######################################################################
def check_file_for_valid_csv(filePath):
    try:
        with open(filePath, "r") as file:
            csvFile = csv.reader(file)
            for row in csvFile:
                if len(row) == 0:
                    return False
                else:
                    return True
    except FileNotFoundError as error:
        print(f"File Not Found: {dataFilePath}")
        return False


######################################################################
# __utils_error_logger(): Used to log error messages to the error log
#                        file
#
######################################################################
def __utils_error_logger(errorMessage, function, errorLevel):
    try:
        with open(errorLogFilePath, "a") as file:    
            match errorLevel:
                case ErrorLevel.MINOR: 
                   file.write(f"From Python Logged @ {time.ctime()}\n")
                   file.write(f"Minor Error: {errorMessage} in function call {function}()\n")
                   file.write("======================================================================================\n")
                   file.close()
                   return ErrorLevel.MINOR
                case ErrorLevel.MODERATE:
                    file.write(f"From Python Logged @ {time.ctime()}\n")
                    file.write(f"Moderate Error: {errorMessage} in function call {function}()\n")
                    file.write("======================================================================================\n")
                    file.close()
                    return ErrorLevel.MODERATE
                case ErrorLevel.CRITICAL:
                    file.write(f"From Python Logged @ {time.ctime()}\n")
                    file.write(f"Critical Error: {errorMessage} in function call {function}()\n")
                    file.write("======================================================================================\n")
                    file.close()
                    print(f"{settings.Colors.RED}Critical Error occurred @ {time.ctime()}: For more information see logs/errors.log{settings.Colors.RESET}\n")
                    exit() #exits the Python script not the entire program
    except FileNotFoundError as error:
        print(f"File Not Found: {errorLogFilePath}")
        return -1 

######################################################################
# __utils_runtime_logger(): Logs runtime activity
#
######################################################################               
def __utils_runtime_logger(action, function):
    if(settings.GlobalSettings.RuntimeLoggingEnabled == 1):
        try:
            with open(runtimeLogFilePath, "a") as file:
                file.write(f"From Python Logged @ {time.ctime()}\n")
                # Note: the action param isn't always a user action. It can be an action the program takes as well
                file.write(f"User Action: User {action} in function call {function}()\n")
                file.write("======================================================================================\n")
                file.close()
        except:
            __utils_error_logger("Could not write to runtime log file", "__utils_runtime_logger", ErrorLevel.CRITICAL)
    elif( settings.GlobalSettings.RuntimeLoggingEnabled == 0):
        return 0

######################################################################
# __utils_operation_cancelled(): Logs when an operation is cancelled
#                                and displays a message to the user
#
######################################################################
def __utils_operation_cancelled(functionName):
    clear()
    print(f"{settings.Colors.YELLOW}Cancelling operation{settings.Colors.RESET}")
    __utils_runtime_logger("Cancelled operation", functionName)
    time.sleep(1)
    clear()

######################################################################
# __utils_operation_completed(): When a feature requires multiple steps
#                                use this
#
######################################################################
def show_current_step(str, currentStep, totalSteps):
    print(f"{str} Step:{currentStep}/{totalSteps}")
    print("--------------------------------------------------")

######################################################################
# has_one_non_space_char(): Helper function that checks if a string has
#                           at least one non space character
#
######################################################################    
def has_one_non_space_char(str):
    for char in str:
        if char != " ":
            return True
    return False


######################################################################
# get_settings_values_from_config(): Reads the settings.config file and
#                                    sets the global trigger values. 
#
# Note: This only reads the settings named SkipBulkLoaderInfoConfirmation
#       and SkipBulkLoaderPostEntryMenu in the settings.config file
######################################################################
def get_settings_values_from_config():
    with open(settingsConfigPath) as file:
        for line in file:
            key, value = map(str.strip, line.split("=", 1))
            if key == "SkipBulkLoaderInfoConfirmation":
                settings.GlobalSettings.SkipBulkLoaderInfoConfirmation = value
                return settings.GlobalSettings.SkipBulkLoaderInfoConfirmation
            elif key == "SkipBulkLoaderPostEntryMenu":
                settings.GlobalSettings.SkipBulkLoaderPostEntryMenu = value
                return settings.GlobalSettings.SkipBulkLoaderPostEntryMenu

######################################################################
# check_if_bulk_data_loader_settings_exist(): Checks if the settings.config file
#                                   contains the settings:
#                                   SkipBulkLoaderInfoConfirmation
#                                   SkipBulkLoaderPostEntryMenu
#
# See usage in add_settings_to_config()
######################################################################
def check_if_bulk_data_loader_settings_exist():
    __utils_runtime_logger("attempted to check if bulk data loader settings exist", "check_if_bulkd_data_loader_settings_exist")
    try:
     with open(settingsConfigPath, "r") as file:
        content = file.read()
        if "skipBulkLoaderInfoConfirmation" in content and "skipBulkLoaderPostEntryMenu" in content:
            return True
        else:
            return False
    except:
        print(f"File Not Found: {settingsConfigPath}")
        return -1


def check_for_special_chars(str):
    specialChars = ["!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "-", "+", "=", "{", "}", "[", "]", ":", ";", "'", "<", ">", ",", ".", "?", "/", "|", "\\", "`", "~"]
    for char in str:
        if char in specialChars:
            return True
    return False