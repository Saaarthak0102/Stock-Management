#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../include/menu.h"
#include "../include/stock.h"
#include "../include/supplier.h"
#include "../include/sales.h"
#include "../include/reports.h"
#include "../include/utils.h"

static void print_banner_file(void) {
    FILE *file = fopen("assets/banner.txt", "r");
    if (file == NULL) {
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void display_banner(void) {
    clear_screen();
    set_color(BRIGHT_CYAN);
    print_banner_file();
    reset_color();
    printf("\nPress any key to continue...");
    getch();
}

void display_main_menu(void) {
    int choice = 0;
    while (choice != 6) {
        clear_screen();
        display_menu_header("MAIN MENU");
        printf("\n1. Stock Management\n");
        printf("2. Supplier Management\n");
        printf("3. Sales and Billing\n");
        printf("4. Reports and Analytics\n");
        printf("5. Admin Settings\n");
        printf("6. Logout and Exit\n");
        printf("\nEnter your choice [1-6]: ");
        scanf("%d", &choice);
        fflush(stdin);
        clear_screen();
        switch (choice) {
            case 1:
                display_stock_menu();
                break;
            case 2:
                display_supplier_menu();
                break;
            case 3:
                display_sales_menu();
                break;
            case 4:
                display_reports_menu();
                break;
            case 5:
                display_admin_menu();
                break;
            case 6:
                set_color(BRIGHT_GREEN);
                printf("\nGoodbye!\n");
                reset_color();
                exit(0);
            default:
                set_color(BRIGHT_RED);
                printf("Invalid choice!\n");
                reset_color();
                pause_screen();
        }
    }
}

void display_stock_menu(void) {
    int choice = 0;
    while (choice != 10) {
        clear_screen();
        display_menu_header("STOCK MANAGEMENT");
        printf("\n1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("6. Update Stock Quantity\n");
        printf("7. Sort by Price\n");
        printf("8. Sort by Quantity\n");
        printf("9. View Low Stock Alerts\n");
        printf("10. Return to Main Menu\n");
        printf("\nEnter your choice [1-10]: ");
        scanf("%d", &choice);
        fflush(stdin);
        clear_screen();
        switch (choice) {
            case 1:
                display_menu_header("ADD NEW PRODUCT");
                add_stock(NULL);
                pause_screen();
                break;
            case 2:
                display_menu_header("PRODUCT INVENTORY");
                read_stock();
                pause_screen();
                break;
            case 3:
                display_menu_header("SEARCH PRODUCT");
                search_stock_interactive();
                pause_screen();
                break;
            case 4: {
                int id;
                display_menu_header("UPDATE PRODUCT");
                printf("Enter Product ID to update: ");
                scanf("%d", &id);
                Stock *stock = search_stock_by_id(id);
                if (stock == NULL) {
                    set_color(BRIGHT_RED);
                    printf("Product not found.\n");
                    reset_color();
                } else {
                    printf("Current Name: %s\n", stock->name);
                    printf("New Name: ");
                    fgets(stock->name, sizeof(stock->name), stdin);
                    trim_string(stock->name);
                    printf("New Category: ");
                    fgets(stock->category, sizeof(stock->category), stdin);
                    trim_string(stock->category);
                    printf("New Price: ");
                    scanf("%f", &stock->price);
                    printf("New Quantity: ");
                    scanf("%d", &stock->quantity);
                    printf("New Supplier ID: ");
                    scanf("%d", &stock->supplier_id);
                    if (update_stock(id, stock)) {
                        set_color(BRIGHT_GREEN);
                        printf("Product updated.\n");
                        reset_color();
                    }
                    free(stock);
                }
                pause_screen();
                break;
            }
            case 5: {
                int id;
                display_menu_header("DELETE PRODUCT");
                printf("Enter Product ID to delete: ");
                scanf("%d", &id);
                if (confirm_action("Are you sure you want to delete this product?")) {
                    if (delete_stock(id)) {
                        set_color(BRIGHT_GREEN);
                        printf("Product deleted.\n");
                        reset_color();
                    } else {
                        set_color(BRIGHT_RED);
                        printf("Product not found.\n");
                        reset_color();
                    }
                }
                pause_screen();
                break;
            }
            case 6: {
                int id, qty;
                display_menu_header("UPDATE STOCK QUANTITY");
                printf("Enter Product ID: ");
                scanf("%d", &id);
                Stock *stock = search_stock_by_id(id);
                if (stock == NULL) {
                    set_color(BRIGHT_RED);
                    printf("Product not found.\n");
                    reset_color();
                } else {
                    printf("Current Qty: %d\n", stock->quantity);
                    printf("Enter adjustment (+/-): ");
                    scanf("%d", &qty);
                    stock->quantity += qty;
                    if (stock->quantity < 0) stock->quantity = 0;
                    if (update_stock(id, stock)) {
                        set_color(BRIGHT_GREEN);
                        printf("Stock updated.\n");
                        reset_color();
                    }
                    free(stock);
                }
                pause_screen();
                break;
            }
            case 7:
                display_menu_header("SORT BY PRICE");
                sort_stock_by_price();
                pause_screen();
                break;
            case 8:
                display_menu_header("SORT BY QUANTITY");
                sort_stock_by_quantity();
                pause_screen();
                break;
            case 9:
                display_menu_header("LOW STOCK ALERTS");
                display_low_stock_alerts();
                pause_screen();
                break;
            case 10:
                return;
            default:
                set_color(BRIGHT_RED);
                printf("Invalid choice!\n");
                reset_color();
                pause_screen();
        }
    }
}

void display_supplier_menu(void) {
    int choice = 0;
    while (choice != 7) {
        clear_screen();
        display_menu_header("SUPPLIER MANAGEMENT");
        printf("\n1. Add New Supplier\n");
        printf("2. View All Suppliers\n");
        printf("3. Search Supplier\n");
        printf("4. Update Supplier\n");
        printf("5. Delete Supplier\n");
        printf("6. View Supplier Stock\n");
        printf("7. Return to Main Menu\n");
        printf("\nEnter your choice [1-7]: ");
        scanf("%d", &choice);
        fflush(stdin);
        clear_screen();
        switch (choice) {
            case 1:
                display_menu_header("ADD NEW SUPPLIER");
                add_supplier(NULL);
                pause_screen();
                break;
            case 2:
                display_menu_header("SUPPLIER LIST");
                read_suppliers();
                pause_screen();
                break;
            case 3: {
                int id;
                display_menu_header("SEARCH SUPPLIER");
                printf("Enter Supplier ID: ");
                scanf("%d", &id);
                Supplier *supplier = search_supplier_by_id(id);
                if (supplier == NULL) {
                    set_color(BRIGHT_RED);
                    printf("Supplier not found.\n");
                    reset_color();
                } else {
                    set_color(BRIGHT_GREEN);
                    printf("ID: %d\nName: %s\nContact: %s\nEmail: %s\nCity: %s\n",
                           supplier->id, supplier->name, supplier->contact, supplier->email, supplier->city);
                    reset_color();
                    free(supplier);
                }
                pause_screen();
                break;
            }
            case 4: {
                int id;
                display_menu_header("UPDATE SUPPLIER");
                printf("Enter Supplier ID to update: ");
                scanf("%d", &id);
                Supplier *supplier = search_supplier_by_id(id);
                if (supplier == NULL) {
                    set_color(BRIGHT_RED);
                    printf("Supplier not found.\n");
                    reset_color();
                } else {
                    printf("New Name: ");
                    fgets(supplier->name, sizeof(supplier->name), stdin);
                    trim_string(supplier->name);
                    printf("New Contact: ");
                    fgets(supplier->contact, sizeof(supplier->contact), stdin);
                    trim_string(supplier->contact);
                    printf("New Email: ");
                    fgets(supplier->email, sizeof(supplier->email), stdin);
                    trim_string(supplier->email);
                    printf("New City: ");
                    fgets(supplier->city, sizeof(supplier->city), stdin);
                    trim_string(supplier->city);
                    if (update_supplier(id, supplier)) {
                        set_color(BRIGHT_GREEN);
                        printf("Supplier updated.\n");
                        reset_color();
                    }
                    free(supplier);
                }
                pause_screen();
                break;
            }
            case 5: {
                int id;
                display_menu_header("DELETE SUPPLIER");
                printf("Enter Supplier ID to delete: ");
                scanf("%d", &id);
                if (confirm_action("Are you sure you want to delete this supplier?")) {
                    if (delete_supplier(id)) {
                        set_color(BRIGHT_GREEN);
                        printf("Supplier deleted.\n");
                        reset_color();
                    } else {
                        set_color(BRIGHT_RED);
                        printf("Supplier not found.\n");
                        reset_color();
                    }
                }
                pause_screen();
                break;
            }
            case 6: {
                int id;
                display_menu_header("SUPPLIER STOCK");
                printf("Enter Supplier ID: ");
                scanf("%d", &id);
                display_supplier_stock(id);
                pause_screen();
                break;
            }
            case 7:
                return;
            default:
                set_color(BRIGHT_RED);
                printf("Invalid choice!\n");
                reset_color();
                pause_screen();
        }
    }
}

void display_sales_menu(void) {
    int choice = 0;
    while (choice != 4) {
        clear_screen();
        display_menu_header("SALES AND BILLING");
        printf("\n1. Create New Sale\n");
        printf("2. View Sales History\n");
        printf("3. View Most Sold Items\n");
        printf("4. Return to Main Menu\n");
        printf("\nEnter your choice [1-4]: ");
        scanf("%d", &choice);
        fflush(stdin);
        clear_screen();
        switch (choice) {
            case 1:
                display_menu_header("CREATE NEW SALE");
                create_sale();
                pause_screen();
                break;
            case 2:
                display_menu_header("SALES HISTORY");
                read_sales();
                pause_screen();
                break;
            case 3:
                display_menu_header("MOST SOLD ITEMS");
                display_most_sold_items();
                pause_screen();
                break;
            case 4:
                return;
            default:
                set_color(BRIGHT_RED);
                printf("Invalid choice!\n");
                reset_color();
                pause_screen();
        }
    }
}

void display_reports_menu(void) {
    int choice = 0;
    while (choice != 8) {
        clear_screen();
        display_menu_header("REPORTS AND ANALYTICS");
        printf("\n1. Daily Sales Report\n");
        printf("2. Monthly Sales Report\n");
        printf("3. Revenue Report\n");
        printf("4. Stock Valuation\n");
        printf("5. Low Stock Report\n");
        printf("6. Activity Logs\n");
        printf("7. Export to CSV\n");
        printf("8. Return to Main Menu\n");
        printf("\nEnter your choice [1-8]: ");
        scanf("%d", &choice);
        fflush(stdin);
        clear_screen();
        switch (choice) {
            case 1:
                display_menu_header("DAILY SALES REPORT");
                generate_daily_report();
                pause_screen();
                break;
            case 2:
                display_menu_header("MONTHLY SALES REPORT");
                generate_monthly_report();
                pause_screen();
                break;
            case 3:
                display_menu_header("REVENUE REPORT");
                generate_revenue_report();
                pause_screen();
                break;
            case 4:
                display_menu_header("STOCK VALUATION");
                display_stock_valuation();
                pause_screen();
                break;
            case 5:
                display_menu_header("LOW STOCK REPORT");
                display_low_stock_report();
                pause_screen();
                break;
            case 6:
                display_menu_header("ACTIVITY LOGS");
                display_activity_logs();
                pause_screen();
                break;
            case 7:
                display_menu_header("EXPORT TO CSV");
                if (export_to_csv()) {
                    set_color(BRIGHT_GREEN);
                    printf("Export successful.\n");
                    reset_color();
                } else {
                    set_color(BRIGHT_RED);
                    printf("Export failed.\n");
                    reset_color();
                }
                pause_screen();
                break;
            case 8:
                return;
            default:
                set_color(BRIGHT_RED);
                printf("Invalid choice!\n");
                reset_color();
                pause_screen();
        }
    }
}

void display_admin_menu(void) {
    int choice = 0;
    while (choice != 4) {
        clear_screen();
        display_menu_header("ADMIN SETTINGS");
        printf("\n1. Backup Data\n");
        printf("2. Restore Data\n");
        printf("3. System Information\n");
        printf("4. Return to Main Menu\n");
        printf("\nEnter your choice [1-4]: ");
        scanf("%d", &choice);
        fflush(stdin);
        clear_screen();
        switch (choice) {
            case 1:
                display_menu_header("BACKUP DATA");
                if (backup_data()) {
                    set_color(BRIGHT_GREEN);
                    printf("Backup created.\n");
                    reset_color();
                } else {
                    set_color(BRIGHT_RED);
                    printf("Backup failed.\n");
                    reset_color();
                }
                pause_screen();
                break;
            case 2:
                display_menu_header("RESTORE DATA");
                if (confirm_action("Restore from latest backup?")) {
                    if (restore_data()) {
                        set_color(BRIGHT_GREEN);
                        printf("Restore completed.\n");
                        reset_color();
                    } else {
                        set_color(BRIGHT_RED);
                        printf("Restore failed.\n");
                        reset_color();
                    }
                }
                pause_screen();
                break;
            case 3:
                display_menu_header("SYSTEM INFO");
                printf("Stock Management System v2.0\n");
                printf("Binary file-based storage\n");
                pause_screen();
                break;
            case 4:
                return;
            default:
                set_color(BRIGHT_RED);
                printf("Invalid choice!\n");
                reset_color();
                pause_screen();
        }
    }
}
