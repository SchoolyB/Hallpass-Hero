import json

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

# since the python file isnt affiliated with CMake, we need to use a relative path
filePath = "../../build/data/active_data.json"

if check_file_for_valid_json(filePath):
    print("File is valid JSON")
else:
    print("File is not valid JSON")
