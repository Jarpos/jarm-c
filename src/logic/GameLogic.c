#include "GameLogic.h"

#include <stdlib.h>
#include <time.h>

Board_t* msw_Init(uint16_t cols, uint16_t rows)
{
    rows = rows < MAX_GRID_ROWS // Assign to given row amount or standard size
        && rows > MIN_GRID_ROWS
         ? rows : STD_GRID_ROWS;
    cols = cols < MAX_GRID_COLUMNS // Assign to given column amount or standard size
        && cols > MIN_GRID_COLUMNS
         ? cols : STD_GRID_COLUMNS;

    Board_t* board = (Board_t*)malloc(sizeof(Board_t) + (rows * sizeof(uint16_t*)));
    board->Cols = cols;
    board->Rows = rows;
    board->GameState = GS_ONGOING;

    for (int i = 0; i < board->Rows; i++)
        board->Grid[i] = (uint16_t*)malloc(cols * sizeof(uint16_t*));

    msw_InitMines(board);
    return board;
}

void msw_InitMines(Board_t* board)
{
    srand((unsigned int)time(NULL));
    for (uint16_t x = 0; x < board->Rows; x++)
        for (uint16_t y = 0; y < board->Cols; y++)
            board->Grid[x][y] = rand() % 2;
}

void msw_FlagCell(Board_t* board, char** commandvec)
{
    int32_t row = -1,
            col = -1;
    row = (int32_t)atol(commandvec[1]);
    col = (int32_t)atol(commandvec[2]);

    // Check if a valid position is entered & if the conversion worked
    if (board->Cols < col || col < 0
     || board->Rows < row || row < 0) return;
    switch (board->Grid[row][col])
    { // TODO: Rework flagging - use bitwise operations maybe
    case FS_NOTHING:   board->Grid[row][col] = FS_NOTHING_F; break;
    case FS_NOTHING_F: board->Grid[row][col] = FS_NOTHING;   break;
    case FS_BOMB:      board->Grid[row][col] = FS_BOMB_F;    break;
    case FS_BOMB_F:    board->Grid[row][col] = FS_BOMB;      break;
    case FS_CLEARED:   return;                               break;
    }
}

void msw_MakeMove(Board_t* board, char** commandvec)
{
    int32_t row = -1,
            col = -1;
    row = (int32_t)atol(commandvec[1]);
    col = (int32_t)atol(commandvec[2]);

    // Check if a valid Position is entered & if the conversion worked
    if (board->Cols < col || board->Rows < row ||
        col < 0           || row < 0) return;

    switch (board->Grid[row][col])
    {
    case FS_NOTHING: board->Grid[row][col] = FS_CLEARED; break;
    case FS_BOMB:    board->GameState      = GS_LOST;    break;
    case FS_CLEARED: return;                             break;
    }
}

void msw_UpdateGameState(Board_t* board)
{
    if (board->GameState != GS_ONGOING) return;

    for (uint16_t row = 0; row < board->Rows; row++)
    {
        for (uint16_t col = 0; col < board->Cols; col++)
        {
            switch (board->Grid[row][col])
            {
            case FS_BOMB:            break;
            case FS_NOTHING: return; break;
            case FS_CLEARED:         break;
            }
        }
    }

    // Win conditions are met if no empty field is left
    board->GameState = GS_WON;
}

static int test_adjacent(Board_t* board, uint16_t row, uint16_t col)
{
    if (!(col >= board->Cols || col < 0) &&
        !(row >= board->Rows || row < 0))
    {
        if (board->Grid[row][col] == FS_BOMB ||
            board->Grid[row][col] == FS_BOMB_F)
        {
            return 1;
        }
    }
    return 0;
}

uint16_t msw_GetAdjacent(Board_t* board, uint16_t row, uint16_t col)
{
    uint16_t cnt = 0;

    if (test_adjacent(board, row-1, col-1)) cnt++; // Cells above current cell
    if (test_adjacent(board, row+0, col-1)) cnt++;
    if (test_adjacent(board, row+1, col-1)) cnt++;

    if (test_adjacent(board, row-1, col+0)) cnt++; // Cells right/left to current cell
    if (test_adjacent(board, row+1, col+0)) cnt++;

    if (test_adjacent(board, row-1, col+1)) cnt++; // Cells below current cell
    if (test_adjacent(board, row+0, col+1)) cnt++;
    if (test_adjacent(board, row+1, col+1)) cnt++;

    return cnt;
}

void msw_FreeBoard(Board_t* board)
{
    for (uint16_t i = 0; i < board->Rows; i++)
    {
        free(board->Grid[i]);
        board->Grid[i] = NULL;
    }

    free(board);
    board = NULL;
}

