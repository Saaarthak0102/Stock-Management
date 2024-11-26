// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>





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
