package lib
import "core:os"
import "core:fmt"
import "core:strings"
import "core:strconv"

//The size arg is the size in bytes you need the input buffer to be. If nothing is passed default is 128
@(require_results)
get_input_as_str_and_int :: proc() -> (string, int, Error) {
	buf := new([32]byte)
	defer free(buf)

	n, err := os.read(os.stdin, buf[:])
	if err != 0 {
		return "", 0, make_error("Failed to read input from command line", .CRITICAL)
	}
	resultAsStr := strings.trim_right(string(buf[:n]), "\r\n")
	resultAsInt:= strconv.atoi(resultAsStr)

	return strings.clone(resultAsStr), resultAsInt, make_error()
}
