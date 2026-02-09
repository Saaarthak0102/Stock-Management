#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include "../include/supplier.h"
#include "../include/stock.h"
#include "../include/reports.h"
#include "../include/utils.h"

int get_next_supplier_id(void) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    if (file == NULL) return 1;
    Supplier supplier;
    int max_id = 0;
    while (fread(&supplier, sizeof(Supplier), 1, file)) {
        if (supplier.id > max_id) max_id = supplier.id;
    }
    fclose(file);
    return max_id + 1;
}

int supplier_id_exists(int id) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    if (file == NULL) return 0;
    Supplier supplier;
    while (fread(&supplier, sizeof(Supplier), 1, file)) {
        if (supplier.id == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int add_supplier(Supplier *supplier) {
    (void)supplier;
    Supplier new_supplier;
    printf("\nSupplier ID: ");
    new_supplier.id = get_next_supplier_id();
    printf("%d (Auto-assigned)\n", new_supplier.id);

    int valid = 0;
    do {
        fflush(stdin);
        printf("\nSupplier Name: ");
        fgets(new_supplier.name, sizeof(new_supplier.name), stdin);
        trim_string(new_supplier.name);
        if (strlen(new_supplier.name) > 0) valid = 1;
    } while (!valid);

    valid = 0;
    do {
        fflush(stdin);
        printf("\nContact Number: ");
        fgets(new_supplier.contact, sizeof(new_supplier.contact), stdin);
        trim_string(new_supplier.contact);
        if (strlen(new_supplier.contact) > 0) valid = 1;
    } while (!valid);

    valid = 0;
    do {
        fflush(stdin);
        printf("\nEmail: ");
        fgets(new_supplier.email, sizeof(new_supplier.email), stdin);
        trim_string(new_supplier.email);
        if (validate_email(new_supplier.email)) valid = 1;
    } while (!valid);

    printf("\nCity: ");
    fgets(new_supplier.city, sizeof(new_supplier.city), stdin);
    trim_string(new_supplier.city);

    new_supplier.created_at = time(NULL);

    if (confirm_action("\nAdd this supplier?")) {
        FILE *file = fopen(SUPPLIERS_FILE, "ab");
        if (file == NULL) return 0;
        if (fwrite(&new_supplier, sizeof(Supplier), 1, file)) {
            fclose(file);
            char log_msg[200];
            snprintf(log_msg, sizeof(log_msg), "Added supplier: %s (ID: %d)", new_supplier.name, new_supplier.id);
            log_activity("ADD_SUPPLIER", log_msg);
            return 1;
        }
        fclose(file);
    }
    return 0;
}

void read_suppliers(void) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    if (file == NULL) {
        printf("\nNo supplier records found.\n");
        return;
    }
    Supplier supplier;
    printf("\n+------------+----------------------+--------------+----------------------+----------+\n");
    printf("| %-10s | %-20s | %-12s | %-20s | %-8s |\n", "ID", "Name", "Contact", "Email", "City");
    printf("+------------+----------------------+--------------+----------------------+----------+\n");
    while (fread(&supplier, sizeof(Supplier), 1, file)) {
        printf("| %-10d | %-20s | %-12s | %-20s | %-8s |\n",
               supplier.id, supplier.name, supplier.contact, supplier.email, supplier.city);
    }
    printf("+------------+----------------------+--------------+----------------------+----------+\n");
    fclose(file);
}

Supplier* search_supplier_by_id(int id) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    if (file == NULL) return NULL;
    Supplier *supplier = (Supplier *)malloc(sizeof(Supplier));
    while (fread(supplier, sizeof(Supplier), 1, file)) {
        if (supplier->id == id) {
            fclose(file);
            return supplier;
        }
    }
    fclose(file);
    free(supplier);
    return NULL;
}

Supplier* search_supplier_by_name(const char *name) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    if (file == NULL) return NULL;
    Supplier *supplier = (Supplier *)malloc(sizeof(Supplier));
    char search_name[MAX_SUPPLIER_NAME];
    strncpy(search_name, name, sizeof(search_name) - 1);
    search_name[sizeof(search_name) - 1] = '\0';
    to_uppercase(search_name);
    while (fread(supplier, sizeof(Supplier), 1, file)) {
        char temp_name[MAX_SUPPLIER_NAME];
        strncpy(temp_name, supplier->name, sizeof(temp_name) - 1);
        temp_name[sizeof(temp_name) - 1] = '\0';
        to_uppercase(temp_name);
        if (strcmp(temp_name, search_name) == 0) {
            fclose(file);
            return supplier;
        }
    }
    fclose(file);
    free(supplier);
    return NULL;
}

int update_supplier(int id, Supplier *new_supplier) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    FILE *temp = fopen("data/temp_supplier.dat", "wb");
    if (file == NULL || temp == NULL) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }
    Supplier supplier;
    int found = 0;
    while (fread(&supplier, sizeof(Supplier), 1, file)) {
        if (supplier.id == id) {
            if (new_supplier != NULL) {
                supplier = *new_supplier;
                supplier.id = id;
            }
            found = 1;
        }
        fwrite(&supplier, sizeof(Supplier), 1, temp);
    }
    fclose(file);
    fclose(temp);
    if (found) {
        remove(SUPPLIERS_FILE);
        rename("data/temp_supplier.dat", SUPPLIERS_FILE);
        return 1;
    }
    remove("data/temp_supplier.dat");
    return 0;
}

int delete_supplier(int id) {
    FILE *file = fopen(SUPPLIERS_FILE, "rb");
    FILE *temp = fopen("data/temp_supplier.dat", "wb");
    if (file == NULL || temp == NULL) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }
    Supplier supplier;
    int found = 0;
    while (fread(&supplier, sizeof(Supplier), 1, file)) {
        if (supplier.id != id) {
            fwrite(&supplier, sizeof(Supplier), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);
    if (found) {
        remove(SUPPLIERS_FILE);
        rename("data/temp_supplier.dat", SUPPLIERS_FILE);
        return 1;
    }
    remove("data/temp_supplier.dat");
    return 0;
}

void display_supplier_stock(int supplier_id) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo stock records found.\n");
        return;
    }
    Stock stock;
    printf("\n+------------+----------------------+----------+----------+-------+\n");
    printf("| %-10s | %-20s | %-8s | %-8s | %-5s |\n", "ID", "Name", "Category", "Price", "Qty");
    printf("+------------+----------------------+----------+----------+-------+\n");
    while (fread(&stock, sizeof(Stock), 1, file)) {
        if (stock.supplier_id == supplier_id) {
            printf("| %-10d | %-20s | %-8s | %-8.2f | %-5d |\n",
                   stock.id, stock.name, stock.category, stock.price, stock.quantity);
        }
    }
    printf("+------------+----------------------+----------+----------+-------+\n");
    fclose(file);
}
