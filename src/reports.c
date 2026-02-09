#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include "../include/reports.h"
#include "../include/sales.h"
#include "../include/stock.h"
#include "../include/supplier.h"
#include "../include/auth.h"
#include "../include/utils.h"

void log_activity(const char *action, const char *details) {
    FILE *file = fopen(LOGS_FILE, "ab");
    if (file == NULL) return;
    ActivityLog log;
    log.timestamp = time(NULL);
    strncpy(log.action, action, sizeof(log.action) - 1);
    log.action[sizeof(log.action) - 1] = '\0';
    strncpy(log.details, details, sizeof(log.details) - 1);
    log.details[sizeof(log.details) - 1] = '\0';
    fwrite(&log, sizeof(ActivityLog), 1, file);
    fclose(file);
}

void generate_daily_report(void) {
    FILE *file = fopen(SALES_FILE, "rb");
    if (file == NULL) {
        printf("\nNo sales records found.\n");
        return;
    }
    Sale sale;
    float total_amount = 0.0f;
    int transaction_count = 0;
    time_t today = time(NULL);
    struct tm *today_tm = localtime(&today);
    printf("\nDaily Sales Report for %04d-%02d-%02d\n",
           today_tm->tm_year + 1900, today_tm->tm_mon + 1, today_tm->tm_mday);
    printf("\n+----------+----------------------+-------+----------+\n");
    printf("| %-8s | %-20s | %-5s | %-8s |\n", "Bill#", "Product", "Qty", "Amount");
    printf("+----------+----------------------+-------+----------+\n");
    while (fread(&sale, sizeof(Sale), 1, file)) {
        struct tm *sale_tm = localtime(&sale.sale_date);
        if (sale_tm->tm_year == today_tm->tm_year &&
            sale_tm->tm_mon == today_tm->tm_mon &&
            sale_tm->tm_mday == today_tm->tm_mday) {
            printf("| %-8d | %-20s | %-5d | %-8.2f |\n",
                   sale.bill_number, sale.product_name, sale.quantity, sale.total_amount);
            total_amount += sale.total_amount;
            transaction_count++;
        }
    }
    fclose(file);
    printf("+----------+----------------------+-------+----------+\n");
    if (transaction_count == 0) {
        printf("\nNo sales today.\n");
    } else {
        printf("\nTotal Transactions: %d\n", transaction_count);
        printf("Total Sales: %.2f\n", total_amount);
    }
}

void generate_monthly_report(void) {
    FILE *file = fopen(SALES_FILE, "rb");
    if (file == NULL) {
        printf("\nNo sales records found.\n");
        return;
    }
    Sale sale;
    float total_amount = 0.0f;
    int transaction_count = 0;
    time_t now = time(NULL);
    struct tm *now_tm = localtime(&now);
    printf("\nMonthly Sales Report for %02d/%04d\n", now_tm->tm_mon + 1, now_tm->tm_year + 1900);
    printf("\n+----------+----------------------+-------+----------+\n");
    printf("| %-8s | %-20s | %-5s | %-8s |\n", "Bill#", "Product", "Qty", "Amount");
    printf("+----------+----------------------+-------+----------+\n");
    while (fread(&sale, sizeof(Sale), 1, file)) {
        struct tm *sale_tm = localtime(&sale.sale_date);
        if (sale_tm->tm_year == now_tm->tm_year && sale_tm->tm_mon == now_tm->tm_mon) {
            printf("| %-8d | %-20s | %-5d | %-8.2f |\n",
                   sale.bill_number, sale.product_name, sale.quantity, sale.total_amount);
            total_amount += sale.total_amount;
            transaction_count++;
        }
    }
    fclose(file);
    printf("+----------+----------------------+-------+----------+\n");
    if (transaction_count == 0) {
        printf("\nNo sales this month.\n");
    } else {
        printf("\nTotal Transactions: %d\n", transaction_count);
        printf("Total Sales: %.2f\n", total_amount);
    }
}

void generate_revenue_report(void) {
    float total_sales = calculate_total_sales();
    printf("\nTotal Revenue: %.2f\n", total_sales);
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file != NULL) {
        Stock stock;
        float inventory_value = 0.0f;
        while (fread(&stock, sizeof(Stock), 1, file)) {
            inventory_value += stock.price * stock.quantity;
        }
        fclose(file);
        printf("Current Inventory Value: %.2f\n", inventory_value);
    }
}

void display_stock_valuation(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo stock records found.\n");
        return;
    }
    Stock stock;
    float total_value = 0.0f;
    printf("\n+------------+----------------------+-------+----------+----------+\n");
    printf("| %-10s | %-20s | %-5s | %-8s | %-8s |\n", "ID", "Name", "Qty", "Price", "Value");
    printf("+------------+----------------------+-------+----------+----------+\n");
    while (fread(&stock, sizeof(Stock), 1, file)) {
        float item_value = stock.price * stock.quantity;
        printf("| %-10d | %-20s | %-5d | %-8.2f | %-8.2f |\n",
               stock.id, stock.name, stock.quantity, stock.price, item_value);
        total_value += item_value;
    }
    printf("+------------+----------------------+-------+----------+----------+\n");
    fclose(file);
    printf("\nTotal Inventory Value: %.2f\n", total_value);
}

void display_low_stock_report(void) {
    display_low_stock_alerts();
}

void display_activity_logs(void) {
    FILE *file = fopen(LOGS_FILE, "rb");
    if (file == NULL) {
        printf("\nNo activity logs found.\n");
        return;
    }
    ActivityLog log;
    printf("\nActivity Logs\n");
    while (fread(&log, sizeof(ActivityLog), 1, file)) {
        char time_str[30];
        struct tm *tm_info = localtime(&log.timestamp);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
        printf("[%s] %s: %s\n", time_str, log.action, log.details);
    }
    fclose(file);
}

void generate_inventory_report(void) {
    printf("\nInventory Report\n");
    read_stock();
}

int export_to_csv(void) {
    FILE *stock_in = fopen(STOCK_FILE, "rb");
    FILE *stock_out = fopen("data/stock_export.csv", "w");
    if (stock_in == NULL || stock_out == NULL) {
        if (stock_in) fclose(stock_in);
        if (stock_out) fclose(stock_out);
        return 0;
    }
    fprintf(stock_out, "ID,Name,Category,Price,Quantity,SupplierID\n");
    Stock stock;
    while (fread(&stock, sizeof(Stock), 1, stock_in)) {
        fprintf(stock_out, "%d,\"%s\",\"%s\",%.2f,%d,%d\n",
                stock.id, stock.name, stock.category, stock.price, stock.quantity, stock.supplier_id);
    }
    fclose(stock_in);
    fclose(stock_out);

    FILE *sales_in = fopen(SALES_FILE, "rb");
    FILE *sales_out = fopen("data/sales_export.csv", "w");
    if (sales_in == NULL || sales_out == NULL) {
        if (sales_in) fclose(sales_in);
        if (sales_out) fclose(sales_out);
        return 0;
    }
    fprintf(sales_out, "BillNumber,ProductID,ProductName,Quantity,Price,TotalAmount\n");
    Sale sale;
    while (fread(&sale, sizeof(Sale), 1, sales_in)) {
        fprintf(sales_out, "%d,%d,\"%s\",%d,%.2f,%.2f\n",
                sale.bill_number, sale.product_id, sale.product_name, sale.quantity, sale.price, sale.total_amount);
    }
    fclose(sales_in);
    fclose(sales_out);
    return 1;
}

int backup_data(void) {
    _mkdir("data/backup");
    const char *files[] = {STOCK_FILE, SALES_FILE, SUPPLIERS_FILE, LOGS_FILE, USERS_FILE};
    const char *names[] = {"stock.dat", "sales.dat", "suppliers.dat", "logs.dat", "users.dat"};
    for (int i = 0; i < 5; i++) {
        FILE *src = fopen(files[i], "rb");
        if (src == NULL) continue;
        char dst_path[128];
        snprintf(dst_path, sizeof(dst_path), "data/backup/%s", names[i]);
        FILE *dst = fopen(dst_path, "wb");
        if (dst == NULL) {
            fclose(src);
            return 0;
        }
        int c;
        while ((c = fgetc(src)) != EOF) fputc(c, dst);
        fclose(src);
        fclose(dst);
    }
    return 1;
}

int restore_data(void) {
    const char *files[] = {"data/backup/stock.dat", "data/backup/sales.dat", "data/backup/suppliers.dat", "data/backup/logs.dat", "data/backup/users.dat"};
    const char *targets[] = {STOCK_FILE, SALES_FILE, SUPPLIERS_FILE, LOGS_FILE, USERS_FILE};
    for (int i = 0; i < 5; i++) {
        FILE *src = fopen(files[i], "rb");
        if (src == NULL) continue;
        FILE *dst = fopen(targets[i], "wb");
        if (dst == NULL) {
            fclose(src);
            return 0;
        }
        int c;
        while ((c = fgetc(src)) != EOF) fputc(c, dst);
        fclose(src);
        fclose(dst);
    }
    return 1;
}

void display_most_sold_report(void) {
    display_most_sold_items();
}
