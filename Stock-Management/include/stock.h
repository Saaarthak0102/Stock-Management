#ifndef STOCK_H
#define STOCK_H

#include <time.h>

#define MAX_PRODUCT_NAME 50
#define MAX_CATEGORY 30
#define STOCK_FILE "data/stock.dat"
#define LOW_STOCK_THRESHOLD 10

typedef struct {
    int id;
    char name[MAX_PRODUCT_NAME];
    char category[MAX_CATEGORY];
    int quantity;
    float price;
    int supplier_id;
    time_t created_at;
    time_t modified_at;
} Stock;

int add_stock(Stock *stock);
void read_stock(void);
Stock* search_stock_by_id(int id);
Stock* search_stock_by_name(const char *name);
int update_stock(int id, Stock *new_stock);
int delete_stock(int id);
void sort_stock_by_price(void);
void sort_stock_by_quantity(void);
void display_low_stock_alerts(void);
void search_stock_interactive(void);
int get_next_stock_id(void);
int stock_id_exists(int id);
void log_activity(const char *action, const char *details);

#endif
