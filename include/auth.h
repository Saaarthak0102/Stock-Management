#ifndef AUTH_H
#define AUTH_H

#include <time.h>

#define MAX_USERNAME 30
#define MAX_PASSWORD 30
#define MAX_LOGIN_ATTEMPTS 3
#define USERS_FILE "data/users.dat"

typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    time_t created_at;
    int is_admin;
} User;

int authenticate_user(void);
void init_users_file(void);
void hash_password(const char *password, char *hashed);
int verify_password(const char *password, const char *hashed);
void get_masked_password(char *password, int max_len);

#endif
