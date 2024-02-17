#===============================================================================
#File Name   : settings.py
#-------------------------------------------------------------------------------
#Author      : Marshall Burns a.k.a. Schooly
#-------------------------------------------------------------------------------
#Description : This file contains all functions that are used to modify the
#              settings.config file
#              
# Note: These only modify the settings that are used in the Python portion of the
#       source code. The settings that are used in the C portion of the source code
#       are modified in the _settings.c.              
#===============================================================================
import time
import sys
sys.path.append("../lib/utils/python")
import utils


class Colors:
    RED = "\033[91m"
    GREEN = "\033[92m"
    YELLOW = "\033[93m"
    RESET = "\033[0m"

class GlobalSettings:
    RuntimeLoggingEnabled = None
    AutoStudentIDGenerationEnabled = None
    ColorsEnabled = None
    # A global flag that when enabled will skip the confirmation of the student information
    SkipBulkLoaderInfoConfirmation = None
    # A global flag that when enabled will skip the menu that appears after a student is entered
    # this will allow the user to enter as many students as they want without having to go through the menu
    SkipBulkLoaderPostEntryMenu = None #will hold a value of 1 or 0, 1 for true and 0 for false



######################################################################
# get_and_set_settings(): Retrieves the key value pairs from the settings.config
#                 file and sets the global trigger values accordingly
# 
# See usage in main()
######################################################################
def get_and_set_settings():
    opt1 = get_setting("runtimeLoggingEnabled", "get_and_set_settings", GlobalSettings.RuntimeLoggingEnabled, "RuntimeLoggingEnabled")
    opt2 = get_setting("autoStudentIDGenerationEnabled", "get_and_set_settings", GlobalSettings.AutoStudentIDGenerationEnabled, "AutoStudentIDGenerationEnabled")
    opt3 = get_setting("colorEnabled", "get_and_set_settings", GlobalSettings.ColorsEnabled, "ColorsEnabled")
    opt4 = get_setting("skipBulkLoaderInfoConfirmation", "get_and_set_settings", GlobalSettings.SkipBulkLoaderInfoConfirmation, "SkipBulkLoaderInfoConfirmation")
    opt5 = get_setting("skipBulkLoaderPostEntryMenu", "get_and_set_settings", GlobalSettings.SkipBulkLoaderPostEntryMenu, "SkipBulkLoaderPostEntryMenu")

    GlobalSettings.RuntimeLoggingEnabled = opt1
    GlobalSettings.AutoStudentIDGenerationEnabled = opt2
    GlobalSettings.ColorsEnabled = opt3
    GlobalSettings.SkipBulkLoaderInfoConfirmation = opt4
    GlobalSettings.SkipBulkLoaderPostEntryMenu = opt5
    
    # Convert the string values to integers
    GlobalSettings.RuntimeLoggingEnabled = int(GlobalSettings.RuntimeLoggingEnabled)
    GlobalSettings.AutoStudentIDGenerationEnabled = int(GlobalSettings.AutoStudentIDGenerationEnabled)
    GlobalSettings.ColorsEnabled = int(GlobalSettings.ColorsEnabled)
    GlobalSettings.SkipBulkLoaderInfoConfirmation = int(GlobalSettings.SkipBulkLoaderInfoConfirmation)
    GlobalSettings.SkipBulkLoaderPostEntryMenu = int(GlobalSettings.SkipBulkLoaderPostEntryMenu)
    
    if(GlobalSettings.ColorsEnabled == 0):
        Colors.RED = ""
        Colors.GREEN = ""
        Colors.YELLOW = ""
        Colors.RESET = ""
    elif(GlobalSettings.ColorsEnabled == 1):
        Colors.RED = "\033[91m"
        Colors.GREEN = "\033[92m"
        Colors.YELLOW = "\033[93m"
        Colors.RESET = "\033[0m"
    utils.__utils_runtime_logger("Initiated settings retrieval", "get_and_set_settings")


######################################################################
# add_settings_to_config(): Adds the settings key value pairs
#                                      to the settings.config file on
#                                      startup
# See usage in startup.py
######################################################################
def add_settings_to_config():
    utils.__utils_runtime_logger("attempted to add bulk loader specific settings to settings.config ", "add_settings_to_config")
    try:
        with open(utils.settingsConfigPath, "a") as file:
            #Check if the key value pairs already exist in the settings.config file
            valuesExist = utils.check_if_settings_values_exist() 
            if valuesExist == True:
                file.close()
            elif(valuesExist == False):
                #If they dont exist then add them to the settings.config file
                file.write("skipBulkLoaderInfoConfirmation=0\n")
                file.write("skipBulkLoaderPostEntryMenu=0\n")
                file.close()
    except FileNotFoundError as error:
        print(f"File Not Found: {utils.settingsConfigPath}")
        return False

######################################################################
# get_setting(): Retrieves the value of a setting from the settings.config
#                file
# See usage in get_and_set_settings()
######################################################################
def get_setting(settingName, functionName, defaultSettingValue, logMessage):
    try:
        with open(utils.settingsConfigPath) as file:
            for line in file:
                if "=" in line:
                    key, value = map(str.strip, line.split("=", 1))
                    if key == settingName:
                        return value.strip()  # Return the setting value if found
    except FileNotFoundError as error:
        utils.__utils_error_logger(f"File Not Found: {error}", f"{functionName}")
    except Exception as error:
        utils.__utils_error_logger(f"Error reading settings file: {error}", f"{functionName}")

    # Return the default value if setting is not found or there is an error
    return defaultSettingValue

# Initialize the settings on startup
def main():
    add_settings_to_config()
    get_and_set_settings()

if __name__ == "__main__":
    main()