cmake_minimum_required(VERSION 3.1...3.10)

project(jarm VERSION 0.0
        DESCRIPTION "Just another random Minesweeper")


if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
                          # First line activates more Warnings
        set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic \
                           -fsanitize=address -g3 -D_FORTIFY_SOURCE=2 \
                           -D_GLIBCXX_ASSERTIONS -fexceptions -fstack-protector \
                           -fstack-protector-all -g -pipe -D_REENTRANT")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(psd ${PROJECT_SOURCE_DIR})
set(cd ${psd})


# Generate cli library
set(cd ${psd}/src/cli)
set(cli ${cd}/Cli.c)
add_library(cli "${cli}")

# Generate logic library
set(cd ${psd}/src/logic)
set(log ${cd}/GameLogic.c)
add_library(log "${log}")

# Generate executable with linkage to libraries
set(libs cli
         log)

IF (NOT WIN32)
    set(libs ${libs} m) # "m" is needed to link to libm (math.h)
                        # but windows does linking to math automatically
                        # so we don't need math.h here
ENDIF()

add_executable(prog.out src/main.c)
target_link_libraries(prog.out PUBLIC ${libs})
