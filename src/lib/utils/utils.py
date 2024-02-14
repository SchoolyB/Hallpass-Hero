import json

filePath = "../build/data/active_data.json"


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

