#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "../include/sales.h"
#include "../include/stock.h"
#include "../include/reports.h"
#include "../include/transaction.h"
#include "../include/utils.h"

int get_next_bill_number(void) {
    FILE *file = fopen(SALES_FILE, "rb");
    if (file == NULL) return 1001;
    Sale sale;
    int max_bill = 1000;
    while (fread(&sale, sizeof(Sale), 1, file)) {
        if (sale.bill_number > max_bill) max_bill = sale.bill_number;
    }
    fclose(file);
    return max_bill + 1;
}

int validate_sale(int product_id, int quantity) {
    Stock *stock = search_stock_by_id(product_id);
    if (stock == NULL) return 0;
    if (stock->quantity < quantity) {
        free(stock);
        return 0;
    }
    free(stock);
    return 1;
}

/*
 * Process a sale transaction
 * Updates stock quantity, records sale, and logs transaction
 * Returns: 1 on success, 0 on failure
 */
int process_sale(Sale *sale) {
    if (sale == NULL) return 0;
    
    Stock *stock = search_stock_by_id(sale->product_id);
    if (stock == NULL) {
        set_color(BRIGHT_RED);
        printf("Error: Product not found.\n");
        reset_color();
        return 0;
    }
    
    /* Update stock quantity */
    stock->quantity -= sale->quantity;
    
    if (!update_stock(sale->product_id, stock)) {
        free(stock);
        set_color(BRIGHT_RED);
        printf("Error: Failed to update stock.\n");
        reset_color();
        return 0;
    }
    
    /* Record sale in sales file */
    FILE *file = fopen(SALES_FILE, "ab");
    if (file == NULL) {
        free(stock);
        set_color(BRIGHT_RED);
        printf("Error: Unable to open sales file.\n");
        reset_color();
        return 0;
    }
    
    if (fwrite(sale, sizeof(Sale), 1, file) != 1) {
        fclose(file);
        free(stock);
        set_color(BRIGHT_RED);
        printf("Error: Failed to record sale.\n");
        reset_color();
        return 0;
    }
    fclose(file);
    
    /* Record transaction for sale */
    char note[MAX_TRANSACTION_NOTE];
    snprintf(note, sizeof(note), "Sale - Bill #%d", sale->bill_number);
    record_transaction(sale->product_id, TRANS_SALE, 
                      -(sale->quantity), stock->quantity, 
                      sale->price, note);
    
    free(stock);
    return 1;
}

float calculate_total_sales(void) {
    FILE *file = fopen(SALES_FILE, "rb");
    if (file == NULL) return 0.0f;
    Sale sale;
    float total = 0.0f;
    while (fread(&sale, sizeof(Sale), 1, file)) total += sale.total_amount;
    fclose(file);
    return total;
}

void generate_bill(Sale *sale) {
    char bill_filename[64];
    snprintf(bill_filename, sizeof(bill_filename), "data/bill_%d.txt", sale->bill_number);
    FILE *file = fopen(bill_filename, "w");
    if (file == NULL) return;
    char date[20];
    char time_str[20];
    get_current_date(date, sizeof(date));
    get_current_time(time_str, sizeof(time_str));
    fprintf(file, "========================================\n");
    fprintf(file, "      STOCK MANAGEMENT SYSTEM BILL      \n");
    fprintf(file, "========================================\n\n");
    fprintf(file, "Bill Number     : %d\n", sale->bill_number);
    fprintf(file, "Date            : %s\n", date);
    fprintf(file, "Time            : %s\n", time_str);
    fprintf(file, "----------------------------------------\n\n");
    fprintf(file, "Product Name    : %s\n", sale->product_name);
    fprintf(file, "Product ID      : %d\n", sale->product_id);
    fprintf(file, "Unit Price      : %.2f\n", sale->price);
    fprintf(file, "Quantity        : %d\n", sale->quantity);
    fprintf(file, "----------------------------------------\n");
    fprintf(file, "TOTAL AMOUNT    : %.2f\n", sale->total_amount);
    fprintf(file, "----------------------------------------\n\n");
    fprintf(file, "Thank you for your purchase!\n");
    fclose(file);
}

/*
 * Create a new sale transaction
 * Interactive function with validation and confirmation
 * Returns: 1 on success, 0 on failure
 */
int create_sale(void) {
    Sale sale;
    
    printf("\nEnter Product ID to sell: ");
    if (scanf("%d", &sale.product_id) != 1) {
        fflush(stdin);
        set_color(BRIGHT_RED);
        printf("Invalid input.\n");
        reset_color();
        return 0;
    }
    
    Stock *stock = search_stock_by_id(sale.product_id);
    if (stock == NULL) {
        set_color(BRIGHT_RED);
        printf("Product not found.\n");
        reset_color();
        return 0;
    }
    
    /* Copy product information */
    strncpy(sale.product_name, stock->name, sizeof(sale.product_name) - 1);
    sale.product_name[sizeof(sale.product_name) - 1] = '\0';
    sale.price = stock->price;
    
    /* Display product info */
    printf("\nProduct: %s\n", sale.product_name);
    printf("Available: %d units\n", stock->quantity);
    printf("Price per unit: %.2f\n", sale.price);
    
    if (stock->quantity == 0) {
        set_color(BRIGHT_RED);
        printf("\nError: Product is out of stock!\n");
        reset_color();
        free(stock);
        return 0;
    }
    
    /* Get quantity */
    int valid = 0;
    do {
        printf("\nEnter quantity to sell (1-%d): ", stock->quantity);
        if (scanf("%d", &sale.quantity) != 1) {
            fflush(stdin);
            set_color(BRIGHT_RED);
            printf("Invalid input. Please enter a number.\n");
            reset_color();
            continue;
        }
        
        if (sale.quantity <= 0) {
            set_color(BRIGHT_RED);
            printf("Quantity must be greater than 0.\n");
            reset_color();
        } else if (sale.quantity > stock->quantity) {
            set_color(BRIGHT_RED);
            printf("Insufficient stock. Only %d units available.\n", stock->quantity);
            reset_color();
        } else {
            valid = 1;
        }
    } while (!valid);
    
    free(stock);
    
    /* Calculate total */
    sale.total_amount = sale.price * sale.quantity;
    sale.bill_number = get_next_bill_number();
    sale.id = sale.bill_number;
    sale.sale_date = time(NULL);
    
    /* Display summary */
    printf("\n--- Sale Summary ---\n");
    printf("Product: %s\n", sale.product_name);
    printf("Quantity: %d\n", sale.quantity);
    printf("Unit Price: %.2f\n", sale.price);
    printf("Total Amount: %.2f\n", sale.total_amount);
    
    if (confirm_action("\nProcess this sale?")) {
        if (process_sale(&sale)) {
            generate_bill(&sale);
            
            char log_msg[200];
            snprintf(log_msg, sizeof(log_msg), 
                    "Sale #%d: %s (Qty: %d, Amount: %.2f)",
                    sale.bill_number, sale.product_name, 
                    sale.quantity, sale.total_amount);
            log_activity("SALE", log_msg);
            
            set_color(BRIGHT_GREEN);
            printf("\nSale processed successfully!\n");
            printf("Bill #%d has been generated.\n", sale.bill_number);
            reset_color();
            return 1;
        } else {
            set_color(BRIGHT_RED);
            printf("\nFailed to process sale.\n");
            reset_color();
        }
    } else {
        printf("\nSale cancelled.\n");
    }
    
    return 0;
}

void read_sales(void) {
    FILE *file = fopen(SALES_FILE, "rb");
    if (file == NULL) {
        printf("\nNo sales records found.\n");
        return;
    }
    Sale sale;
    printf("\n+----------+----------------------+-------+----------+----------+\n");
    printf("| %-8s | %-20s | %-5s | %-8s | %-8s |\n", "Bill#", "Product", "Qty", "Price", "Total");
    printf("+----------+----------------------+-------+----------+----------+\n");
    while (fread(&sale, sizeof(Sale), 1, file)) {
        printf("| %-8d | %-20s | %-5d | %-8.2f | %-8.2f |\n",
               sale.bill_number, sale.product_name, sale.quantity, sale.price, sale.total_amount);
    }
    printf("+----------+----------------------+-------+----------+----------+\n");
    fclose(file);
}

void display_sales_summary(void) {
    float total = calculate_total_sales();
    printf("\nTotal Sales: %.2f\n", total);
}

void display_most_sold_items(void) {
    FILE *file = fopen(SALES_FILE, "rb");
    if (file == NULL) {
        printf("\nNo sales records found.\n");
        return;
    }
    Sale *sales = NULL;
    int count = 0;
    Sale sale;
    while (fread(&sale, sizeof(Sale), 1, file)) count++;
    if (count == 0) {
        fclose(file);
        printf("\nNo sales records found.\n");
        return;
    }
    sales = (Sale *)malloc(sizeof(Sale) * count);
    rewind(file);
    for (int i = 0; i < count; i++) fread(&sales[i], sizeof(Sale), 1, file);
    fclose(file);
    typedef struct {
        int product_id;
        char product_name[50];
        int total_qty;
        float total_amount;
    } ProductSummary;
    ProductSummary *summary = (ProductSummary *)malloc(sizeof(ProductSummary) * count);
    int summary_count = 0;
    for (int i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < summary_count; j++) {
            if (summary[j].product_id == sales[i].product_id) {
                summary[j].total_qty += sales[i].quantity;
                summary[j].total_amount += sales[i].total_amount;
                found = 1;
                break;
            }
        }
        if (!found) {
            summary[summary_count].product_id = sales[i].product_id;
            strncpy(summary[summary_count].product_name, sales[i].product_name,
                    sizeof(summary[summary_count].product_name) - 1);
            summary[summary_count].product_name[sizeof(summary[summary_count].product_name) - 1] = '\0';
            summary[summary_count].total_qty = sales[i].quantity;
            summary[summary_count].total_amount = sales[i].total_amount;
            summary_count++;
        }
    }
    for (int i = 0; i < summary_count - 1; i++) {
        for (int j = 0; j < summary_count - i - 1; j++) {
            if (summary[j].total_qty < summary[j + 1].total_qty) {
                ProductSummary temp = summary[j];
                summary[j] = summary[j + 1];
                summary[j + 1] = temp;
            }
        }
    }
    printf("\n+----------+----------------------+-------+----------+\n");
    printf("| %-8s | %-20s | %-5s | %-8s |\n", "Prod ID", "Product", "Qty", "Revenue");
    printf("+----------+----------------------+-------+----------+\n");
    for (int i = 0; i < summary_count; i++) {
        printf("| %-8d | %-20s | %-5d | %-8.2f |\n",
               summary[i].product_id, summary[i].product_name, summary[i].total_qty, summary[i].total_amount);
    }
    printf("+----------+----------------------+-------+----------+\n");
    free(sales);
    free(summary);
}
