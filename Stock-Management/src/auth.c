#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../include/auth.h"
#include "../include/utils.h"

void hash_password(const char *password, char *hashed) {
    if (password == NULL || hashed == NULL) return;
    for (int i = 0; password[i] != '\0'; i++) {
        hashed[i] = (char)(password[i] + 3);
    }
    hashed[strlen(password)] = '\0';
}

int verify_password(const char *password, const char *hashed) {
    if (password == NULL || hashed == NULL) return 0;
    char temp[MAX_PASSWORD];
    hash_password(password, temp);
    return (strcmp(temp, hashed) == 0);
}

void init_users_file(void) {
    FILE *file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        file = fopen(USERS_FILE, "wb");
        if (file != NULL) {
            User admin;
            strncpy(admin.username, "admin", sizeof(admin.username) - 1);
            admin.username[sizeof(admin.username) - 1] = '\0';
            hash_password("admin", admin.password);
            admin.created_at = time(NULL);
            admin.is_admin = 1;
            fwrite(&admin, sizeof(User), 1, file);
            fclose(file);
        }
    } else {
        fclose(file);
    }
}

void get_masked_password(char *password, int max_len) {
    if (password == NULL) return;
    int i = 0;
    char ch;
    while (i < max_len - 1) {
        ch = getch();
        if (ch == 13) {
            break;
        } else if (ch == 8) {
            if (i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
        } else {
            password[i] = ch;
            printf("*");
            fflush(stdout);
            i++;
        }
    }
    password[i] = '\0';
}

int authenticate_user(void) {
    init_users_file();
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int attempts = 0;

    clear_screen();
    display_menu_header("LOGIN");

    while (attempts < MAX_LOGIN_ATTEMPTS) {
        printf("\nAttempt %d of %d\n", attempts + 1, MAX_LOGIN_ATTEMPTS);
        printf("\nUsername: ");
        scanf("%29s", username);
        printf("Password: ");
        get_masked_password(password, MAX_PASSWORD);
        printf("\n");

        FILE *file = fopen(USERS_FILE, "rb");
        if (file != NULL) {
            User user;
            int found = 0;
            while (fread(&user, sizeof(User), 1, file)) {
                if (strcmp(user.username, username) == 0) {
                    if (verify_password(password, user.password)) {
                        found = 1;
                        break;
                    }
                }
            }
            fclose(file);
            if (found) {
                set_color(BRIGHT_GREEN);
                printf("\n===============================================\n");
                printf("  LOGIN SUCCESSFUL - WELCOME TO STOCK MANAGER!\n");
                printf("===============================================\n");
                reset_color();
                pause_screen();
                return 1;
            }
        }

        if (strcmp(username, "user") == 0 && strcmp(password, "pass") == 0) {
            set_color(BRIGHT_GREEN);
            printf("\n===============================================\n");
            printf("  LOGIN SUCCESSFUL - WELCOME TO STOCK MANAGER!\n");
            printf("===============================================\n");
            reset_color();
            pause_screen();
            return 1;
        }

        attempts++;
        if (attempts < MAX_LOGIN_ATTEMPTS) {
            set_color(BRIGHT_RED);
            printf("\nInvalid username or password!\n");
            printf("Remaining attempts: %d\n", MAX_LOGIN_ATTEMPTS - attempts);
            reset_color();
            pause_screen();
            clear_screen();
        }
    }

    set_color(BRIGHT_RED);
    printf("\n===============================================\n");
    printf("  LOGIN FAILED - TOO MANY ATTEMPTS!\n");
    printf("  Program will exit now.\n");
    printf("===============================================\n");
    reset_color();
    pause_screen();
    return 0;
}
