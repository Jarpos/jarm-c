#pragma once
#ifndef _COMMAND_LINE_INTERFACE_42_H_
#define _COMMAND_LINE_INTERFACE_42_H_

#include <stdbool.h>

#include "../logic/GameLogic.h"
#include "../config.h"
#include "conscodes.h"

void cli_PrintBoard(Board_t* board, bool clear);
void cli_ResetCursor(Board_t* board);
char* cli_GetCommand();
char** cli_SplitCommand(char* command);
int8_t cli_ParseClArguments(int argc, char* argv[], uint16_t* row, uint16_t* col);
void cli_PrintEndScreen(Board_t* board);
void cli_IngameHelp(Board_t* board, char** commandvec);
void cli_PrintHelp();

#endif // !_COMMAND_LINE_INTERFACE_42_H_
