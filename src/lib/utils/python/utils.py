import os
import time
import json
from enum import Enum

filePath = "../build/data/active_data.json"

# The current session number of the bulk data loader
globalSessionNumber = None
# The total number of students ever uploaded to the system
globalTotalStudentNumber = None

class ErrorLevel(Enum):
    MINOR = 0
    MODERATE = 1
    CRITICAL = 2

class Colors:
    RED = "\033[91m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    RESET = "\033[0m"


def clear():
    # this only works on unix based systems
    os.system("clear")


# increments the session number by one
def update_bulk_data_loader_session():
    bulkDataLoaderSessionNumber = globalSessionNumber 
    bulkDataLoaderSessionNumber += 1
    return bulkDataLoaderSessionNumber

# increments the total students processed number by one
def update_total_student_number():
    bulkDataLoaderTotalStudentNumber = globalTotalStudentNumber
    bulkDataLoaderTotalStudentNumber += 1
    return bulkDataLoaderTotalStudentNumber

# increments the student number of the current session by one
def session_student_iota():
    bulkDataLoaderSessionStudentNumber = 0
    bulkDataLoaderSessionStudentNumber += 1
    return bulkDataLoaderSessionStudentNumber


# todo this func will read from the stats.log file and return the student number count key value
# def check_total_student_number():

#todo this func will read from the stats.log file and return the current session number key value 
# def check_total_session_number():



######################################################################
# check_file_for_valid_json(): Helper function to make sure the passed
#                              in file contains valid json
#
# see usage in 
######################################################################
def check_file_for_valid_json(filePath):
    # try to open the file and load the JSON data
    try:
        with open(filePath, "r") as file:
            jsonData = json.load(file)
        return True
    # if there is an error, print the error and return False
    except json.JSONDecodeError as error:
        print(f"Error Decoding JSON in {filePath}: {error}")
        return False
    # if the file is not found, print the error and return False
    except FileNotFoundError as error:
        print(f"File Not Found: {filePath}")
        return False



######################################################################
# insert_json_array: simply inserts a blank json array into the passed
#                    in file  
#
# see usage in 
######################################################################
def insert_json_array(filePath):
  try:
    with open(filePath, "w") as file:
       json.dump([], file)
  except FileNotFoundError as error:
    print(f"File Not Found: {filePath}")
    return False


               

def __utils_error_logger(errorMessage, function, ErrorLevel):
    try:
        with open("../../../logs/errors.log", "a") as file:    
            match ErrorLevel:
                case ErrorLevel.MINOR: 
                   file.write(f"Logged @ {time.ctime()}\n")
                   file.write(f"Minor Error: {errorMessage} in function call {function}()\n")
                   file.write("======================================================================================\n")
                   file.close()
                   return ErrorLevel.MINOR
                case ErrorLevel.MODERATE:
                    file.write(f"Logged @ {time.ctime()}\n")
                    file.write(f"Moderate Error: {errorMessage} in function call {function}()\n")
                    file.write("======================================================================================\n")
                    file.close()
                    return ErrorLevel.MODERATE
                case ErrorLevel.CRITICAL:
                    file.write(f"Logged @ {time.ctime()}\n")
                    file.write(f"Critical Error: {errorMessage} in function call {function}()\n")
                    file.write("======================================================================================\n")
                    file.close()
                    print(f"{Colors.RED}Critical Error occurred @ {time.ctime()}: For more information see logs/errors.log\n")
                    exit() 
    except:
        print("Error: Could not write to error log file") 

def __utils_runtime_logger(action, function):
    try:
        with open("../../../logs/runtime.log", "a") as file:
            file.write(f"Logged @ {time.ctime()}\n")
            file.write(f"User Action: User {action} in function call {function}()\n")
            file.write("======================================================================================\n")
            file.close()
    except:
        print("Error: Could not write to runtime log file")

def __utils_operation_cancelled(functionName):
    clear()
    print(f"{Colors.YELLOW}Cancelling operation{Colors.RESET}")
    __utils_runtime_logger("Cancelled operation", functionName)
    time.sleep(1)
    clear()


def show_current_step(str, currentStep, totalSteps):
    print(f"{str} Step:{currentStep}/{totalSteps}")
    print("--------------------------------------------------")


def has_one_non_space_char(str):
    for char in str:
        if char != " ":
            return True
    return False

def update_stats_file():
    with open(filePath, "w") as file:
        file.write("Current Bulk Data Loader Session: " + str(update_bulk_data_loader_session()) + "\n")
        #todo not quite done with this. technically this should be a running total not the current session total
        file.write("Total Students Processed: " + str(session_student_iota()) + "\n")
