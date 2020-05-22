#pragma once
#ifndef _GAME_LOGIC_42_H_
#define _GAME_LOGIC_42_H_

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../config.h"

typedef struct Board_t
{
    uint16_t Cols;
    uint16_t Rows;

    // Gamestates: (See config.h)
    // 0000 0000 | Running
    // 0000 0001 | Game end - lost
    // 0000 0010 | Game end - won
    uint8_t GameState;

    // TODO: Use better method than pointer array
    // uint16_t Board[][];
    uint16_t* Grid[];
} Board_t;

Board_t* msw_Init(uint16_t cols, uint16_t rows);
void msw_InitMines(Board_t* board);
void msw_FlagCell(Board_t* board, char** commandvec);
void msw_MakeMove(Board_t* board, char** commandvec);
void msw_UpdateGameState(Board_t* board);
uint16_t msw_GetAdjacent(Board_t* board, uint16_t col, uint16_t row);
void msw_FreeBoard(Board_t* board);

#endif // !_GAME_LOGIC_42_H_

