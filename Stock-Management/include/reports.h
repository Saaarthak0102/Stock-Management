#ifndef REPORTS_H
#define REPORTS_H

#include <time.h>

#define LOGS_FILE "data/logs.dat"

typedef struct {
    time_t timestamp;
    char action[100];
    char details[200];
} ActivityLog;

void generate_daily_report(void);
void generate_monthly_report(void);
void display_most_sold_report(void);
void display_low_stock_report(void);
void generate_revenue_report(void);
void display_stock_valuation(void);
void log_activity(const char *action, const char *details);
void display_activity_logs(void);
void generate_inventory_report(void);
int export_to_csv(void);
int backup_data(void);
int restore_data(void);

#endif
