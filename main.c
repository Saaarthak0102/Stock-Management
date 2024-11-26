// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    // Load necessary data from files
    int customerCount = 0, employeeCount = 0, productCount = 0, saleCount = 0, purchaseCount = 0;

    loadCustomers(customers, &customerCount);
    loadEmployees(employees, &employeeCount);
    loadProducts(products, &productCount);

    // Log in system
    login();

    int choice;
    char role[20];
    printf("Enter your role (admin/cashier/manager): ");
    scanf("%s", role);

    // Main loop for menu
    while (1) {
        printf("\nStore Management System\n");
        printf("1. Inventory Management\n");
        printf("2. Sales Management\n");
        printf("3. Purchase Management\n");
        printf("4. Customer Management\n");
        printf("5. Employee Management\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Inventory Management
                if (strcmp(role, "admin") == 0 || strcmp(role, "manager") == 0) {
                    printf("Inventory Management\n");
                    printf("1. View Stock\n");
                    printf("2. Add New Product\n");
                    printf("3. Update Stock\n");
                    printf("4. Delete Product\n");
                    printf("5. Search Product\n");
                    printf("6. Go Back\n");
                    int inventoryChoice;
                    scanf("%d", &inventoryChoice);

                    switch (inventoryChoice) {
                        case 1:
                            viewStock(products, productCount);
                            break;
                        case 2:
                            addProduct(products, &productCount);
                            break;
                        case 3:
                            // Update stock functionality (not implemented here, placeholder)
                            break;
                        case 4:
                            // Delete product functionality (not implemented here, placeholder)
                            break;
                        case 5:
                            // Search product functionality (not implemented here, placeholder)
                            break;
                        case 6:
                            break;
                        default:
                            printf("Invalid option.\n");
                            break;
                    }
                } else {
                    printf("You do not have permission to access Inventory Management.\n");
                }
                break;

            case 2:
                // Sales Management
                if (strcmp(role, "admin") == 0 || strcmp(role, "cashier") == 0) {
                    printf("Sales Management\n");
                    recordSale(sales, &saleCount, products, productCount);
                } else {
                    printf("You do not have permission to access Sales Management.\n");
                }
                break;

            case 3:
                // Purchase Management
                if (strcmp(role, "admin") == 0 || strcmp(role, "manager") == 0) {
                    printf("Purchase Management\n");
                    recordPurchase(purchases, &purchaseCount, products, &productCount);
                } else {
                    printf("You do not have permission to access Purchase Management.\n");
                }
                break;

            case 4:
                // Customer Management
                if (strcmp(role, "admin") == 0 || strcmp(role, "manager") == 0) {
                    printf("Customer Management\n");
                    addCustomer(customers, &customerCount);
                } else {
                    printf("You do not have permission to access Customer Management.\n");
                }
                break;

            case 5:
                // Employee Management
                if (strcmp(role, "admin") == 0) {
                    printf("Employee Management\n");
                    addEmployee(employees, &employeeCount);
                } else {
                    printf("You do not have permission to access Employee Management.\n");
                }
                break;

            case 6:
                // Exit
                saveEmployees(employees, employeeCount);
                saveCustomers(customers, customerCount);
                saveProducts(products, productCount);
                savePurchases(purchases, purchaseCount);
                saveSales(sales, saleCount);
                printf("Exiting program...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
