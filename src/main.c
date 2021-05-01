#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "cli/Cli.h"
#include "logic/GameLogic.h"

typedef struct command_t
{
    char name;
    void (*func)(Board_t*, char**);
} command_t;

int main(int argc, char* argv[])
{
#if __DEBUG__ >= 1
    printf("This is a debug build. As such it may display where the bombs are.\n"
           "Disable this by setting __DEBUG__ in src/config.h equal to 0\n\n");
#endif

    /* Declare bare minimum of argv
     * variables needed for parsing */
    uint16_t row = 0,
             col = 0;

    // Parse command line args and exit on error/"--help" option
    switch (cli_ParseClArguments(argc, argv, &row, &col))
    {
    case PAR_SUCCESS:           break; // Ignore since programm must continue
    case PAR_HELP:    return 0; break; // Return success since help was printed as requested
    case PAR_ERROR:   return 1; break; // Return without success since parser ran into error
    }

    /* Declare/Initialise the rest
     * of the needed variables */
    char* command = NULL;
    char** commandvec = NULL;
    Board_t* board = msw_Init(row, col);

    // Declare builtin commands
    command_t cmds[] =
    {
        { 's', &msw_MakeMove },
        { 'f', &msw_FlagCell },
        { 'h', &cli_IngameHelp },
    };

    do
    {   // Main gameplay loop
        cli_PrintBoard(board, false);

        // Prevent memory leaks
        free(command);
        free(commandvec);
        command = NULL;
        commandvec = NULL;

        // Get commands, parse 'em and execute 'em if possible
        command = cli_GetCommand();
        commandvec = cli_SplitCommand(command);
        if (!commandvec) continue;

        // Find command in builtin commands and execute it
        for (size_t i = 0; i < sizeof(cmds) / sizeof(command_t); i++)
        {
            if (cmds[i].name == commandvec[0][0])
            {
                cmds[i].func(board, commandvec);
                break;
            }
        }

        // Test if game is already won
        msw_UpdateGameState(board);
        cli_ResetCursor(board);
    }
    while(board->GameState == GS_ONGOING);

    /* Print board with bombs visible
     * and give feedback on gameresult */
    cli_PrintBoard(board, true);
    cli_PrintEndScreen(board);

    // Free used memory
    free(command);
    free(commandvec);
    msw_FreeBoard(board);
    command = NULL;
    commandvec = NULL;
    board = NULL;

    return 0;
}



