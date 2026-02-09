/*
 * Graph Visualization Module
 * 
 * Provides ASCII-based visualization capabilities for stock data analysis.
 * All graphs are terminal-based with no external dependencies.
 * 
 * Features:
 *   - Stock level trend graphs (time-series line graphs)
 *   - Comparative bar graphs (all products)
 *   - Low stock visual alerts (color-coded)
 * 
 * Algorithms:
 *   - Min-max normalization for Y-axis scaling
 *   - Dynamic range adjustment
 *   - Color coding based on thresholds
 * 
 * Design:
 *   - No external libraries (pure C)
 *   - Configurable dimensions
 *   - Responsive to data range
 *   - Color-coded output for UX
 * 
 * Usage:
 *   display_stock_trend_graph(product_id);
 *   display_stock_bar_graph();
 *   display_low_stock_graph();
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/graph.h"
#include "../include/stock.h"
#include "../include/transaction.h"
#include "../include/utils.h"

/* Helper to find min/max values in array */
static void find_min_max(int *values, int count, int *min, int *max) {
    if (count == 0) {
        *min = 0;
        *max = 0;
        return;
    }
    
    *min = values[0];
    *max = values[0];
    
    for (int i = 1; i < count; i++) {
        if (values[i] < *min) *min = values[i];
        if (values[i] > *max) *max = values[i];
    }
}

/* Helper to normalize value to graph height */
static int normalize_to_height(int value, int min, int max, int height) {
    if (max == min) return height / 2;
    return (int)(((float)(value - min) / (max - min)) * (height - 1));
}

void display_stock_trend_graph(int stock_id) {
    Stock *stock = search_stock_by_id(stock_id);
    if (stock == NULL) {
        printf("\nProduct not found.\n");
        return;
    }
    
    printf("\n");
    set_color(BRIGHT_CYAN);
    printf("Stock Level Trend: %s (ID: %d)\n", stock->name, stock->id);
    reset_color();
    
    int count = 0;
    Transaction *transactions = get_stock_transactions(stock_id, &count);
    
    if (transactions == NULL || count == 0) {
        printf("\nInsufficient transaction history to generate trend graph.\n");
        free(stock);
        return;
    }
    
    /* Limit to last MAX_DATA_POINTS transactions for readability */
    int start_index = (count > MAX_DATA_POINTS) ? (count - MAX_DATA_POINTS) : 0;
    int points_to_show = count - start_index;
    
    int *quantities = (int *)malloc(sizeof(int) * points_to_show);
    char (*dates)[12] = malloc(sizeof(char[12]) * points_to_show);
    
    if (quantities == NULL || dates == NULL) {
        free(stock);
        free(transactions);
        if (quantities) free(quantities);
        if (dates) free(dates);
        printf("\nMemory allocation error.\n");
        return;
    }
    
    /* Extract quantities and dates */
    for (int i = 0; i < points_to_show; i++) {
        quantities[i] = transactions[start_index + i].quantity_after;
        struct tm *tm_info = localtime(&transactions[start_index + i].timestamp);
        strftime(dates[i], 12, "%m/%d", tm_info);
    }
    
    /* Find min and max for scaling */
    int min_qty, max_qty;
    find_min_max(quantities, points_to_show, &min_qty, &max_qty);
    
    /* Ensure some vertical range */
    if (max_qty == min_qty) {
        max_qty = min_qty + 10;
    }
    
    /* Print graph header */
    printf("\nQuantity Range: %d - %d\n", min_qty, max_qty);
    printf("\n");
    
    /* Print Y-axis labels and graph */
    for (int row = GRAPH_HEIGHT - 1; row >= 0; row--) {
        int qty_at_row = min_qty + (int)(((float)row / (GRAPH_HEIGHT - 1)) * (max_qty - min_qty));
        printf("%4d | ", qty_at_row);
        
        for (int col = 0; col < points_to_show; col++) {
            int normalized = normalize_to_height(quantities[col], min_qty, max_qty, GRAPH_HEIGHT);
            
            if (normalized == row) {
                set_color(BRIGHT_GREEN);
                printf("*");
                reset_color();
            } else if (normalized > row) {
                set_color(DARK_GREEN);
                printf("|");
                reset_color();
            } else {
                printf(" ");
            }
            
            /* Add spacing for readability */
            if (points_to_show <= 15) {
                printf(" ");
            }
        }
        printf("\n");
    }
    
    /* Print X-axis */
    printf("     +");
    for (int i = 0; i < (points_to_show * (points_to_show <= 15 ? 2 : 1)); i++) {
        printf("-");
    }
    printf("\n");
    
    /* Print date labels (every few points to avoid clutter) */
    printf("      ");
    int step = (points_to_show > 20) ? 5 : ((points_to_show > 10) ? 3 : 2);
    for (int i = 0; i < points_to_show; i += step) {
        printf("%-6s", dates[i]);
        if (points_to_show <= 15) printf("  ");
    }
    printf("\n\n");
    
    /* Summary statistics */
    printf("Current Stock: %d units\n", stock->quantity);
    printf("Total Sold: %d units\n", get_total_quantity_sold(stock_id));
    printf("Total Purchased: %d units\n", get_total_quantity_purchased(stock_id));
    
    /* Trend indicator */
    if (points_to_show >= 2) {
        int recent_avg = (quantities[points_to_show - 1] + quantities[points_to_show - 2]) / 2;
        int older_avg = (quantities[0] + quantities[1]) / 2;
        
        printf("Trend: ");
        if (recent_avg > older_avg) {
            set_color(BRIGHT_GREEN);
            printf("Increasing ▲\n");
        } else if (recent_avg < older_avg) {
            set_color(BRIGHT_RED);
            printf("Decreasing ▼\n");
        } else {
            set_color(BRIGHT_YELLOW);
            printf("Stable →\n");
        }
        reset_color();
    }
    
    free(stock);
    free(transactions);
    free(quantities);
    free(dates);
}

void display_stock_bar_graph(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo stock records found.\n");
        return;
    }
    
    /* Count and load stocks */
    Stock temp;
    int count = 0;
    while (fread(&temp, sizeof(Stock), 1, file)) count++;
    
    if (count == 0) {
        fclose(file);
        printf("\nNo stock records found.\n");
        return;
    }
    
    Stock *stocks = (Stock *)malloc(sizeof(Stock) * count);
    if (stocks == NULL) {
        fclose(file);
        return;
    }
    
    rewind(file);
    for (int i = 0; i < count; i++) {
        fread(&stocks[i], sizeof(Stock), 1, file);
    }
    fclose(file);
    
    /* Find max quantity for scaling */
    int max_qty = 0;
    for (int i = 0; i < count; i++) {
        if (stocks[i].quantity > max_qty) {
            max_qty = stocks[i].quantity;
        }
    }
    
    if (max_qty == 0) max_qty = 1;
    
    /* Display bar graph */
    printf("\n");
    set_color(BRIGHT_CYAN);
    printf("Stock Quantity Bar Graph\n");
    reset_color();
    printf("\n");
    
    /* Show up to 20 products to avoid clutter */
    int display_count = (count > 20) ? 20 : count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%-20.20s | ", stocks[i].name);
        
        int bar_length = (int)((float)stocks[i].quantity / max_qty * 50);
        
        /* Color code based on stock level */
        if (stocks[i].quantity <= LOW_STOCK_THRESHOLD) {
            set_color(BRIGHT_RED);
        } else if (stocks[i].quantity <= LOW_STOCK_THRESHOLD * 2) {
            set_color(BRIGHT_YELLOW);
        } else {
            set_color(BRIGHT_GREEN);
        }
        
        for (int j = 0; j < bar_length; j++) {
            printf("█");
        }
        reset_color();
        
        printf(" %d\n", stocks[i].quantity);
    }
    
    if (count > 20) {
        printf("\n(Showing first 20 of %d products)\n", count);
    }
    
    free(stocks);
}

void display_low_stock_graph(void) {
    FILE *file = fopen(STOCK_FILE, "rb");
    if (file == NULL) {
        printf("\nNo stock records found.\n");
        return;
    }
    
    Stock temp;
    Stock *low_stocks = NULL;
    int count = 0;
    
    /* Find low stock items */
    while (fread(&temp, sizeof(Stock), 1, file)) {
        if (temp.quantity <= LOW_STOCK_THRESHOLD) {
            low_stocks = (Stock *)realloc(low_stocks, sizeof(Stock) * (count + 1));
            if (low_stocks != NULL) {
                low_stocks[count++] = temp;
            }
        }
    }
    fclose(file);
    
    if (count == 0) {
        printf("\nNo low stock items found.\n");
        return;
    }
    
    printf("\n");
    set_color(BRIGHT_RED);
    printf("⚠ LOW STOCK ALERT - Visual Graph\n");
    reset_color();
    printf("\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20.20s | ", low_stocks[i].name);
        
        int bar_length = (int)((float)low_stocks[i].quantity / LOW_STOCK_THRESHOLD * 20);
        
        set_color(BRIGHT_RED);
        for (int j = 0; j < bar_length; j++) {
            printf("■");
        }
        reset_color();
        
        printf(" %d / %d\n", low_stocks[i].quantity, LOW_STOCK_THRESHOLD);
    }
    
    printf("\n");
    set_color(BRIGHT_YELLOW);
    printf("Total low stock items: %d\n", count);
    reset_color();
    
    free(low_stocks);
}
