#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <time.h>

#define TRANSACTION_FILE "data/transactions.dat"
#define MAX_TRANSACTION_NOTE 100

/* Transaction types */
typedef enum {
    TRANS_PURCHASE,     /* Stock added/purchased */
    TRANS_SALE,         /* Stock sold */
    TRANS_ADJUSTMENT    /* Manual stock adjustment */
} TransactionType;

/* Transaction record for tracking stock changes */
typedef struct {
    int id;
    int stock_id;
    TransactionType type;
    int quantity_change;    /* Positive for add, negative for subtract */
    int quantity_after;     /* Stock level after transaction */
    float unit_price;
    time_t timestamp;
    char note[MAX_TRANSACTION_NOTE];
} Transaction;

/*
 * Record a new transaction
 * Returns: 1 on success, 0 on failure
 */
int record_transaction(int stock_id, TransactionType type, int qty_change, 
                       int qty_after, float price, const char *note);

/*
 * Retrieve transaction history for a specific stock item
 * Returns: Array of transactions (caller must free), count stored in out_count
 */
Transaction* get_stock_transactions(int stock_id, int *out_count);

/*
 * Display transaction history for a stock item
 */
void display_stock_transaction_history(int stock_id);

/*
 * Display all recent transactions (last N records)
 */
void display_recent_transactions(int limit);

/*
 * Get the next transaction ID
 */
int get_next_transaction_id(void);

/*
 * Calculate total quantity sold for a stock item
 */
int get_total_quantity_sold(int stock_id);

/*
 * Calculate total quantity purchased/added for a stock item
 */
int get_total_quantity_purchased(int stock_id);

#endif
