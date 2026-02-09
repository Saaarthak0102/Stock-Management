#ifndef GRAPH_H
#define GRAPH_H

#define GRAPH_WIDTH 60
#define GRAPH_HEIGHT 15
#define MAX_DATA_POINTS 30

/*
 * Generate and display an ASCII stock level trend graph
 * Shows how stock quantity has changed over time based on transaction history
 * 
 * Parameters:
 *   stock_id: The product ID to display trends for
 */
void display_stock_trend_graph(int stock_id);

/*
 * Display a simple bar graph for stock quantities across all products
 */
void display_stock_bar_graph(void);

/*
 * Display low stock items as a visual bar chart
 */
void display_low_stock_graph(void);

#endif
