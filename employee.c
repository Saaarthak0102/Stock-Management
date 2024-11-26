// store_management.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants for Employee Management
#define MAX_EMPLOYEES 100
#define ADMIN "admin"
#define CASHIER "cashier"
#define MANAGER "manager"

// Structure for Employee
typedef struct {
    char employeeID[10];
    char name[MAX_NAME];
    char role[20];  // Role like "admin", "cashier", "manager"
    char contactInfo[50];
    int attendance[30];  // Store attendance for 30 days
    int attendanceCount; // Number of days attended in the month
} Employee;

// Function prototypes for Employee Management
void addEmployee(Employee employees[], int *employeeCount);
void saveEmployees(Employee employees[], int size);
void loadEmployees(Employee employees[], int *size);
int findEmployeeIndex(Employee employees[], int size, char employeeID[10]);
void recordAttendance(Employee employees[], int employeeIndex, int day);
void viewEmployeeDetails(Employee employees[], int employeeCount);
void restrictAccessByRole(Employee employees[], int employeeCount, char currentRole[20]);

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
