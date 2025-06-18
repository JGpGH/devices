#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// ANSI color codes for Linux/Unix
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

#define MAX_PROGRESS_LINE 256

typedef struct {
    char last_line[MAX_PROGRESS_LINE];
} ConsoleState;

// Print with color
void print_color(const char* color, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("%s", color);
    vprintf(fmt, args);
    printf(COLOR_RESET);
    va_end(args);
    printf("\n");
    fflush(stdout);
}

// Print and update progress line at the bottom, always keeping it as the last line
void print_update(ConsoleState* state, const char* fmt, ...) {
    // Erase the current progress line
    printf("\r\033[K");
    va_list args;
    va_start(args, fmt);
    vsnprintf(state->last_line, MAX_PROGRESS_LINE, fmt, args);
    va_end(args);
    printf("%s", state->last_line);
    printf(COLOR_RESET);
    fflush(stdout);
}

// Print a message above the progress line, then restore the progress line
void print_above(ConsoleState* state, const char* color, const char* fmt, ...) {
    // Move cursor to start of line, then print a newline to push progress down
    printf("\r\033[K");
    printf("\n");
    // Move cursor up to the new blank line
    printf("\033[1A");
    va_list args;
    va_start(args, fmt);
    printf("%s", color);
    vprintf(fmt, args);
    printf(COLOR_RESET);
    va_end(args);
    printf("\n");
    // Restore the progress line
    printf("%s", state->last_line);
    fflush(stdout);
}

// Restore the last progress line (e.g., after printing something else)
void restore_progress_line(ConsoleState* state) {
    printf("\r\033[K%s", state->last_line);
    fflush(stdout);
}

// Example usage:
// print_color(COLOR_GREEN, "Success!\n");
// print_update("Progress: %d%%", percent);
