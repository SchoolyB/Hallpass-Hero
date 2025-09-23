package lib

import "base:runtime"
import "core:text/regex"
import "core:strings"
import "core:fmt"
import "core:os"


DISALLOWED_CHARS_PATTERN :: `[^a-zA-Z0-9]`
RESET :: "\033[0m"
BOLD :: "\033[1m"
UNDERLINE :: "\033[4m"
RED :: "\033[31m"
GREEN :: "\033[32m"
YELLOW :: "\033[33m"

Error :: struct {
    type: ErrorType,
    message: string,
    location: runtime.Source_Code_Location
}

ErrorType :: enum {
   NONE = 0,
   UNKOWN,
   WARNING,
   ERROR,
   CRITICAL
}

@(require_results)
get_caller_location :: proc(location:= #caller_location) -> runtime.Source_Code_Location{
    return location
}

make_error ::proc(msg:string= "No Error!", type:ErrorType=.NONE, loc:= #caller_location) -> Error{
    return Error{ message = msg, type = type, location = loc}
}

@(require_results)
read_file :: proc(filePath: string, callingProcedure: runtime.Source_Code_Location) -> ([]byte, bool) {
    data, err:=os.read_entire_file(filePath)
	return data, err
}

@(require_results)
write_to_file :: proc(filepath: string, data: []byte, callingProcedure: runtime.Source_Code_Location) -> bool {
    return os.write_entire_file(filepath, data)
}

@(require_results)
append_single_qoutations_rune :: proc(value: rune) -> string {
	return strings.clone(fmt.tprintf("'%c'", value))
}

@(require_results)
trim_qoutations :: proc(value: string) -> string {
	if strings.contains(value, "\"") {
		return strings.clone(strings.trim(value, "\""))
	}
	return strings.clone(value)
}

@(require_results)
contains_disallowed_chars :: proc(input: string) -> bool {
    pattern, regexCompileError := regex.create(DISALLOWED_CHARS_PATTERN)
    if regexCompileError != nil {
        return false
    }
    defer regex.destroy(pattern)

    _, matchFound := regex.match(pattern, input)
    if matchFound {
        return true
    }

    return false
}

//The size arg is the size in bytes you need the input buffer to be. If nothing is passed default is 128
get_input :: proc() -> (string, Error) {
	buf := new([128]byte)
	defer free(buf)

	n, err := os.read(os.stdin, buf[:])
	if err != 0 {
		return "", make_error("Failed to read input from command line", .CRITICAL)
	}
	result := strings.trim_right(string(buf[:n]), "\r\n")
	return strings.clone(result), make_error()
}