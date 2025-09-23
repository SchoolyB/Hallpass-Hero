package lib



T_MainMenuOptions :: enum {
    MANAGE_ROSTERS,
    ADD_STUDENT_TO_DB,
    VIEW_DB,
    SEARCH,
    GET_HELP,
    MODIFY_SETTINGS,
    EXIT_PROGRAM,
}

mainMenuOptions: map[T_MainMenuOptions]int={
    .MANAGE_ROSTERS = 1,
    .ADD_STUDENT_TO_DB = 2,
    .VIEW_DB = 3,
    .SEARCH = 4,
    .GET_HELP = 5,
    .MODIFY_SETTINGS = 6,
    .EXIT_PROGRAM = 7,
}

T_ManageRosterMenuOptions :: enum {
    VIEW_ROSTER,
    CREATE_ROSTER ,
    RENAME_ROSTER,
    DELETE_ROSTER,
    MODIFY_ROSTER, //maybe dont need???
    HELP,
    GO_BACK,
}

manageRosterMenuOptions:= map[T_ManageRosterMenuOptions]int{
    .VIEW_ROSTER = 1,
    .CREATE_ROSTER  = 2,
    .RENAME_ROSTER = 3,
    .DELETE_ROSTER = 4,
    .MODIFY_ROSTER = 5,
    .HELP = 6,
    .GO_BACK = 7,
}


