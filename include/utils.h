#ifndef UTILS_H
#define UTILS_H

#define DARK_BLUE       1
#define DARK_GREEN      2
#define DARK_CYAN       3
#define DARK_RED        4
#define DARK_MAGENTA    5
#define DARK_YELLOW     6
#define DARK_WHITE      7
#define GRAY            8
#define BRIGHT_BLUE     9
#define BRIGHT_GREEN    10
#define BRIGHT_CYAN     11
#define BRIGHT_RED      12
#define BRIGHT_MAGENTA  13
#define BRIGHT_YELLOW   14
#define BRIGHT_WHITE    15

void clear_screen(void);
void pause_screen(void);
void set_color(int color);
void reset_color(void);
void print_colored(int color, const char *format, ...);
int validate_integer(const char *input);
int validate_float(const char *input);
int validate_alphabetic_string(const char *str);
int validate_email(const char *email);
int file_exists(const char *filename);
void get_current_date(char *buffer, int size);
void get_current_time(char *buffer, int size);
int confirm_action(const char *message);
void print_table_header(const char **headers, int *widths, int count);
void print_table_row(const char **values, int *widths, int count);
void trim_string(char *str);
void to_uppercase(char *str);
void to_lowercase(char *str);
void display_menu_header(const char *title);

#endif
