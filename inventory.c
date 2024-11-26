// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
