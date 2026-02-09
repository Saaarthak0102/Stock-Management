#include <stdio.h>
#include <stdlib.h>
#include "../include/auth.h"
#include "../include/menu.h"
#include "../include/utils.h"

int main(void) {
    display_banner();
    init_users_file();
    if (!authenticate_user()) {
        exit(0);
    }
    display_main_menu();
    return 0;
}
