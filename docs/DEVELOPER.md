# Developer Documentation - Stock Management System

## Table of Contents
1. [System Architecture](#system-architecture)
2. [Module Descriptions](#module-descriptions)
3. [Data Structures](#data-structures)
4. [API Reference](#api-reference)
5. [File Formats](#file-formats)
6. [Adding New Features](#adding-new-features)
7. [Best Practices](#best-practices)

## System Architecture

### Layer Architecture
```
┌─────────────────────────────────────┐
│        Presentation Layer           │
│         (menu.c, main.c)           │
├─────────────────────────────────────┤
│        Business Logic Layer         │
│  (stock.c, sales.c, supplier.c,    │
│   transaction.c, reports.c)         │
├─────────────────────────────────────┤
│      Data Persistence Layer         │
│     (Binary File I/O in each       │
│          module)                    │
├─────────────────────────────────────┤
│        Utility Layer                │
│  (utils.c - validation, UI helpers) │
└─────────────────────────────────────┘
```

### Design Principles
- **Separation of Concerns**: UI logic separated from business logic
- **Single Responsibility**: Each module handles one domain
- **DRY (Don't Repeat Yourself)**: Common functions in utils.c
- **Defensive Programming**: Validate all inputs, check all returns

## Module Descriptions

### auth.c / auth.h
**Purpose**: User authentication and authorization

**Key Functions**:
- `authenticate_user()` - Main login flow
- `hash_password()` - Simple password hashing
- `verify_password()` - Password verification
- `init_users_file()` - Create default admin user

**Security Notes**:
- Current hashing is Caesar cipher (+3) - for demonstration only
- In production, use bcrypt or similar
- Passwords stored in `data/users.dat`

### stock.c / stock.h
**Purpose**: Core inventory management operations

**Key Functions**:
- `add_stock()` - Add new product with validation
- `update_stock()` - Modify existing product
- `delete_stock()` - Remove product
- `search_stock_by_id()` - ID-based lookup
- `search_stock_by_name()` - Name-based search (case-insensitive)
- `display_low_stock_alerts()` - Show items below threshold

**Important Notes**:
- All write operations update `modified_at` timestamp
- Transactions are recorded for all quantity changes
- Memory returned by search functions must be freed by caller

### sales.c / sales.h
**Purpose**: Sales transaction processing

**Key Functions**:
- `create_sale()` - Interactive sale creation
- `process_sale()` - Execute sale, update stock, record transaction
- `generate_bill()` - Create text receipt
- `display_most_sold_items()` - Sales analytics

**Transaction Flow**:
1. Validate product availability
2. Get quantity from user
3. Calculate total amount
4. Update stock quantity
5. Record sale in sales.dat
6. Record transaction in transactions.dat
7. Generate bill file
8. Log activity

### transaction.c / transaction.h
**Purpose**: Track all stock movement history

**Key Functions**:
- `record_transaction()` - Add transaction record
- `get_stock_transactions()` - Retrieve history for a product
- `display_stock_transaction_history()` - Show formatted history
- `display_recent_transactions()` - Show last N transactions

**Transaction Types**:
- `TRANS_PURCHASE` - Stock added/purchased
- `TRANS_SALE` - Stock sold
- `TRANS_ADJUSTMENT` - Manual correction

### graph.c / graph.h
**Purpose**: ASCII visualization of data

**Key Functions**:
- `display_stock_trend_graph()` - Time-series line graph
- `display_stock_bar_graph()` - Quantity comparison bars
- `display_low_stock_graph()` - Visual low stock alert

**Algorithm Details**:
- Min-max normalization for Y-axis scaling
- Configurable graph dimensions (GRAPH_WIDTH, GRAPH_HEIGHT)
- Color coding based on stock levels

### supplier.c / supplier.h
**Purpose**: Supplier relationship management

**Key Functions**:
- `add_supplier()` - Register new supplier
- `update_supplier()` - Modify supplier info
- `delete_supplier()` - Remove supplier
- `display_supplier_stock()` - Show products by supplier

### reports.c / reports.h
**Purpose**: Analytics and reporting

**Key Functions**:
- `generate_daily_report()` - Today's sales
- `generate_monthly_report()` - Current month sales
- `display_stock_valuation()` - Inventory value
- `export_to_csv()` - Data export
- `log_activity()` - System audit trail

### utils.c / utils.h
**Purpose**: Shared utility functions

**Categories**:
1. **UI Functions**: `clear_screen()`, `pause_screen()`, `set_color()`
2. **Validation**: `validate_integer()`, `validate_email()`, `validate_alphabetic_string()`
3. **String Ops**: `trim_string()`, `to_uppercase()`, `to_lowercase()`
4. **Date/Time**: `get_current_date()`, `get_current_time()`
5. **User Interaction**: `confirm_action()`

## Data Structures

### Stock Structure
```c
typedef struct {
    int id;                      // Unique identifier
    char name[50];               // Product name
    char category[30];           // Category
    int quantity;                // Current stock level
    float price;                 // Unit price
    int supplier_id;             // FK to supplier
    time_t created_at;           // Creation timestamp
    time_t modified_at;          // Last modification
} Stock;
```

### Transaction Structure
```c
typedef struct {
    int id;                      // Transaction ID
    int stock_id;                // FK to stock
    TransactionType type;        // Purchase/Sale/Adjustment
    int quantity_change;         // +/- quantity
    int quantity_after;          // Stock level after
    float unit_price;            // Price at transaction time
    time_t timestamp;            // When transaction occurred
    char note[100];              // Optional description
} Transaction;
```

### Sale Structure
```c
typedef struct {
    int id;                      // Sale ID
    int bill_number;             // Invoice number
    int product_id;              // FK to stock
    int quantity;                // Quantity sold
    float price;                 // Unit price
    float total_amount;          // quantity * price
    time_t sale_date;            // Sale timestamp
    char product_name[50];       // Denormalized for reporting
} Sale;
```

## File Formats

All data files use binary format for efficiency:

```
data/
├── stock.dat         - Array of Stock structs
├── sales.dat         - Array of Sale structs
├── suppliers.dat     - Array of Supplier structs
├── transactions.dat  - Array of Transaction structs
├── users.dat         - Array of User structs
└── logs.dat          - Array of ActivityLog structs
```

### File I/O Pattern
```c
// Reading
FILE *file = fopen("data/stock.dat", "rb");
Stock stock;
while (fread(&stock, sizeof(Stock), 1, file)) {
    // Process stock
}
fclose(file);

// Writing
FILE *file = fopen("data/stock.dat", "ab");  // append mode
fwrite(&stock, sizeof(Stock), 1, file);
fclose(file);

// Updating (requires temp file)
FILE *src = fopen("data/stock.dat", "rb");
FILE *temp = fopen("data/temp.dat", "wb");
// Copy all except the one to update
// Write updated record
fclose(src);
fclose(temp);
remove("data/stock.dat");
rename("data/temp.dat", "data/stock.dat");
```

## Adding New Features

### Step 1: Define Data Structure
```c
// In include/newfeature.h
typedef struct {
    int id;
    // ... fields
} NewFeature;
```

### Step 2: Implement CRUD Operations
```c
// In src/newfeature.c
int add_newfeature(NewFeature *item);
NewFeature* search_newfeature(int id);
int update_newfeature(int id, NewFeature *item);
int delete_newfeature(int id);
```

### Step 3: Add Menu Option
```c
// In src/menu.c
void display_newfeature_menu(void) {
    // Menu implementation
}
```

### Step 4: Update Build System
```makefile
# In Makefile - automatic with wildcard
# In build.bat and tasks.json
gcc ... src/newfeature.c ...
```

## Best Practices

### Error Handling
```c
// Always check file operations
FILE *file = fopen("data.dat", "rb");
if (file == NULL) {
    set_color(BRIGHT_RED);
    printf("Error: Unable to open file.\n");
    reset_color();
    return 0;
}

// Check memory allocations
Stock *stock = (Stock *)malloc(sizeof(Stock));
if (stock == NULL) {
    return NULL;
}
```

### Input Validation
```c
int valid = 0;
do {
    printf("Enter quantity (1-500): ");
    if (scanf("%d", &quantity) != 1) {
        fflush(stdin);
        printf("Invalid input.\n");
        continue;
    }
    if (quantity >= 1 && quantity <= 500) {
        valid = 1;
    }
} while (!valid);
```

### Memory Management
```c
// Always free allocated memory
Stock *stock = search_stock_by_id(id);
if (stock != NULL) {
    // Use stock
    free(stock);  // Don't forget this!
}
```

### Transaction Consistency
```c
// When modifying stock, always record transaction
if (update_stock(id, stock)) {
    record_transaction(id, TRANS_ADJUSTMENT, 
                      change, new_qty, price, "Note");
    log_activity("UPDATE", details);
}
```

### Code Documentation
```c
/*
 * Function description
 * 
 * Parameters:
 *   param1: Description
 *   param2: Description
 * 
 * Returns: Description
 * 
 * Notes: Any important considerations
 */
```

## Future Enhancement Paths

### Easy Additions
- [ ] Category-based filtering
- [ ] Expiry date tracking
- [ ] Batch/lot number tracking
- [ ] Multi-currency support
- [ ] Discount/promotion handling

### Medium Complexity
- [ ] ncurses terminal UI
- [ ] SQLite database backend
- [ ] Multi-user with roles
- [ ] Real encryption (bcrypt)
- [ ] Network/client-server

### Advanced
- [ ] Web interface (CGI/embedded server)
- [ ] RESTful API
- [ ] Barcode scanning integration
- [ ] Email notifications
- [ ] Advanced analytics (ML predictions)

## Debugging Tips

### Common Issues
1. **File not found**: Ensure `data/` directory exists
2. **Corrupted data**: Delete .dat files, restart with fresh data
3. **Memory leaks**: Use valgrind or similar tools
4. **Segmentation faults**: Check pointer validity before use

### Debug Builds
```bash
gcc -g -Wall -Wextra -std=c99 -o stock_system.exe src/*.c
gdb stock_system.exe
```

### Logging
All operations are logged in `data/logs.dat`. View via "Activity Logs" menu.

## Testing Checklist

- [ ] Add product with various inputs (valid, invalid, edge cases)
- [ ] Update product (verify transaction recorded)
- [ ] Delete product
- [ ] Process sale (check stock updated, transaction recorded, bill generated)
- [ ] Low stock alert (add product with qty < threshold)
- [ ] Transaction history (verify all operations appear)
- [ ] Trend graph (requires multiple transactions)
- [ ] Export to CSV (check file created)
- [ ] Login (wrong password, max attempts)

## Performance Considerations

### Current Limitations
- Linear search: O(n) - acceptable for < 1000 products
- Bubble sort: O(n²) - acceptable for < 500 products
- File rewrite for updates: Slow for large datasets

### Optimization Opportunities
1. Index file for faster lookups
2. Quicksort for larger datasets
3. Cache frequently accessed records
4. Batch operations

## Version History

### v2.0 (Current)
- Added transaction history tracking
- Implemented ASCII trend graphs
- Enhanced input validation
- Improved error handling
- Added comprehensive documentation

### v1.0 (Original)
- Basic CRUD operations
- Sales and billing
- Supplier management
- Simple reporting

---

**Maintained by**: Development Team  
**Last Updated**: February 2026
