#pragma once
#ifndef _CONFIGURATION_42_H_
#define _CONFIGURATION_42_H_


// Settings enabling/disabling certain things
#define __DEBUG__  0 // Debug Level of compiled program
                     // (0 indicates no debugging)
#define __COLORS__ 1 // If colored output is enabled
                     // (requires Terminal that is compatible with VT100 escape sequences)



// General game settings
#define MAX_GRID_COLUMNS 50 // Max amount of columns per game grid
#define MIN_GRID_COLUMNS 5  // Min amount of columns per game grid
#define STD_GRID_COLUMNS 5  // Standard amount of columns per game grid

#define MAX_GRID_ROWS    50 // Max amount of rows per game grid
#define MIN_GRID_ROWS    5  // Min amount of rows per game grid
#define STD_GRID_ROWS    5  // Standard amount of rows per game grid

#define LINE_COUNT 2  // Amount of extra lines added to amount of grid columns

// Buffers
#define COMMAND_BUF 32 // char* buffer for commands

#define COMMAND_DELIMS " " // Command delimiters

// Gamestates
#define GS_ONGOING 0 // Game ongoing
#define GS_LOST    1 // Game lost
#define GS_WON     2 // Game won

// Fieldstates
#define FS_NOTHING   0x00 // Empty field
#define FS_NOTHING_F 0x10 // Flagged empty field
#define FS_BOMB      0x01 // Field with bomb
#define FS_BOMB_F    0x11 // Flagged field with bomb
#define FS_CLEARED   0x02 // Field cleared by player

// Fieldcharacters
#define FC_NOTHING    "? "  // Field with nothing
#define FC_NOTHING_F  "! "  // Flagged field (maybe add colour)
#define FC_BOMB       "? "  // Field with bomb normally
#define FC_BOMB_F     "! "  // Field with bomb flagged
#define FC_BOMB_CLEAR "B "  // Bomb when grid is printed in clear mode
#define FC_CLEARED    "%d " // Cleared field (Number of adjacent mines)

// Commandlinearguments parser return values
#define PAR_SUCCESS 0 // Arguments are successfully parsed - don't return main (now)
#define PAR_HELP    1 // Help argument was passed - return main with success
#define PAR_ERROR   2 // Parser encountered an error while parsing - return main with error

// Changes made upon compiling as debug
#if __DEBUG__ == 1
    #undef FC_BOMB // Show bombs when compiled as Debug
    #define FC_BOMB "B "

    #undef FC_BOMB_F // Show flagged bombs when compiled as Debug
    #define FC_BOMB_F "F "
#endif // !__DEBUG__

// OS specific stuff
#if defined(__unix__)
#endif // !(__unix__)

#if defined(_WIN32) || defined(_WIN64)
#endif // !(_WIN32 || _WIN64)

// Event colors - these don't matter when colors are turned off
#include "cli/conscodes.h"
#define COL_LOSS    C_F_RED   // Loss color
#define COL_WIN     C_F_GREEN // Win color
#define COL_INDEX   C_F_BROWN // Grid indexnumber color
#define COL_HIDDEN  C_F_WHITE // Hidden field
#define COL_CLEARED C_F_GREEN // Opened field
#define COL_FLAGGED C_F_RED   // Flagged field
#define COL_RESET   C_RESET   // Reset colors

#endif // !_CONFIGURATION_42_H_
