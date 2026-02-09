#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <time.h>

#define MAX_SUPPLIER_NAME 50
#define MAX_CONTACT 20
#define MAX_EMAIL 50
#define SUPPLIERS_FILE "data/suppliers.dat"

typedef struct {
    int id;
    char name[MAX_SUPPLIER_NAME];
    char contact[MAX_CONTACT];
    char email[MAX_EMAIL];
    char city[30];
    time_t created_at;
} Supplier;

int add_supplier(Supplier *supplier);
void read_suppliers(void);
Supplier* search_supplier_by_id(int id);
Supplier* search_supplier_by_name(const char *name);
int update_supplier(int id, Supplier *new_supplier);
int delete_supplier(int id);
void display_supplier_stock(int supplier_id);
int get_next_supplier_id(void);
int supplier_id_exists(int id);
void log_activity(const char *action, const char *details);

#endif
