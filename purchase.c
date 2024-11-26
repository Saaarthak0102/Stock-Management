// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants for Purchase
#define MAX_SUPPLIERS 100
#define MAX_PURCHASES 1000

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
