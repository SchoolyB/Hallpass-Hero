package main

import "core:fmt"
import "core:strings"
import "core:strconv"
import "../src/lib"
import "../src/rosters"
import "../src/auth"

//todo: move me :)
mainMenuOptionsArr:[]string={
    "1. Manage Rosters",
    "2. View and manage an existing roster",
    "3. Add students to the student database",
    "4. View and manage the student database",
    "5. Search for a student",
    "6. Help",
    "7. Settings",
    "8. Exit"
}


main ::proc(){
    //login auth happens first see auth.odin
    handle_main_menu_loop()


}

handle_main_menu_loop :: proc(){
    using fmt
    using strconv
    using lib

    G_MainMenuRunning = true

        for(G_MainMenuRunning) {
            println("Welcome to the Hall Pass Hero main menu!")
            println("Please enter the corresponding number of your desired operation:\n")

            for option in mainMenuOptionsArr{
                println(option)
            }

            _, inputAsInt,  inputError:= get_input_as_str_and_int()

            switch(inputAsInt){
            case mainMenuOptions[.MANAGE_ROSTERS]:
                rosters.show_roster_management_menu()
            case mainMenuOptions[.ADD_STUDENT_TO_DB]:
                break
            case mainMenuOptions[.VIEW_DB]:
                break
            case mainMenuOptions[.SEARCH]:
                break
            case mainMenuOptions[.GET_HELP]:
                break
            case mainMenuOptions[.MODIFY_SETTINGS]:
                break
            case mainMenuOptions[.EXIT_PROGRAM]:
                break
            case:
                fmt.println(INVALID_OPTION_CHOSEN)
                handle_main_menu_loop()
            }
        }
}