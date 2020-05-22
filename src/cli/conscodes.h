// Select escape sequence based on Current OS
#if defined(__unix__)
    #define _ESC_ "\033" // Console escape sequence
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define _ESC_ "\x1b" // Console escape sequence
#endif

// Cursor manipulation
#define CLEAR_LINE _ESC_ "[2K"
#define GO_UP(x)   printf(_ESC_ "[%dA", x)
#define GO_DO(x)   printf(_ESC_ "[%dB", x)

// Console foreground color codes
#define C_F_BLACK   _ESC_ "[30m"
#define C_F_RED     _ESC_ "[31m"
#define C_F_GREEN   _ESC_ "[32m"
#define C_F_BROWN   _ESC_ "[33m"
#define C_F_BLUE    _ESC_ "[34m"
#define C_F_MAGENTA _ESC_ "[35m"
#define C_F_CYAN    _ESC_ "[36m"
#define C_F_WHITE   _ESC_ "[37m"

// Console background color codes
#define C_B_BLACK   _ESC_ "[40m"
#define C_B_RED     _ESC_ "[41m"
#define C_B_GREEN   _ESC_ "[42m"
#define C_B_BROWN   _ESC_ "[43m"
#define C_B_BLUE    _ESC_ "[44m"
#define C_B_MAGENTA _ESC_ "[45m"
#define C_B_CYAN    _ESC_ "[46m"
#define C_B_WHITE   _ESC_ "[47m"

#define C_RESET _ESC_ "[0m"
