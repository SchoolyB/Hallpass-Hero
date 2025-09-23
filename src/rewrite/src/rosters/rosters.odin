package rosters
import "../lib"
import "core:fmt"


newRosterMenu :[]string={
    "1. View an existing roster",
    "2. Create a new roster",
    "3. Rename an existing roster",
    "4. Delete an existing roster",
    "5. Modify an existing roster",
    "6. Help",
    "7. Go back to main menu"
}


show_roster_management_menu :: proc() -> lib.Error{
    using lib

    G_ManageRosterMenuRunning = true

    for(G_ManageRosterMenuRunning){
        for option in newRosterMenu{
            fmt.println(option)
        }

        _ , inputAsInt,  inputError:= get_input_as_str_and_int()

        switch(inputAsInt){
            case manageRosterMenuOptions[.VIEW_ROSTER]:
                break
            case manageRosterMenuOptions[.CREATE_ROSTER]:
                create_new_roster()
                break
            case manageRosterMenuOptions[.RENAME_ROSTER]:
                break
            case manageRosterMenuOptions[.DELETE_ROSTER]:
                break
            case manageRosterMenuOptions[.MODIFY_ROSTER]:
                break
            case manageRosterMenuOptions[.GO_BACK]:
                G_ManageRosterMenuRunning =  false
                return make_error()
            case:
                fmt.println(INVALID_OPTION_CHOSEN)
                break
        }
    }

    return make_error()
}


create_new_roster :: proc()-> lib.Error{
    using lib
    using fmt

    println("Please enter the name of the new roster")
    println("Roster names can be no less than 2 AND no more than 32 characters")
    println("To cancel this operation enter 'cancel' ")

    rosterNameInput, inputError:= get_input()
    if (len(rosterNameInput) < 2){
        println("The entered roster name must be atleast 2 characters")
        return make_error()
    }else if (len(rosterNameInput) > 32){
        println("The entered roster name but be less than 32 characters")
        return make_error()
    }else{
        //trigger ostrichdb interactivity??
    }

    if rosterNameInput == "cancel"{
        println("Canceling operation...")
    }

    return make_error()
}



// check_if_roster_exists ::proc(roster:string) -> (bool, lib.Error){



// }

