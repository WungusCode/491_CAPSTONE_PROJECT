#ifndef _INDENT_PRINT_H

#define _INDENT_PRINT_H 1

#include <stdio.h>

// Control the indentation amount (e.g., 2 spaces per level).
#define INDENT_LEVEL 2

// A static variable to hold the current indentation depth.
extern int current_indent;

// The core printing macro.
// It uses a `do-while` loop to ensure proper handling with semicolons and `if`/`else` statements.
#define LOG_INDENTED(fmt, ...) do { \
    for (int i = 0; i < current_indent * INDENT_LEVEL; i++) { \
        putchar(' '); \
    } \
    printf(fmt, ##__VA_ARGS__); \
} while (0)

// Macros to manage the indentation state.
#define INDENT_INC() (current_indent++)
#define INDENT_DEC() (current_indent--)
#define INDENT_RESET() (current_indent = 0)

// Helper macros for block-structured logging.
#define LOG_BLOCK_START(fmt, ...) do { \
    LOG_INDENTED(fmt, ##__VA_ARGS__); \
    INDENT_INC(); \
} while (0)

#define LOG_BLOCK_END(fmt, ...) do { \
    INDENT_DEC(); \
    LOG_INDENTED(fmt, ##__VA_ARGS__); \
} while (0)

#endif
