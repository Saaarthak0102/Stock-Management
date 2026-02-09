#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include <stdarg.h>
#include <conio.h>
#include "../include/utils.h"

void clear_screen(void) {
    system("cls");
}

void pause_screen(void) {
    printf("\nPress any key to continue...");
    getch();
}

void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)color);
}

void reset_color(void) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, BRIGHT_WHITE);
}

void print_colored(int color, const char *format, ...) {
    va_list args;
    va_start(args, format);
    set_color(color);
    vprintf(format, args);
    reset_color();
    va_end(args);
}

int validate_integer(const char *input) {
    if (input == NULL || input[0] == '\0') {
        return 0;
    }
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit((unsigned char)input[i]) && !(i == 0 && input[i] == '-')) {
            return 0;
        }
    }
    return 1;
}

int validate_float(const char *input) {
    if (input == NULL || input[0] == '\0') {
        return 0;
    }
    int dot_count = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (!isdigit((unsigned char)input[i]) && !(i == 0 && input[i] == '-')) {
            return 0;
        }
    }
    return 1;
}

int validate_alphabetic_string(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]) && str[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int validate_email(const char *email) {
    if (email == NULL || email[0] == '\0') {
        return 0;
    }
    int at_count = 0;
    int dot_count = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_count++;
        } else if (email[i] == '.') {
            dot_count++;
        }
    }
    return (at_count == 1 && dot_count >= 1);
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

void get_current_date(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d", tm_info);
}

void get_current_time(char *buffer, int size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "%H:%M:%S", tm_info);
}

int confirm_action(const char *message) {
    char choice;
    printf("%s [y/n]: ", message);
    scanf(" %c", &choice);
    fflush(stdin);
    return (choice == 'y' || choice == 'Y');
}

void print_table_header(const char **headers, int *widths, int count) {
    set_color(BRIGHT_BLUE);
    printf("+");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n|");
    for (int i = 0; i < count; i++) {
        printf(" %-*s |", widths[i], headers[i]);
    }
    printf("\n+");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < widths[i] + 2; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");
    reset_color();
}

void print_table_row(const char **values, int *widths, int count) {
    printf("|");
    for (int i = 0; i < count; i++) {
        printf(" %-*s |", widths[i], values[i]);
    }
    printf("\n");
}

void trim_string(char *str) {
    if (str == NULL) return;
    int start = 0;
    int end = (int)strlen(str) - 1;
    while (start <= end && isspace((unsigned char)str[start])) start++;
    while (end >= start && isspace((unsigned char)str[end])) end--;
    if (start > 0) {
        memmove(str, str + start, end - start + 2);
    }
    str[end - start + 1] = '\0';
}

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void display_menu_header(const char *title) {
    if (title == NULL) return;
    set_color(BRIGHT_BLUE);
    printf("\n===============================================\n");
    printf("  %s\n", title);
    printf("===============================================\n");
    reset_color();
}
