package auth

import "core:fmt"
import "../lib"
import "core:c/libc"
import "core:encoding/json"
import "core:encoding/base64"
import "core:strings"
import "core:crypto/hash"
import "core:crypto/hmac"

@(private)
result: [32]u8

HPH_JWT_SIGNATURE :: "MyApp2025$SuperSecret#Key!With@Numbers123&Symbols*AndLetters456"

UserCredentialType :: enum {
    FIRST_NAME = 0,
    LAST_NAME,
    EMAIL,
    USERNAME,
    PASSWORD
}

UserCredentials :: struct {
    firstName, lastName, email, userName, password :string
}

@(init)
run_login ::proc(){
    userCreds:= make_user_credentials()
    get_user_credential(.FIRST_NAME, userCreds)
    get_user_credential(.LAST_NAME, userCreds)
    get_user_credential(.EMAIL, userCreds)

    fmt.println("DEBUG: userCreds after getting credential: ", userCreds)

    //generate a URL encoded and signed
    jwt, jwtError:= generate_users_jwt(userCreds)
    if jwtError.type != .NONE do fmt.println("ERROR LOGGING IN")


    fmt.println("DEBUG: encodeJWT: ",jwt)
    // send_jwt_payload(jwt, "http://localhost:8042/version")

}

send_jwt_payload ::proc(jwt:string, route:string){
    b:strings.Builder =strings.builder_make()

    shipmentStr:= fmt.tprintf("curl -H Authorization: Bearer %s %s",jwt, route)
    shipmentCStr:= strings.to_cstring(&b)
    libc.system(shipmentCStr)
}


make_user_credentials :: proc() -> ^UserCredentials{
    userCreds:= new(UserCredentials)
    userCreds.firstName = ""
    userCreds.lastName = ""
    userCreds.email = ""
    userCreds.userName = ""
    userCreds.password = ""

    return userCreds
}

//Gets the users input for the passed in 'UserCredentialType'
get_user_credential :: proc(credType: UserCredentialType, userCredsStruct: ^UserCredentials) -> lib.Error{
    using lib
    using fmt

    credName:string= ---
    input:string
    inputError:lib.Error

    switch(credType){
        case .FIRST_NAME:
            credName = "First Name"
            break
        case .LAST_NAME:
            credName = "Last Name"
            break
        case .EMAIL:
            credName = "Email"
            break
        case .USERNAME:
            credName = "Username"
            break
        case .PASSWORD:
            credName = "Password"
            break
    }


    printfln("Please enter your %s :", credName)

    if credType == .PASSWORD{
        //TODO: hide the echo if password here
        input, inputError = get_input()
        if inputError.type == .CRITICAL {
            //TODO: show echo  here
            return inputError
        }
            //TODO: show echo  here
    } else{
        input, inputError = get_input()
        if inputError.type == .CRITICAL {
            return inputError
        }
    }

    if (len(input) == 0 ){
        printfln("%s cannot be blank. Please try again.", credName)
        get_user_credential(credType, userCredsStruct)
    }

    switch(credType){
        case .FIRST_NAME:
            userCredsStruct.firstName = input
            break
        case .LAST_NAME:
            userCredsStruct.lastName = input
            break
        case .EMAIL:
            userCredsStruct.email = input
            break
        case .USERNAME:
            userCredsStruct.userName = input
            break
        case .PASSWORD:
            userCredsStruct.password = input
            break
    }

    return make_error()
}

header_json_structure :: struct {
    alg: string ,
    typ: string ,
}

payload_json_structure :: struct {
    sub: string ,
    iss: string ,
    azp: string ,
    exp: string ,
    iat: string ,
    nbf: string ,
    jti: string ,
    email: string ,
    first_name: string ,
    last_name: string ,
}

//Generates and returns a JSON string for auth
generate_users_jwt :: proc(userCredsStruct: ^UserCredentials) -> (string, lib.Error){

    headerJSON:= new(payload_json_structure)
    payloadJSON:= new(payload_json_structure)

    payload:=map[string]string{
        "sub"= "hall_pass_hero_user",
        "iss" = "hallpasshero",
        "azp" = "http://localhost:3000",
        "exp" = "9999999999",
        "iat" = "1640908800",
        "nbf" = "1640908800",
        "jti" = "jwt_123456789abcdef",


        "email" = userCredsStruct.email,
        "first_name" = userCredsStruct.firstName,
        "last_name" = userCredsStruct.lastName
    }

    headers:= map[string]string{
        "alg"= "HS256",
        "typ"= "JWT"
    }

    headerJSONStr, marshalErrorTwo:= json.marshal(headers)
    if marshalErrorTwo != nil do return "", lib.make_error("Error marshaling JWT hearders", .CRITICAL)

    payloadJSONStr, marshalErrorOne:= json.marshal(payload)
    if marshalErrorOne != nil do return "", lib.make_error("Error marshaling JWT payload", .CRITICAL)


    encodedHeader := encode_url_base64(string(headerJSONStr))
    encodedPayload := encode_url_base64(string(payloadJSONStr))

    unsignedToken := fmt.tprintf("%s.%s", encodedHeader, encodedPayload)
    signatureAsBytes := sign_jwt_with_hs256(unsignedToken, HPH_JWT_SIGNATURE)
    encodedSignature := encode_url_base64(string(signatureAsBytes))

    jwt := fmt.tprintf("%s.%s.%s", encodedHeader, encodedPayload, encodedSignature)

    return jwt, lib.make_error()
}


//URL encode the passed in JSON string
encode_url_base64 :: proc(data:string) -> string {

    encoded:= base64.encode(transmute([]u8)data)
    result, replaceError := strings.replace_all(encoded, "+", "-")
    result, replaceError = strings.replace_all(result, "/", "_")
    result = strings.trim_right(result, "=")

    return result
}

//Signs the passed token(data) with passed in signature
sign_jwt_with_hs256 :: proc(data: string, signature: string) -> []u8 {
    hmac.sum(hash.Algorithm.SHA256,result[:], transmute([]u8)data, transmute([]u8)signature)

    return result[:]
}