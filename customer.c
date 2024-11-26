// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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
