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

typedef struct {
    char employeeID[10];
    char name[MAX_NAME];
    char position[MAX_NAME];
    int attendance; // 0 for absent, 1 for present
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
