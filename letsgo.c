// store_management.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define MAX_PRODUCTS 1000
#define MAX_EMPLOYEES 100
#define MAX_CUSTOMERS 500
#define MAX_USERID 20
#define MAX_PASSWORD 20
#define MAX_NAME 50
#define MAX_PAYMENT_MODE 10
#define INVENTORY_FILE "inventory.csv"
#define SALES_FILE "sales.csv"
#define PURCHASE_FILE "purchases.csv"
#define EMPLOYEES_FILE "employees.csv"
#define CUSTOMERS_FILE "customers.csv"
#define PRODUCTS_FILE "products.csv"

// Constants for inventory
#define MAX_CATEGORY 30
#define MAX_SUPPLIER 50
#define MIN_STOCK_THRESHOLD 10
#define MAX_DATE 12 // Format: DD/MM/YYYY

// Constants for Sales
#define MAX_DISCOUNT 100 // Maximum allowable discount percentage

// Constants for Purchase
#define MAX_SUPPLIERS 100
#define MAX_PURCHASES 1000

// Constants for Employee Management
#define ADMIN "admin"
#define CASHIER "cashier"
#define MANAGER "manager"

// Structures
typedef struct {
    char productID[10];
    char productName[MAX_NAME];
    char category[MAX_CATEGORY];
    char supplier[MAX_SUPPLIER];
    int quantity;
    float costPrice;
    float sellingPrice;
    char expiryDate[MAX_DATE]; // Optional, use "N/A" if not applicable
} Product;

// Structure for Employee
typedef struct {
    char employeeID[10];
    char name[MAX_NAME];
    char role[20];  // Role like "admin", "cashier", "manager"
    char contactInfo[50];
    int attendance[30];  // Store attendance for 30 days
    int attendanceCount; // Number of days attended in the month
} Employee;

typedef struct {
    char customerID[10];
    char name[MAX_NAME];
    char contact[15];
} Customer;

typedef struct {
    char saleID[10];
    char productID[10];
    char customerID[10];
    int quantitySold;
    float totalAmount;
    char paymentMode[MAX_PAYMENT_MODE]; // cash, card, UPI
    char timeStamp[20];
} Sale;

typedef struct {
    char purchaseID[10];
    char productID[10];
    int quantityPurchased;
    float totalAmount;
    char supplierName[MAX_NAME];
    char timeStamp[20];
} Purchase;

typedef struct {
    char userID[MAX_USERID];
    char password[MAX_PASSWORD];
} User;

// Structures for Supplier and Purchase
typedef struct {
    char supplierID[10];
    char supplierName[MAX_NAME];
    char contactInfo[50];
    char address[100];
} Supplier;

typedef struct {
    char purchaseID[10];
    char productID[10];
    char supplierID[10];
    int quantityPurchased;
    float totalCost;
    char timeStamp[MAX_DATE_TIME];
} Purchase;


// Function prototypes
void login();
void initializeFiles();
void loadInventory(Product inventory[], int *size);
void saveInventory(Product inventory[], int size);
void addProduct(Product inventory[], int *size);
void updateInventory(Product inventory[], int size);
void showInventory(Product inventory[], int size);

void handleSales(Product inventory[], int *inventorySize, Sale sales[], int *salesCount);
void recordSale(Product inventory[], int *inventorySize, Sale *sale);
void saveSales(Sale sales[], int size);
void loadSales(Sale sales[], int *size);
void showSales(Sale sales[], int size);

void handlePurchases(Product inventory[], int *inventorySize, Purchase purchases[], int *purchaseCount);
void recordPurchase(Product inventory[], int *inventorySize, Purchase *purchase);
void savePurchases(Purchase purchases[], int size);
void loadPurchases(Purchase purchases[], int *size);
void showPurchases(Purchase purchases[], int size);

void manageCustomers(Customer customers[], int *customerCount);
void addCustomer(Customer customers[], int *customerCount);
void showCustomers(Customer customers[], int customerCount);

void manageEmployees(Employee employees[], int *employeeCount);
void addEmployee(Employee employees[], int *employeeCount);
void markAttendance(Employee employees[], int employeeCount);
void showAttendance(Employee employees[], int employeeCount);

void handlePayment(float totalAmount);
void getCurrentTime(char *buffer);
int findProductIndex(Product inventory[], int size, const char *productID);
int findCustomerIndex(Customer customers[], int size, const char *customerID);
int findEmployeeIndex(Employee employees[], int size, const char *employeeID);

// Helper functions
void clearBuffer();
void pauseAndClear();
void printLineSeparator();

// Inventory 
void viewStock(Product inventory[], int size);
void addNewProduct(Product inventory[], int *size);
void updateStock(Product inventory[], int size);
void deleteProduct(Product inventory[], int *size);
void searchProduct(Product inventory[], int size);
void checkStockAlerts(Product inventory[], int size);

// Function prototypes for Sales Management
void recordSale(Product inventory[], int *inventorySize, Sale sales[], int *salesCount, Customer customers[], int customerCount);
void generateBill(Sale sale);
void applyDiscount(float *totalAmount);
void dailySalesReport(Sale sales[], int size);
void monthlySalesReport(Sale sales[], int size);
void saveSales(Sale sales[], int size);
void loadSales(Sale sales[], int *size);
int findCustomer(Customer customers[], int size);

// Function prototypes for Purchase Management
void addPurchase(Product inventory[], int *inventorySize, Purchase purchases[], int *purchaseCount, Supplier suppliers[], int *supplierCount);
void supplierManagement(Supplier suppliers[], int *supplierCount);
void purchaseReports(Purchase purchases[], int size);
void savePurchases(Purchase purchases[], int size);
void loadPurchases(Purchase purchases[], int *size);
void saveSuppliers(Supplier suppliers[], int size);
void loadSuppliers(Supplier suppliers[], int *size);
int findSupplier(Supplier suppliers[], int size);
void addCustomerIfNotPresent(Customer customers[], int *customerCount);

// Function prototypes for Customer Management
void addCustomer(Customer customers[], int *customerCount);
void saveCustomers(Customer customers[], int size);
void loadCustomers(Customer customers[], int *size);
int findCustomerIndex(Customer customers[], int size, char customerID[10]);
void updateLoyaltyPoints(Customer customers[], int customerIndex, float amountSpent);
void viewCustomerDetails(Customer customers[], int customerCount);
void applyLoyaltyDiscount(Customer customers[], int *customerCount, float *totalAmount);

// Function prototypes for Employee Management
void addEmployee(Employee employees[], int *employeeCount);
void saveEmployees(Employee employees[], int size);
void loadEmployees(Employee employees[], int *size);
int findEmployeeIndex(Employee employees[], int size, char employeeID[10]);
void recordAttendance(Employee employees[], int employeeIndex, int day);
void viewEmployeeDetails(Employee employees[], int employeeCount);
void restrictAccessByRole(Employee employees[], int employeeCount, char currentRole[20]);

// customer.c

// Add a New Customer
void addCustomer(Customer customers[], int *customerCount) {
    if (*customerCount >= MAX_CUSTOMERS) {
        printf("Error: Customer database is full.\n");
        return;
    }

    Customer newCustomer;
    printf("Enter Customer ID: ");
    scanf("%s", newCustomer.customerID);
    printf("Enter Customer Name: ");
    scanf(" %[^\n]s", newCustomer.name);
    printf("Enter Customer Contact Info: ");
    scanf(" %[^\n]s", newCustomer.contactInfo);
    newCustomer.loyaltyPoints = 0;  // Initial loyalty points
    newCustomer.purchaseCount = 0;  // Initial purchase count

    customers[*customerCount] = newCustomer;
    (*customerCount)++;

    saveCustomers(customers, *customerCount);
    printf("Customer added successfully!\n");
}

// Save Customer Data to File
void saveCustomers(Customer customers[], int size) {
    FILE *file = fopen(CUSTOMERS_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save customer data.\n");
        return;
    }

    fprintf(file, "CustomerID,Name,ContactInfo,LoyaltyPoints,PurchaseCount\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%s,%s,%.2f,%d\n",
                customers[i].customerID,
                customers[i].name,
                customers[i].contactInfo,
                customers[i].loyaltyPoints,
                customers[i].purchaseCount);
    }

    fclose(file);
}

// Load Customer Data from File
void loadCustomers(Customer customers[], int *size) {
    FILE *file = fopen(CUSTOMERS_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to load customer data.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    *size = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%f,%d",
               customers[*size].customerID,
               customers[*size].name,
               customers[*size].contactInfo,
               &customers[*size].loyaltyPoints,
               &customers[*size].purchaseCount);

        (*size)++;
    }

    fclose(file);
}

// Find Customer Index by ID
int findCustomerIndex(Customer customers[], int size, char customerID[10]) {
    for (int i = 0; i < size; i++) {
        if (strcmp(customers[i].customerID, customerID) == 0) {
            return i;
        }
    }
    return -1;  // Customer not found
}

// Update Loyalty Points after Purchase
void updateLoyaltyPoints(Customer customers[], int customerIndex, float amountSpent) {
    if (customerIndex >= 0) {
        customers[customerIndex].loyaltyPoints += amountSpent * 0.05;  // 5% of the amount spent as loyalty points
        customers[customerIndex].purchaseCount++;
        saveCustomers(customers, *customerCount);
        printf("Updated loyalty points: %.2f\n", customers[customerIndex].loyaltyPoints);
    }
}

// View Customer Details
void viewCustomerDetails(Customer customers[], int customerCount) {
    char customerID[10];
    printf("Enter Customer ID to view details: ");
    scanf("%s", customerID);

    int customerIndex = findCustomerIndex(customers, customerCount, customerID);
    if (customerIndex == -1) {
        printf("Error: Customer not found.\n");
        return;
    }

    printf("Customer Details:\n");
    printf("ID: %s\n", customers[customerIndex].customerID);
    printf("Name: %s\n", customers[customerIndex].name);
    printf("Contact Info: %s\n", customers[customerIndex].contactInfo);
    printf("Loyalty Points: %.2f\n", customers[customerIndex].loyaltyPoints);
    printf("Number of Purchases: %d\n", customers[customerIndex].purchaseCount);
}

// Apply Loyalty Discount (if applicable)
void applyLoyaltyDiscount(Customer customers[], int *customerCount, float *totalAmount) {
    char customerID[10];
    printf("Enter Customer ID to apply loyalty discount: ");
    scanf("%s", customerID);

    int customerIndex = findCustomerIndex(customers, *customerCount, customerID);
    if (customerIndex == -1) {
        printf("Error: Customer not found.\n");
        return;
    }

    if (customers[customerIndex].loyaltyPoints >= 100) {
        float discount = customers[customerIndex].loyaltyPoints / 100;  // 1 point = 1 unit discount
        *totalAmount -= discount;
        printf("Loyalty discount applied: %.2f\n", discount);
    } else {
        printf("Not enough loyalty points to apply a discount.\n");
    }
}


//employee.c

// Add a New Employee
void addEmployee(Employee employees[], int *employeeCount) {
    if (*employeeCount >= MAX_EMPLOYEES) {
        printf("Error: Employee database is full.\n");
        return;
    }

    Employee newEmployee;
    printf("Enter Employee ID: ");
    scanf("%s", newEmployee.employeeID);
    printf("Enter Employee Name: ");
    scanf(" %[^\n]s", newEmployee.name);
    printf("Enter Employee Role (admin, cashier, manager): ");
    scanf("%s", newEmployee.role);
    printf("Enter Employee Contact Info: ");
    scanf(" %[^\n]s", newEmployee.contactInfo);
    
    // Initialize attendance for 30 days
    for (int i = 0; i < 30; i++) {
        newEmployee.attendance[i] = 0;  // Mark as absent initially
    }
    newEmployee.attendanceCount = 0;

    employees[*employeeCount] = newEmployee;
    (*employeeCount)++;

    saveEmployees(employees, *employeeCount);
    printf("Employee added successfully!\n");
}

// Save Employee Data to File
void saveEmployees(Employee employees[], int size) {
    FILE *file = fopen(EMPLOYEES_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save employee data.\n");
        return;
    }

    fprintf(file, "EmployeeID,Name,Role,ContactInfo,AttendanceCount\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%s,%s,%s,%d\n",
                employees[i].employeeID,
                employees[i].name,
                employees[i].role,
                employees[i].contactInfo,
                employees[i].attendanceCount);
    }

    fclose(file);
}

// Load Employee Data from File
void loadEmployees(Employee employees[], int *size) {
    FILE *file = fopen(EMPLOYEES_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to load employee data.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    *size = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%d",
               employees[*size].employeeID,
               employees[*size].name,
               employees[*size].role,
               employees[*size].contactInfo,
               &employees[*size].attendanceCount);

        (*size)++;
    }

    fclose(file);
}

// Find Employee Index by ID
int findEmployeeIndex(Employee employees[], int size, char employeeID[10]) {
    for (int i = 0; i < size; i++) {
        if (strcmp(employees[i].employeeID, employeeID) == 0) {
            return i;
        }
    }
    return -1;  // Employee not found
}

// Record Attendance for Employee
void recordAttendance(Employee employees[], int employeeIndex, int day) {
    if (employeeIndex >= 0 && day >= 1 && day <= 30) {
        employees[employeeIndex].attendance[day - 1] = 1; // Mark attendance as present
        employees[employeeIndex].attendanceCount++;
        saveEmployees(employees, *employeeCount);
        printf("Attendance for Employee ID %s marked as present for day %d.\n", employees[employeeIndex].employeeID, day);
    } else {
        printf("Invalid day or employee index.\n");
    }
}

// View Employee Details
void viewEmployeeDetails(Employee employees[], int employeeCount) {
    char employeeID[10];
    printf("Enter Employee ID to view details: ");
    scanf("%s", employeeID);

    int employeeIndex = findEmployeeIndex(employees, employeeCount, employeeID);
    if (employeeIndex == -1) {
        printf("Error: Employee not found.\n");
        return;
    }

    printf("Employee Details:\n");
    printf("ID: %s\n", employees[employeeIndex].employeeID);
    printf("Name: %s\n", employees[employeeIndex].name);
    printf("Role: %s\n", employees[employeeIndex].role);
    printf("Contact Info: %s\n", employees[employeeIndex].contactInfo);
    printf("Attendance Count: %d\n", employees[employeeIndex].attendanceCount);

    printf("Attendance for the past 30 days:\n");
    for (int i = 0; i < 30; i++) {
        printf("Day %d: %s\n", i + 1, employees[employeeIndex].attendance[i] == 1 ? "Present" : "Absent");
    }
}

// Restrict Access Based on Role
void restrictAccessByRole(Employee employees[], int employeeCount, char currentRole[20]) {
    if (strcmp(currentRole, ADMIN) == 0) {
        printf("Admin has full access to all features.\n");
        // Allow access to all functions
    } else if (strcmp(currentRole, MANAGER) == 0) {
        printf("Manager has restricted access. Cannot access employee management features.\n");
        // Restrict access to employee management
    } else if (strcmp(currentRole, CASHIER) == 0) {
        printf("Cashier has limited access. Cannot access sales management or reports.\n");
        // Restrict access to sales and reporting features
    } else {
        printf("Unknown role. Access denied.\n");
    }
}

//inventory.c

// View Inventory Stock Levels
void viewStock(Product inventory[], int size) {
    printf("=== Current Inventory Stock ===\n");
    printf("ID\tName\t\tCategory\tSupplier\tQty\tCost Price\tSell Price\tExpiry\n");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++) {
        printf("%-8s%-16s%-16s%-16s%-8d%-14.2f%-14.2f%s\n",
               inventory[i].productID,
               inventory[i].productName,
               inventory[i].category,
               inventory[i].supplier,
               inventory[i].quantity,
               inventory[i].costPrice,
               inventory[i].sellingPrice,
               inventory[i].expiryDate);
    }

    printf("----------------------------------------------------------------------------------------\n");
}

// Add New Product to Inventory
void addNewProduct(Product inventory[], int *size) {
    if (*size >= MAX_PRODUCTS) {
        printf("Error: Inventory is full, cannot add more products.\n");
        return;
    }

    Product newProduct;
    printf("Enter Product Details:\n");
    printf("Product ID: ");
    scanf("%s", newProduct.productID);
    printf("Product Name: ");
    scanf(" %[^\n]s", newProduct.productName); // to read full name with spaces
    printf("Category: ");
    scanf("%s", newProduct.category);
    printf("Supplier: ");
    scanf("%s", newProduct.supplier);
    printf("Quantity: ");
    scanf("%d", &newProduct.quantity);
    printf("Cost Price: ");
    scanf("%f", &newProduct.costPrice);
    printf("Selling Price: ");
    scanf("%f", &newProduct.sellingPrice);
    printf("Expiry Date (DD/MM/YYYY or 'N/A' if not applicable): ");
    scanf("%s", newProduct.expiryDate);

    inventory[*size] = newProduct;
    (*size)++;

    saveInventory(inventory, *size);
    printf("Product added successfully!\n");
}

// Update Stock Levels or Details
void updateStock(Product inventory[], int size) {
    char productID[10];
    printf("Enter Product ID to update: ");
    scanf("%s", productID);

    int index = findProductIndex(inventory, size, productID);
    if (index == -1) {
        printf("Error: Product with ID %s not found.\n", productID);
        return;
    }

    printf("Updating details for product: %s\n", inventory[index].productName);
    printf("Enter new quantity (current: %d): ", inventory[index].quantity);
    scanf("%d", &inventory[index].quantity);
    printf("Enter new cost price (current: %.2f): ", inventory[index].costPrice);
    scanf("%f", &inventory[index].costPrice);
    printf("Enter new selling price (current: %.2f): ", inventory[index].sellingPrice);
    scanf("%f", &inventory[index].sellingPrice);
    printf("Enter new expiry date (current: %s): ", inventory[index].expiryDate);
    scanf("%s", inventory[index].expiryDate);

    saveInventory(inventory, size);
    printf("Product details updated successfully!\n");
}

// Delete Product from Inventory
void deleteProduct(Product inventory[], int *size) {
    char productID[10];
    printf("Enter Product ID to delete: ");
    scanf("%s", productID);

    int index = findProductIndex(inventory, *size, productID);
    if (index == -1) {
        printf("Error: Product with ID %s not found.\n", productID);
        return;
    }

    for (int i = index; i < *size - 1; i++) {
        inventory[i] = inventory[i + 1];
    }

    (*size)--;
    saveInventory(inventory, *size);
    printf("Product deleted successfully!\n");
}

// Search for Products by Name, Category, or ID
void searchProduct(Product inventory[], int size) {
    char searchTerm[MAX_NAME];
    printf("Enter product name, category, or ID to search: ");
    scanf(" %[^\n]s", searchTerm);

    printf("=== Search Results ===\n");
    printf("ID\tName\t\tCategory\tSupplier\tQty\tCost Price\tSell Price\tExpiry\n");
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++) {
        if (strstr(inventory[i].productName, searchTerm) != NULL ||
            strstr(inventory[i].category, searchTerm) != NULL ||
            strstr(inventory[i].productID, searchTerm) != NULL) {
            printf("%-8s%-16s%-16s%-16s%-8d%-14.2f%-14.2f%s\n",
                   inventory[i].productID,
                   inventory[i].productName,
                   inventory[i].category,
                   inventory[i].supplier,
                   inventory[i].quantity,
                   inventory[i].costPrice,
                   inventory[i].sellingPrice,
                   inventory[i].expiryDate);
        }
    }

    printf("----------------------------------------------------------------------------------------\n");
}

// Check for Low Stock or Expiry Alerts
void checkStockAlerts(Product inventory[], int size) {
    printf("=== Stock Alerts ===\n");
    int alertFound = 0;

    for (int i = 0; i < size; i++) {
        // Check for low stock
        if (inventory[i].quantity < MIN_STOCK_THRESHOLD) {
            printf("Low stock alert: %s (ID: %s) - Quantity: %d\n", 
                   inventory[i].productName, 
                   inventory[i].productID, 
                   inventory[i].quantity);
            alertFound = 1;
        }

        // Check for nearing expiry (if applicable)
        if (strcmp(inventory[i].expiryDate, "N/A") != 0) {
            struct tm expiryTime = {0};
            strptime(inventory[i].expiryDate, "%d/%m/%Y", &expiryTime);
            time_t expiry = mktime(&expiryTime);
            time_t now = time(NULL);
            double daysToExpiry = difftime(expiry, now) / (60 * 60 * 24);

            if (daysToExpiry <= 30) { // If expiry is within 30 days
                printf("Expiry alert: %s (ID: %s) - Expiry Date: %s\n", 
                       inventory[i].productName, 
                       inventory[i].productID, 
                       inventory[i].expiryDate);
                alertFound = 1;
            }
        }
    }

    if (!alertFound) {
        printf("No stock alerts at this time.\n");
    }
}

//purchase.c

// Add a Purchase
void addPurchase(Product inventory[], int *inventorySize, Purchase purchases[], int *purchaseCount, Supplier suppliers[], int *supplierCount) {
    Purchase newPurchase;
    
    // Check or add customer details before proceeding
    addCustomerIfNotPresent(customers, &customerCount);

    // Ask for supplier details
    int supplierIndex = findSupplier(suppliers, *supplierCount);
    if (supplierIndex == -1) {
        printf("Supplier not found. Adding a new supplier.\n");
        supplierManagement(suppliers, supplierCount);
        supplierIndex = *supplierCount - 1; // Assume the new supplier was added at the end
    }

    // Ask for product details for the purchase
    char productID[10];
    int quantity;
    printf("Enter Product ID for purchase: ");
    scanf("%s", productID);

    int productIndex = findProductIndex(inventory, *inventorySize, productID);
    if (productIndex == -1) {
        printf("Error: Product with ID %s not found in inventory.\n", productID);
        return;
    }

    printf("Enter quantity to purchase: ");
    scanf("%d", &quantity);

    // Record the purchase details
    strcpy(newPurchase.productID, inventory[productIndex].productID);
    strcpy(newPurchase.supplierID, suppliers[supplierIndex].supplierID);
    newPurchase.quantityPurchased = quantity;
    newPurchase.totalCost = quantity * inventory[productIndex].costPrice;

    // Get the current timestamp
    getCurrentTime(newPurchase.timeStamp);

    // Update inventory after purchase
    inventory[productIndex].quantity += quantity;
    saveInventory(inventory, *inventorySize);

    // Save the purchase to purchase records
    purchases[*purchaseCount] = newPurchase;
    (*purchaseCount)++;
    savePurchases(purchases, *purchaseCount);

    printf("Purchase recorded successfully!\n");
}

// Supplier Management
void supplierManagement(Supplier suppliers[], int *supplierCount) {
    if (*supplierCount >= MAX_SUPPLIERS) {
        printf("Error: Supplier database is full.\n");
        return;
    }

    Supplier newSupplier;
    printf("Enter Supplier Details:\n");
    printf("Supplier ID: ");
    scanf("%s", newSupplier.supplierID);
    printf("Supplier Name: ");
    scanf(" %[^\n]s", newSupplier.supplierName);
    printf("Contact Information: ");
    scanf(" %[^\n]s", newSupplier.contactInfo);
    printf("Address: ");
    scanf(" %[^\n]s", newSupplier.address);

    suppliers[*supplierCount] = newSupplier;
    (*supplierCount)++;

    saveSuppliers(suppliers, *supplierCount);
    printf("Supplier added successfully!\n");
}

// Purchase Reports for Specific Periods
void purchaseReports(Purchase purchases[], int size) {
    char startDate[MAX_DATE], endDate[MAX_DATE];
    struct tm startTm = {0}, endTm = {0};
    time_t startTimestamp, endTimestamp;

    printf("Enter start date (DD/MM/YYYY): ");
    scanf("%s", startDate);
    printf("Enter end date (DD/MM/YYYY): ");
    scanf("%s", endDate);

    // Convert dates to time_t for comparison
    strptime(startDate, "%d/%m/%Y", &startTm);
    startTimestamp = mktime(&startTm);

    strptime(endDate, "%d/%m/%Y", &endTm);
    endTimestamp = mktime(&endTm);

    printf("=== Purchase Report from %s to %s ===\n", startDate, endDate);
    printf("ID\tProduct ID\tSupplier ID\tQty\tCost\tDate & Time\n");
    printf("-----------------------------------------------------------------\n");

    float totalCost = 0.0;
    for (int i = 0; i < size; i++) {
        struct tm purchaseTm = {0};
        strptime(purchases[i].timeStamp, "%d/%m/%Y %H:%M:%S", &purchaseTm);
        time_t purchaseTimestamp = mktime(&purchaseTm);

        if (purchaseTimestamp >= startTimestamp && purchaseTimestamp <= endTimestamp) {
            printf("%-8s%-12s%-12s%-8d%-8.2f%s\n",
                   purchases[i].purchaseID,
                   purchases[i].productID,
                   purchases[i].supplierID,
                   purchases[i].quantityPurchased,
                   purchases[i].totalCost,
                   purchases[i].timeStamp);

            totalCost += purchases[i].totalCost;
        }
    }

    printf("-----------------------------------------------------------------\n");
    printf("Total Cost for the period: %.2f\n", totalCost);
}

// Save Purchases to File
void savePurchases(Purchase purchases[], int size) {
    FILE *file = fopen(PURCHASES_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save purchases data.\n");
        return;
    }

    fprintf(file, "PurchaseID,ProductID,SupplierID,QuantityPurchased,TotalCost,TimeStamp\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%s,%s,%d,%.2f,%s\n",
                purchases[i].purchaseID,
                purchases[i].productID,
                purchases[i].supplierID,
                purchases[i].quantityPurchased,
                purchases[i].totalCost,
                purchases[i].timeStamp);
    }

    fclose(file);
}

// Load Purchases from File
void loadPurchases(Purchase purchases[], int *size) {
    FILE *file = fopen(PURCHASES_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to load purchases data.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    *size = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%f,%[^\n]",
               purchases[*size].purchaseID,
               purchases[*size].productID,
               purchases[*size].supplierID,
               &purchases[*size].quantityPurchased,
               &purchases[*size].totalCost,
               purchases[*size].timeStamp);

        (*size)++;
    }

    fclose(file);
}

// Save Suppliers to File
void saveSuppliers(Supplier suppliers[], int size) {
    FILE *file = fopen(SUPPLIERS_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save suppliers data.\n");
        return;
    }

    fprintf(file, "SupplierID,SupplierName,ContactInfo,Address\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%s,%s,%s\n",
                suppliers[i].supplierID,
                suppliers[i].supplierName,
                suppliers[i].contactInfo,
                suppliers[i].address);
    }

    fclose(file);
}

// Load Suppliers from File
void loadSuppliers(Supplier suppliers[], int *size) {
    FILE *file = fopen(SUPPLIERS_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to load suppliers data.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    *size = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%[^\n]",
               suppliers[*size].supplierID,
               suppliers[*size].supplierName,
               suppliers[*size].contactInfo,
               suppliers[*size].address);

        (*size)++;
    }

    fclose(file);
}

// Add Customer if Not Already Present
void addCustomerIfNotPresent(Customer customers[], int *customerCount) {
    char customerID[10];
    printf("Enter Customer ID for the purchase: ");
    scanf("%s", customerID);

    int customerExists = 0;
    for (int i = 0; i < *customerCount; i++) {
        if (strcmp(customers[i].customerID, customerID) == 0) {
            customerExists = 1;
            break;
        }
    }

    if (!customerExists) {
        printf("Customer not found. Adding a new customer.\n");
        if (*customerCount < MAX_CUSTOMERS) {
            printf("Enter Customer Name: ");
            scanf(" %[^\n]s", customers[*customerCount].name);
            strcpy(customers[*customerCount].customerID, customerID);
            (*customerCount)++;

            saveCustomers(customers, *customerCount);
        } else {
            printf("Error: Customer database is full.\n");
        }
    }
}

//sales.c


// Record a Sales Transaction
void recordSale(Product inventory[], int *inventorySize, Sale sales[], int *salesCount, Customer customers[], int customerCount) {
    Sale newSale;
    int customerIndex = findCustomer(customers, customerCount);
    
    if (customerIndex == -1) {
        printf("Error: Customer not found.\n");
        return;
    }

    // Ask for product details
    char productID[10];
    int quantity;
    printf("Enter Product ID for sale: ");
    scanf("%s", productID);

    int productIndex = findProductIndex(inventory, *inventorySize, productID);
    if (productIndex == -1) {
        printf("Error: Product with ID %s not found in inventory.\n", productID);
        return;
    }

    printf("Enter quantity to sell (Available: %d): ", inventory[productIndex].quantity);
    scanf("%d", &quantity);

    if (quantity > inventory[productIndex].quantity) {
        printf("Error: Not enough stock available for this sale.\n");
        return;
    }

    // Record the sale details
    strcpy(newSale.productID, inventory[productIndex].productID);
    strcpy(newSale.customerID, customers[customerIndex].customerID);
    newSale.quantitySold = quantity;
    newSale.totalAmount = quantity * inventory[productIndex].sellingPrice;
    strcpy(newSale.paymentMode, ""); // Payment handled separately

    // Apply any discounts
    applyDiscount(&newSale.totalAmount);

    // Update inventory after sale
    inventory[productIndex].quantity -= quantity;
    saveInventory(inventory, *inventorySize);

    // Get the current timestamp
    getCurrentTime(newSale.timeStamp);

    // Save sale to sales records
    sales[*salesCount] = newSale;
    (*salesCount)++;
    saveSales(sales, *salesCount);

    // Generate a bill/receipt for the sale
    generateBill(newSale);

    printf("Sale recorded successfully!\n");
}

// Generate a Bill/Receipt for the Sale
void generateBill(Sale sale) {
    printf("\n=== Receipt ===\n");
    printf("Sale ID: %s\n", sale.saleID);
    printf("Product ID: %s\n", sale.productID);
    printf("Customer ID: %s\n", sale.customerID);
    printf("Quantity Sold: %d\n", sale.quantitySold);
    printf("Total Amount: %.2f\n", sale.totalAmount);
    printf("Payment Mode: %s\n", sale.paymentMode);
    printf("Date & Time: %s\n", sale.timeStamp);
    printf("================\n");
}

// Apply Discounts to Sales
void applyDiscount(float *totalAmount) {
    int applyDiscount;
    printf("Would you like to apply a discount? (1 for Yes, 0 for No): ");
    scanf("%d", &applyDiscount);

    if (applyDiscount) {
        float discountPercentage;
        printf("Enter discount percentage (0 - %d): ", MAX_DISCOUNT);
        scanf("%f", &discountPercentage);

        if (discountPercentage < 0 || discountPercentage > MAX_DISCOUNT) {
            printf("Invalid discount percentage. No discount applied.\n");
        } else {
            *totalAmount -= (*totalAmount * (discountPercentage / 100));
            printf("Discount applied. New total: %.2f\n", *totalAmount);
        }
    }
}

// Generate Daily Sales Report
void dailySalesReport(Sale sales[], int size) {
    char date[MAX_DATE];
    printf("Enter the date (DD/MM/YYYY) for the daily report: ");
    scanf("%s", date);

    float dailyTotal = 0.0;
    printf("=== Daily Sales Report for %s ===\n", date);
    printf("ID\tProduct ID\tCustomer ID\tQty\tAmount\tDate & Time\n");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++) {
        if (strstr(sales[i].timeStamp, date) != NULL) {
            printf("%-8s%-12s%-12s%-8d%-8.2f%s\n",
                   sales[i].saleID,
                   sales[i].productID,
                   sales[i].customerID,
                   sales[i].quantitySold,
                   sales[i].totalAmount,
                   sales[i].timeStamp);

            dailyTotal += sales[i].totalAmount;
        }
    }

    printf("------------------------------------------------------------------\n");
    printf("Total Sales for %s: %.2f\n", date, dailyTotal);
}

// Generate Monthly Sales Report
void monthlySalesReport(Sale sales[], int size) {
    char monthYear[8]; // Format: MM/YYYY
    printf("Enter the month and year (MM/YYYY) for the monthly report: ");
    scanf("%s", monthYear);

    float monthlyTotal = 0.0;
    printf("=== Monthly Sales Report for %s ===\n", monthYear);
    printf("ID\tProduct ID\tCustomer ID\tQty\tAmount\tDate & Time\n");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < size; i++) {
        if (strstr(sales[i].timeStamp + 3, monthYear) != NULL) { // Check month/year in timestamp
            printf("%-8s%-12s%-12s%-8d%-8.2f%s\n",
                   sales[i].saleID,
                   sales[i].productID,
                   sales[i].customerID,
                   sales[i].quantitySold,
                   sales[i].totalAmount,
                   sales[i].timeStamp);

            monthlyTotal += sales[i].totalAmount;
        }
    }

    printf("------------------------------------------------------------------\n");
    printf("Total Sales for %s: %.2f\n", monthYear, monthlyTotal);
}

// Save Sales Records to File
void saveSales(Sale sales[], int size) {
    FILE *file = fopen(SALES_FILE, "w");
    if (file == NULL) {
        printf("Error: Unable to save sales data.\n");
        return;
    }

    fprintf(file, "SaleID,ProductID,CustomerID,QuantitySold,TotalAmount,PaymentMode,TimeStamp\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%s,%s,%d,%.2f,%s,%s\n",
                sales[i].saleID,
                sales[i].productID,
                sales[i].customerID,
                sales[i].quantitySold,
                sales[i].totalAmount,
                sales[i].paymentMode,
                sales[i].timeStamp);
    }

    fclose(file);
}

// Load Sales Records from File
void loadSales(Sale sales[], int *size) {
    FILE *file = fopen(SALES_FILE, "r");
    if (file == NULL) {
        printf("Error: Unable to load sales data.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file); // Skip header
    *size = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%[^,],%d,%f,%[^,],%[^\n]",
               sales[*size].saleID,
               sales[*size].productID,
               sales[*size].customerID,
               &sales[*size].quantitySold,
               &sales[*size].totalAmount,
               sales[*size].paymentMode,
               sales[*size].timeStamp);

        (*size)++;
    }

    fclose(file);
}

// Helper function to find customer
int findCustomer(Customer customers[], int size) {
    char customerID[10];
    printf("Enter Customer ID: ");
    scanf("%s", customerID);

    for (int i = 0; i < size; i++) {
        if (strcmp(customers[i].customerID, customerID) == 0) {
            return i;
        }
    }
    return -1;
}

//userfile.c

#define MAX_USERS 5 // Number of predefined users for login

// Predefined users for login (For simplicity, users are hardcoded)
User predefinedUsers[MAX_USERS] = {
    {"admin", "password123"},
    {"manager", "manager321"},
    {"cashier1", "cashierpass1"},
    {"cashier2", "cashierpass2"},
    {"stock", "stockpass"}
};

// Function Implementations

// Login system
void login() {
    char inputUserID[MAX_USERID], inputPassword[MAX_PASSWORD];
    int attempts = 3, loginSuccess = 0;

    printf("=== Store Management System Login ===\n");

    while (attempts > 0 && !loginSuccess) {
        printf("User ID: ");
        scanf("%s", inputUserID);
        printf("Password: ");
        scanf("%s", inputPassword);

        // Check user credentials
        for (int i = 0; i < MAX_USERS; i++) {
            if (strcmp(inputUserID, predefinedUsers[i].userID) == 0 &&
                strcmp(inputPassword, predefinedUsers[i].password) == 0) {
                printf("\nLogin successful. Welcome, %s!\n\n", inputUserID);
                loginSuccess = 1;
                break;
            }
        }

        if (!loginSuccess) {
            attempts--;
            printf("Invalid UserID or Password. Attempts remaining: %d\n", attempts);
        }
    }

    if (!loginSuccess) {
        printf("Maximum login attempts exceeded. Exiting program.\n");
        exit(1);
    }
}

// File Initialization
void initializeFiles() {
    FILE *file;

    // Check if Inventory file exists, if not create it
    file = fopen(INVENTORY_FILE, "r");
    if (file == NULL) {
        file = fopen(INVENTORY_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to create inventory file.\n");
            exit(1);
        }
        fprintf(file, "ProductID,ProductName,Quantity,Price\n");
        printf("Inventory file created successfully.\n");
    }
    fclose(file);

    // Check if Sales file exists, if not create it
    file = fopen(SALES_FILE, "r");
    if (file == NULL) {
        file = fopen(SALES_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to create sales file.\n");
            exit(1);
        }
        fprintf(file, "SaleID,ProductID,CustomerID,QuantitySold,TotalAmount,PaymentMode,TimeStamp\n");
        printf("Sales file created successfully.\n");
    }
    fclose(file);

    // Check if Purchases file exists, if not create it
    file = fopen(PURCHASE_FILE, "r");
    if (file == NULL) {
        file = fopen(PURCHASE_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to create purchases file.\n");
            exit(1);
        }
        fprintf(file, "PurchaseID,ProductID,QuantityPurchased,TotalAmount,SupplierName,TimeStamp\n");
        printf("Purchases file created successfully.\n");
    }
    fclose(file);

    // Check if Employees file exists, if not create it
    file = fopen(EMPLOYEES_FILE, "r");
    if (file == NULL) {
        file = fopen(EMPLOYEES_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to create employees file.\n");
            exit(1);
        }
        fprintf(file, "EmployeeID,Name,Position,Attendance\n");
        printf("Employees file created successfully.\n");
    }
    fclose(file);

    // Check if Customers file exists, if not create it
    file = fopen(CUSTOMERS_FILE, "r");
    if (file == NULL) {
        file = fopen(CUSTOMERS_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to create customers file.\n");
            exit(1);
        }
        fprintf(file, "CustomerID,Name,Contact\n");
        printf("Customers file created successfully.\n");
    }
    fclose(file);
}

int main() {
    // Initial setup
    login();             // User must log in before accessing the system
    initializeFiles();   // Ensure all necessary files are set up
    
    // The rest of the program will go here, e.g., main menu and other functionalities
    printf("System initialized successfully. Ready for operation.\n");

    // Placeholder for main program operations
    // ...
    
    return 0;
