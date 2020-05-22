#include "Cli.h"

static void internal_cli_PrintCell(Board_t* board, uint16_t row, uint16_t col, bool clear)
{
    for (size_t tmp = col; tmp > 10; tmp /= 10)
        printf(" ");

    switch (board->Grid[row][col])
    {
    case FS_BOMB:      printf(clear == true ? COL_HIDDEN FC_BOMB_CLEAR
                                            : COL_HIDDEN FC_BOMB);   break;
    case FS_BOMB_F:    printf(clear == true ? COL_HIDDEN FC_BOMB_CLEAR
                                            : COL_HIDDEN FC_BOMB_F); break;
    case FS_NOTHING:   printf(COL_HIDDEN FC_NOTHING);                break;
    case FS_NOTHING_F: printf(COL_HIDDEN FC_NOTHING_F);              break;
    case FS_CLEARED:   printf(COL_CLEARED FC_CLEARED,
                              msw_GetAdjacent(board, col, row));     break;
    }
}

static int internal_cli_GetNumbLength(uint16_t numb)
{
    /* Basically returns the exponent of this notation + 1:
     * 3.2 * 10^4 which is equal to 32000
     * In the example the exponent is 4 but numb is 5 long so the +1 is needed
     * We don't need to worry about negative numbers since its uint*_t
     *
     * We just need to worry about 0 since log10(0) would return -HUGE_VAL
     * (log10(1) returns 0 and with the plus one it's right again)
     */
    return numb == 0 ? 1 : log10(numb) + 1; // <-- Need to link to math.h (libm) for this
}

void cli_PrintBoard(Board_t* board, bool clear)
{
    int maxRowLen = internal_cli_GetNumbLength(board->Rows);

    for (uint16_t tmp = 0; tmp < maxRowLen + 1; tmp++)
        printf(" ");
    for (uint16_t col = 0; col < board->Cols; col++)
        printf(COL_INDEX "%hu " COL_RESET, col);

    for (uint16_t row = 0; row < board->Rows; row++)
    {
        // TODO: Fix row ouput
        printf("\n");
        for (int32_t tmp = maxRowLen - internal_cli_GetNumbLength(row); tmp > 0; tmp--)
            printf(" ");
        printf(COL_INDEX "%hu " COL_RESET, row);

        for (uint16_t col = 0; col < board->Cols; col++)
            internal_cli_PrintCell(board, row, col, clear);
    }

    printf(COL_RESET "\n" CLEAR_LINE);
}

void cli_ResetCursor(Board_t* board)
{
    GO_UP(board->Rows + LINE_COUNT);
}

char* cli_GetCommand()
{
    int c = 0;

    /* Clear buffer of newlines & push first
     * non newline character back onto stdin */
    while ((c = getchar()) == '\n');
    if (ungetc(c, stdin) == EOF) return NULL;

    size_t buffSize = COMMAND_BUF;
    char* buffer = malloc(buffSize);

    for (size_t i = 0;; i++)
    {
        c = getchar();
        if (/*c == EOF || c == '\n' ||*/ c == ';')
        {
            buffer[i] = '\0';
            buffer = realloc(buffer, i + 1);
            return buffer;
        }
        buffer[i] = c == '\0' ? ' ' : c;

        if (i + 1 >= buffSize)
        {
            buffSize += COMMAND_BUF;
            buffer = realloc(buffer, buffSize);

            if (!buffer) return NULL;
        }
    }
}

char** cli_SplitCommand(char* command)
{
    size_t buffSize = COMMAND_BUF,
           position = 0;
    char** tokens = malloc(sizeof(char*) * buffSize);
    char*  curtok = NULL;

    if (!tokens) return NULL;

    curtok = strtok(command, COMMAND_DELIMS);
    while (curtok != NULL)
    {
        tokens[position++] = curtok;

        if (position >= buffSize)
        {
            buffSize += COMMAND_BUF;
            tokens = realloc(tokens, sizeof(char*) * buffSize);
            if (!tokens) return NULL;
        }

        curtok = strtok(NULL, COMMAND_DELIMS);
    }
    tokens[position] = NULL;
    tokens = realloc(tokens, sizeof(char*) * (position + 1));
    return tokens;
}

int8_t cli_ParseClArguments(int argc, char* argv[], uint16_t* row, uint16_t* col)
{
    /* Test if commandline args are 
     * correct/contain help option */
    if (argc == 1)
    {
        cli_PrintHelp();
        return PAR_ERROR;
    }
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
            cli_PrintHelp();
            return PAR_HELP;
        }
    }

    // Assign options
    if (sscanf(argv[1], "%hu", row) == 0
     || sscanf(argv[2], "%hu", col) == 0)
    {   // TODO: Use value of "errno" for more exception handling
        printf("Error in parsing arguments (r, c):\n"
               "r=\"%s\", c=\"%s\" could not be interpreted as unsigned number.\n"
               "See \"--help\" for usage information\n", argv[1], argv[2]);
        return PAR_ERROR;
    }

    return PAR_SUCCESS;
}

void cli_PrintEndScreen(Board_t* board)
{
    switch (board->GameState)
    {
    case GS_LOST: printf(COL_LOSS "You have lost!\n" COL_RESET); break;
    case GS_WON:  printf(COL_WIN  "You have won!\n"  COL_RESET); break;
    }
    printf(CLEAR_LINE);
}

void cli_IngameHelp(Board_t* board, char** commandvec)
{
    // Keep cursor aligned with map
    getchar();
    GO_UP(1);

    printf("s y x; (Select cell)  /  "
           "f y x; (Flag cell)  /  "
           "h (Help);  (Enter to continue)");
    while (getchar() != '\n');
}

void cli_PrintHelp()
{
    printf("Usage: (./)[EXECUTABLE] r c [OPTIONS]\n"
           "Play badly made minesweeper on the console\n"
           "\n"
           "Options:\n"
           "       r      \tSpecify amount of rows (number)\n"
           "       c      \tSpecify amount of columns (number)\n"
           "       --help \tDisplay this help and exit\n"
           "\n"
           "Note:\n"
           "Not Inputing these options or inputing values\n"
           "that are not within the range specified while\n"
           "compilation (r=%d-%d, c=%d-%d) will result in a fallback\n"
           "onto the compilation standard values of r=%d and c=%d\n"
           "\n"
           "Example:\n"
           "./jarm 10 10\n",
           MIN_GRID_ROWS, MAX_GRID_ROWS, MIN_GRID_COLUMNS, MAX_GRID_COLUMNS,
           STD_GRID_ROWS, STD_GRID_COLUMNS);
}
