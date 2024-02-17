#===============================================================================
#File Name   : utils.py
#-------------------------------------------------------------------------------
#Author      : Marshall Burns a.k.a. Schooly
#-------------------------------------------------------------------------------
#Description : This file contains several utility functions, variables, 
#              enums, and variables that are used throughout the 'Python'
#              portion of the source code.
#===============================================================================

import os
import time
import json
import sys
from enum import Enum

dataFilePath = "../build/data/active_data.json"
settingsConfigPath = "../../build/settings.config"
statsLogFilePath = "../../logs/stats.log"
errorLogFilePath = "../../logs/errors.log"
runtimeLogFilePath = "../../logs/runtime.log"

sys.path.append("../../../python/")
import settings

class ErrorLevel(Enum):
    MINOR = 0
    MODERATE = 1
    CRITICAL = 2


######################################################################
# clear(): clears the terminal screen
#
######################################################################
def clear():
    # this only works on unix based systems
    os.system("clear")

######################################################################
# check_file_for_valid_json(): Helper function to make sure the passed
#                              in file contains valid json
#
# see usage in 
######################################################################
def check_file_for_valid_json(filePath):
    # try to open the file and load the JSON data
    try:
        with open(dataFilePath, "r") as file:
            jsonData = json.load(file)
        return True
    # if there is an error, print the error and return False
    except json.JSONDecodeError as error:
        print(f"Error Decoding JSON in {dataFilePath}: {error}")
        return False
    # if the file is not found, print the error and return False
    except FileNotFoundError as error:
        print(f"File Not Found: {dataFilePath}")
        return False

######################################################################
# insert_json_a//rray: simply inserts a blank json array into the passed
#                    in file  
#
# see usage in 
######################################################################
def insert_json_array(filePath):
  try:
    with open(filePath, "w") as file:
       json.dump([], file)
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
    print(f"{settings.GlobalSettings.Colors.YELLOW}Cancelling operation{settings.GlobalSettings.Colors.RESET}")
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
# update_stats_file(): Updates the stats.log file to maintain a running
#                      total of students processed as well as total 
#                      sessions of the bulk data loader have been run
#                      
#
######################################################################
def update_stats_file():
    with open(statsLogFilePath, "w") as file:
        file.write("Current Bulk Data Loader Session: " + str(update_bulk_data_loader_session()) + "\n")
        #todo not quite done with this. technically this should be a running total not the current session total
        file.write("Total Students Processed: " + str(session_student_iota()) + "\n")

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
# check_if_settings_values_exist(): Checks if the settings.config file
#                                   contains the settings:
#                                   SkipBulkLoaderInfoConfirmation
#                                   SkipBulkLoaderPostEntryMenu
#
######################################################################
def check_if_settings_values_exist():
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
