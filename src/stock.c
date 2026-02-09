/*
 * Stock Management Module
 * Handles all stock-related operations including CRUD operations,
 * searching, sorting, and low stock alerts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include "../include/stock.h"
#include "../include/reports.h"
#include "../include/transaction.h"
#include "../include/utils.h"

/*
 * Get the next available stock ID
 * Scans all existing stock records to find the maximum ID
 * Returns: Next available ID (max_id + 1)
 */
int get_next_stock_id(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) return 1;
    Stock stock;
    int max_id = 0;
    while (fread(&stock, sizeof(Stock), 1, file)) {
        if (stock.id > max_id) max_id = stock.id;
    }
    fclose(file);
    return max_id + 1;
}

/*
 * Check if a stock ID already exists
 * Returns: 1 if exists, 0 if not found
 */
int stock_id_exists(int id) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) return 0;
    Stock stock;
    while (fread(&stock, sizeof(Stock), 1, file)) {
        if (stock.id == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/*
 * Add a new stock item to inventory
 * Validates all inputs and records transaction history
 * Returns: 1 on success, 0 on failure
 */
int add_stock(Stock *stock) {
    (void)stock;
    Stock new_stock;
    display_menu_header("ADD NEW PRODUCT");
    
    /* Auto-assign ID */
    printf("\nProduct ID: ");
    new_stock.id = get_next_stock_id();
    printf("%d (Auto-assigned)\n", new_stock.id);

    /* Validate product name - alphabetic characters only */
    int valid = 0;
    do {
        fflush(stdin);
        printf("\nProduct Name: ");
        if (fgets(new_stock.name, sizeof(new_stock.name), stdin) == NULL) {
            set_color(BRIGHT_RED);
            printf("Input error. Please try again.\n");
            reset_color();
            continue;
        }
        trim_string(new_stock.name);
        if (strlen(new_stock.name) == 0) {
            set_color(BRIGHT_RED);
            printf("Name cannot be empty.\n");
            reset_color();
            continue;
        }
        if (validate_alphabetic_string(new_stock.name)) {
            valid = 1;
        } else {
            set_color(BRIGHT_RED);
            printf("Invalid name. Use only alphabetic characters.\n");
            reset_color();
        }
    } while (!valid);

    /* Validate category */
    valid = 0;
    do {
        fflush(stdin);
        printf("\nCategory: ");
        if (fgets(new_stock.category, sizeof(new_stock.category), stdin) == NULL) {
            set_color(BRIGHT_RED);
            printf("Input error. Please try again.\n");
            reset_color();
            continue;
        }
        trim_string(new_stock.category);
        if (strlen(new_stock.category) == 0) {
            set_color(BRIGHT_RED);
            printf("Category cannot be empty.\n");
            reset_color();
            continue;
        }
        if (validate_alphabetic_string(new_stock.category)) {
            valid = 1;
        } else {
            set_color(BRIGHT_RED);
            printf("Invalid category. Use only alphabetic characters.\n");
            reset_color();
        }
    } while (!valid);

    /* Validate price with range check */
    valid = 0;
    do {
        printf("\nPrice (10-5000): ");
        if (scanf("%f", &new_stock.price) != 1) {
            fflush(stdin);
            set_color(BRIGHT_RED);
            printf("Invalid input. Please enter a number.\n");
            reset_color();
            continue;
        }
        if (new_stock.price >= 10.0f && new_stock.price <= 5000.0f) {
            valid = 1;
        } else {
            set_color(BRIGHT_RED);
            printf("Price must be between 10 and 5000.\n");
            reset_color();
        }
    } while (!valid);

    /* Validate quantity with range check */
    valid = 0;
    do {
        printf("\nQuantity (1-500): ");
        if (scanf("%d", &new_stock.quantity) != 1) {
            fflush(stdin);
            set_color(BRIGHT_RED);
            printf("Invalid input. Please enter a number.\n");
            reset_color();
            continue;
        }
        if (new_stock.quantity >= 1 && new_stock.quantity <= 500) {
            valid = 1;
        } else {
            set_color(BRIGHT_RED);
            printf("Quantity must be between 1 and 500.\n");
            reset_color();
        }
    } while (!valid);

    /* Optional supplier ID */
    printf("\nSupplier ID (optional, 0 for none): ");
    if (scanf("%d", &new_stock.supplier_id) != 1) {
        new_stock.supplier_id = 0;
    }

    /* Set timestamps */
    new_stock.created_at = time(NULL);
    new_stock.modified_at = time(NULL);

    /* Confirm and save */
    if (confirm_action("\nAdd this product?")) {
        FILE *file = fopen(STOCK_FILE, "ab");
        if (file == NULL) {
            set_color(BRIGHT_RED);
            printf("\nError: Unable to open stock file.\n");
            reset_color();
            return 0;
        }
        
        if (fwrite(&new_stock, sizeof(Stock), 1, file) == 1) {
            fclose(file);
            
            /* Record transaction for stock addition */
            char note[MAX_TRANSACTION_NOTE];
            snprintf(note, sizeof(note), "Initial stock added");
            record_transaction(new_stock.id, TRANS_PURCHASE, 
                             new_stock.quantity, new_stock.quantity, 
                             new_stock.price, note);
            
            /* Log activity */
            char log_msg[200];
            snprintf(log_msg, sizeof(log_msg), 
                    "Added product: %s (ID: %d, Qty: %d)", 
                    new_stock.name, new_stock.id, new_stock.quantity);
            log_activity("ADD_STOCK", log_msg);
            
            set_color(BRIGHT_GREEN);
            printf("\nProduct added successfully!\n");
            reset_color();
            return 1;
        }
        fclose(file);
    }
    
    return 0;
}

/*
 * Display all stock items in a formatted table
 * Shows ID, Name, Category, Price, Quantity, and Supplier ID
 */
void read_stock(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo inventory records found.\n");
        return;
    }
    Stock stock;
    printf("\n+------------+----------------------+----------+----------+-------+----------+\n");
    printf("| %-10s | %-20s | %-8s | %-8s | %-5s | %-8s |\n", "ID", "Name", "Category", "Price", "Qty", "Supplier");
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    while (fread(&stock, sizeof(Stock), 1, file)) {
        printf("| %-10d | %-20s | %-8s | %-8.2f | %-5d | %-8d |\n",
               stock.id, stock.name, stock.category, stock.price, stock.quantity, stock.supplier_id);
    }
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    fclose(file);
}

/*
 * Search for a stock item by ID
 * Returns: Pointer to Stock struct (caller must free), or NULL if not found
 */
Stock* search_stock_by_id(int id) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) return NULL;
    Stock *stock = (Stock *)malloc(sizeof(Stock));
    if (stock == NULL) {
        fclose(file);
        return NULL;
    }
    while (fread(stock, sizeof(Stock), 1, file)) {
        if (stock->id == id) {
            fclose(file);
            return stock;
        }
    }
    fclose(file);
    free(stock);
    return NULL;
}

/*
 * Search for a stock item by name (case-insensitive)
 * Returns: Pointer to Stock struct (caller must free), or NULL if not found
 */
Stock* search_stock_by_name(const char *name) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) return NULL;
    Stock *stock = (Stock *)malloc(sizeof(Stock));
    if (stock == NULL) {
        fclose(file);
        return NULL;
    }
    char search_name[MAX_PRODUCT_NAME];
    strncpy(search_name, name, sizeof(search_name) - 1);
    search_name[sizeof(search_name) - 1] = '\0';
    to_uppercase(search_name);
    while (fread(stock, sizeof(Stock), 1, file)) {
        char temp_name[MAX_PRODUCT_NAME];
        strncpy(temp_name, stock->name, sizeof(temp_name) - 1);
        temp_name[sizeof(temp_name) - 1] = '\0';
        to_uppercase(temp_name);
        if (strcmp(temp_name, search_name) == 0) {
            fclose(file);
            return stock;
        }
    }
    fclose(file);
    free(stock);
    return NULL;
}

int update_stock(int id, Stock *new_stock) {
    FILE *file = fopen(STOCK_FILE, "rb");
    FILE *temp = fopen("data/temp_stock.dat", "wb");
    if (file == NULL || temp == NULL) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }
    Stock stock;
    int found = 0;
    while (fread(&stock, sizeof(Stock), 1, file)) {
        if (stock.id == id) {
            if (new_stock != NULL) {
                stock = *new_stock;
                stock.id = id;
                stock.modified_at = time(NULL);
            }
            found = 1;
        }
        fwrite(&stock, sizeof(Stock), 1, temp);
    }
    fclose(file);
    fclose(temp);
    if (found) {
        remove(STOCK_FILE);
        rename("data/temp_stock.dat", STOCK_FILE);
        return 1;
    }
    remove("data/temp_stock.dat");
    return 0;
}

int delete_stock(int id) {
    FILE *file = fopen(STOCK_FILE, "rb");
    FILE *temp = fopen("data/temp_stock.dat", "wb");
    if (file == NULL || temp == NULL) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return 0;
    }
    Stock stock;
    int found = 0;
    while (fread(&stock, sizeof(Stock), 1, file)) {
        if (stock.id != id) {
            fwrite(&stock, sizeof(Stock), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(temp);
    if (found) {
        remove(STOCK_FILE);
        rename("data/temp_stock.dat", STOCK_FILE);
        return 1;
    }
    remove("data/temp_stock.dat");
    return 0;
}

void sort_stock_by_price(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo inventory records found.\n");
        return;
    }
    Stock stock;
    int count = 0;
    while (fread(&stock, sizeof(Stock), 1, file)) count++;
    if (count == 0) {
        fclose(file);
        printf("\nNo inventory records found.\n");
        return;
    }
    Stock *stocks = (Stock *)malloc(sizeof(Stock) * count);
    rewind(file);
    for (int i = 0; i < count; i++) fread(&stocks[i], sizeof(Stock), 1, file);
    fclose(file);
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (stocks[j].price > stocks[j + 1].price) {
                Stock temp = stocks[j];
                stocks[j] = stocks[j + 1];
                stocks[j + 1] = temp;
            }
        }
    }
    printf("\n+------------+----------------------+----------+----------+-------+----------+\n");
    printf("| %-10s | %-20s | %-8s | %-8s | %-5s | %-8s |\n", "ID", "Name", "Category", "Price", "Qty", "Supplier");
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %-10d | %-20s | %-8s | %-8.2f | %-5d | %-8d |\n",
               stocks[i].id, stocks[i].name, stocks[i].category, stocks[i].price, stocks[i].quantity, stocks[i].supplier_id);
    }
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    free(stocks);
}

void sort_stock_by_quantity(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo inventory records found.\n");
        return;
    }
    Stock stock;
    int count = 0;
    while (fread(&stock, sizeof(Stock), 1, file)) count++;
    if (count == 0) {
        fclose(file);
        printf("\nNo inventory records found.\n");
        return;
    }
    Stock *stocks = (Stock *)malloc(sizeof(Stock) * count);
    rewind(file);
    for (int i = 0; i < count; i++) fread(&stocks[i], sizeof(Stock), 1, file);
    fclose(file);
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (stocks[j].quantity > stocks[j + 1].quantity) {
                Stock temp = stocks[j];
                stocks[j] = stocks[j + 1];
                stocks[j + 1] = temp;
            }
        }
    }
    printf("\n+------------+----------------------+----------+----------+-------+----------+\n");
    printf("| %-10s | %-20s | %-8s | %-8s | %-5s | %-8s |\n", "ID", "Name", "Category", "Price", "Qty", "Supplier");
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    for (int i = 0; i < count; i++) {
        printf("| %-10d | %-20s | %-8s | %-8.2f | %-5d | %-8d |\n",
               stocks[i].id, stocks[i].name, stocks[i].category, stocks[i].price, stocks[i].quantity, stocks[i].supplier_id);
    }
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    free(stocks);
}

void display_low_stock_alerts(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo inventory records found.\n");
        return;
    }
    Stock stock;
    int count = 0;
    printf("\n+------------+----------------------+----------+----------+-------+----------+\n");
    printf("| %-10s | %-20s | %-8s | %-8s | %-5s | %-8s |\n", "ID", "Name", "Category", "Price", "Qty", "Supplier");
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    while (fread(&stock, sizeof(Stock), 1, file)) {
        if (stock.quantity <= LOW_STOCK_THRESHOLD) {
            printf("| %-10d | %-20s | %-8s | %-8.2f | %-5d | %-8d |\n",
                   stock.id, stock.name, stock.category, stock.price, stock.quantity, stock.supplier_id);
            count++;
        }
    }
    printf("+------------+----------------------+----------+----------+-------+----------+\n");
    fclose(file);
    if (count == 0) {
        printf("No low stock items.\n");
    }
}

void search_stock_interactive(void) {
    int choice;
    printf("\n1. Search by ID\n2. Search by Name\nEnter choice: ");
    scanf("%d", &choice);
    fflush(stdin);
    if (choice == 1) {
        int id;
        printf("Enter Product ID: ");
        scanf("%d", &id);
        Stock *stock = search_stock_by_id(id);
        if (stock) {
            printf("ID: %d\nName: %s\nCategory: %s\nPrice: %.2f\nQuantity: %d\nSupplier: %d\n",
                   stock->id, stock->name, stock->category, stock->price, stock->quantity, stock->supplier_id);
            free(stock);
        } else {
            printf("Product not found.\n");
        }
    } else if (choice == 2) {
        char name[MAX_PRODUCT_NAME];
        printf("Enter Product Name: ");
        fgets(name, sizeof(name), stdin);
        trim_string(name);
        Stock *stock = search_stock_by_name(name);
        if (stock) {
            printf("ID: %d\nName: %s\nCategory: %s\nPrice: %.2f\nQuantity: %d\nSupplier: %d\n",
                   stock->id, stock->name, stock->category, stock->price, stock->quantity, stock->supplier_id);
            free(stock);
        } else {
            printf("Product not found.\n");
        }
    } else {
        printf("Invalid choice.\n");
    }
}
