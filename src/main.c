#include <stddef.h>
#include <stdlib.h>

#include "config.h"
#include "cli/Cli.h"
#include "logic/GameLogic.h"

int main(int argc, char* argv[])
{
    /* Declare bare minimum of
     * variables needed for argv parsing */
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

    // Declare builtin commandchars & commandpointers
    char cmd_c[] =
        { 's', 'f', 'h' };
    void (*cmd_p[])(Board_t*, char**) =
        { &msw_MakeMove, &msw_FlagCell, &cli_IngameHelp, };

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
        for (size_t i = 0; i < sizeof(cmd_c) / sizeof(char); i++)
        {
            if (cmd_c[i] == commandvec[0][0])
            {
                cmd_p[i](board, commandvec);
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



