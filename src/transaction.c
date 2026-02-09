/*
 * Transaction History Module
 * 
 * Maintains a complete audit trail of all stock movements.
 * Every change to inventory is recorded with full details.
 * 
 * Features:
 *   - Record purchases, sales, and manual adjustments
 *   - Track quantity before/after changes
 *   - Timestamp every transaction
 *   - Support contextual notes
 *   - Retrieve history by product
 *   - Display formatted transaction logs
 * 
 * Transaction Types:
 *   TRANS_PURCHASE   - Stock added/purchased
 *   TRANS_SALE       - Stock sold to customer
 *   TRANS_ADJUSTMENT - Manual inventory correction
 * 
 * Data Persistence:
 *   - Binary file format (transactions.dat)
 *   - Append-only writes for performance
 *   - Sequential IDs for tracking
 * 
 * Integration:
 *   - Called by add_stock() for purchases
 *   - Called by process_sale() for sales
 *   - Called by stock quantity updates
 * 
 * Benefits:
 *   - Complete audit trail
 *   - Enables trend analysis
 *   - Supports accountability
 *   - Historical reporting
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/transaction.h"
#include "../include/utils.h"

int get_next_transaction_id(void) {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) return 1;
    
    Transaction trans;
    int max_id = 0;
    
    while (fread(&trans, sizeof(Transaction), 1, file)) {
        if (trans.id > max_id) {
            max_id = trans.id;
        }
    }
    fclose(file);
    return max_id + 1;
}

int record_transaction(int stock_id, TransactionType type, int qty_change, 
                       int qty_after, float price, const char *note) {
    if (stock_id <= 0) return 0;
    
    FILE *file = fopen(TRANSACTION_FILE, "ab");
    if (file == NULL) {
        /* Create file if it doesn't exist */
        file = fopen(TRANSACTION_FILE, "wb");
        if (file == NULL) return 0;
    }
    
    Transaction trans;
    trans.id = get_next_transaction_id();
    trans.stock_id = stock_id;
    trans.type = type;
    trans.quantity_change = qty_change;
    trans.quantity_after = qty_after;
    trans.unit_price = price;
    trans.timestamp = time(NULL);
    
    if (note != NULL) {
        strncpy(trans.note, note, sizeof(trans.note) - 1);
        trans.note[sizeof(trans.note) - 1] = '\0';
    } else {
        trans.note[0] = '\0';
    }
    
    int result = fwrite(&trans, sizeof(Transaction), 1, file);
    fclose(file);
    
    return result;
}

Transaction* get_stock_transactions(int stock_id, int *out_count) {
    if (out_count == NULL) return NULL;
    *out_count = 0;
    
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) return NULL;
    
    /* Count matching transactions */
    Transaction temp;
    int count = 0;
    while (fread(&temp, sizeof(Transaction), 1, file)) {
        if (temp.stock_id == stock_id) {
            count++;
        }
    }
    
    if (count == 0) {
        fclose(file);
        return NULL;
    }
    
    /* Allocate and read transactions */
    Transaction *transactions = (Transaction *)malloc(sizeof(Transaction) * count);
    if (transactions == NULL) {
        fclose(file);
        return NULL;
    }
    
    rewind(file);
    int index = 0;
    while (fread(&temp, sizeof(Transaction), 1, file)) {
        if (temp.stock_id == stock_id) {
            transactions[index++] = temp;
        }
    }
    
    fclose(file);
    *out_count = count;
    return transactions;
}

void display_stock_transaction_history(int stock_id) {
    int count = 0;
    Transaction *transactions = get_stock_transactions(stock_id, &count);
    
    if (transactions == NULL || count == 0) {
        printf("\nNo transaction history found for this product.\n");
        return;
    }
    
    printf("\n+------+------------+---------------+--------+----------+--------+\n");
    printf("| %-4s | %-10s | %-13s | %-6s | %-8s | %-6s |\n", 
           "ID", "Date", "Type", "Change", "After", "Price");
    printf("+------+------------+---------------+--------+----------+--------+\n");
    
    for (int i = 0; i < count; i++) {
        char date_str[20];
        struct tm *tm_info = localtime(&transactions[i].timestamp);
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);
        
        const char *type_str;
        switch (transactions[i].type) {
            case TRANS_PURCHASE:
                type_str = "Purchase";
                break;
            case TRANS_SALE:
                type_str = "Sale";
                break;
            case TRANS_ADJUSTMENT:
                type_str = "Adjustment";
                break;
            default:
                type_str = "Unknown";
        }
        
        printf("| %-4d | %-10s | %-13s | %+6d | %-8d | %6.2f |\n",
               transactions[i].id, date_str, type_str, 
               transactions[i].quantity_change,
               transactions[i].quantity_after, 
               transactions[i].unit_price);
    }
    
    printf("+------+------------+---------------+--------+----------+--------+\n");
    
    free(transactions);
}

void display_recent_transactions(int limit) {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) {
        printf("\nNo transaction records found.\n");
        return;
    }
    
    /* Count total transactions */
    Transaction temp;
    int total = 0;
    while (fread(&temp, sizeof(Transaction), 1, file)) {
        total++;
    }
    
    if (total == 0) {
        fclose(file);
        printf("\nNo transaction records found.\n");
        return;
    }
    
    /* Read all transactions */
    Transaction *transactions = (Transaction *)malloc(sizeof(Transaction) * total);
    if (transactions == NULL) {
        fclose(file);
        return;
    }
    
    rewind(file);
    for (int i = 0; i < total; i++) {
        fread(&transactions[i], sizeof(Transaction), 1, file);
    }
    fclose(file);
    
    /* Display last 'limit' transactions */
    int start = (total > limit) ? (total - limit) : 0;
    
    printf("\n+------+----------+------------+---------------+--------+----------+\n");
    printf("| %-4s | %-8s | %-10s | %-13s | %-6s | %-8s |\n", 
           "ID", "Stock ID", "Date", "Type", "Change", "After");
    printf("+------+----------+------------+---------------+--------+----------+\n");
    
    for (int i = start; i < total; i++) {
        char date_str[20];
        struct tm *tm_info = localtime(&transactions[i].timestamp);
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_info);
        
        const char *type_str;
        switch (transactions[i].type) {
            case TRANS_PURCHASE:
                type_str = "Purchase";
                break;
            case TRANS_SALE:
                type_str = "Sale";
                break;
            case TRANS_ADJUSTMENT:
                type_str = "Adjustment";
                break;
            default:
                type_str = "Unknown";
        }
        
        printf("| %-4d | %-8d | %-10s | %-13s | %+6d | %-8d |\n",
               transactions[i].id, transactions[i].stock_id, 
               date_str, type_str,
               transactions[i].quantity_change,
               transactions[i].quantity_after);
    }
    
    printf("+------+----------+------------+---------------+--------+----------+\n");
    
    free(transactions);
}

int get_total_quantity_sold(int stock_id) {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) return 0;
    
    Transaction trans;
    int total = 0;
    
    while (fread(&trans, sizeof(Transaction), 1, file)) {
        if (trans.stock_id == stock_id && trans.type == TRANS_SALE) {
            total += abs(trans.quantity_change);
        }
    }
    
    fclose(file);
    return total;
}

int get_total_quantity_purchased(int stock_id) {
    FILE *file = fopen(TRANSACTION_FILE, "rb");
    if (file == NULL) return 0;
    
    Transaction trans;
    int total = 0;
    
    while (fread(&trans, sizeof(Transaction), 1, file)) {
        if (trans.stock_id == stock_id && trans.type == TRANS_PURCHASE) {
            total += trans.quantity_change;
        }
    }
    
    fclose(file);
    return total;
}
