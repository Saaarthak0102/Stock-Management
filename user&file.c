// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
