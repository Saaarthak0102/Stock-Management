#ifndef SALES_H
#define SALES_H

#include <time.h>

#define SALES_FILE "data/sales.dat"

typedef struct {
    int id;
    int bill_number;
    int product_id;
    int quantity;
    float price;
    float total_amount;
    time_t sale_date;
    char product_name[50];
} Sale;

int create_sale(void);
void read_sales(void);
void generate_bill(Sale *sale);
void display_sales_summary(void);
int get_next_bill_number(void);
float calculate_total_sales(void);
void display_most_sold_items(void);
int validate_sale(int product_id, int quantity);
int process_sale(Sale *sale);

#endif
