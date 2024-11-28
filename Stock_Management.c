// Including necessary libraries for input/output, console handling, string manipulation, character handling, memory allocation, Windows API functions, and time management.
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

// Defining constants for special keys like ENTER, BACKSPACE, SPACE, and TAB.
#define ENTER 13
#define BKSP 8
#define SPACE 32
#define TAB 9

 // Sets the text color in the console based on the provided foreground color ID.
void setcolor(int ForgC) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (ForgC < 0 || ForgC > 15)  // Ensure valid color ID (0-15 for foreground)
        return;

    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

// Resets the console text color to the default (white on black).
void resetcolor() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, 7); // Reset to default (white on black)
}

// Defining a structure to store product details like product name, product company, product ID, product price, and product quantity.
struct item
{
	char productname[40],productcomp[40],c;
	int productid;
	int price;
	int Qnt;
}st;


// Function declarations for handling console operations, user interface, product management, and cursor positioning.
void wel_come(void);
void title(void);
void login();
void menu(void);
void title(void);
void deleteproduct(void);
void gotoxy(short x, short y)
{
	COORD pos ={x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

void add_item();
void read_item();
void search_item();
void edit_item();
void purchase_item();
void view_activity_log();
void log_activity(const char *message);
void log_purchase(const char *message);
void update_stock();

void log_add_item(const struct item *myItem) { // Log Add Item Function Declaration
    char mess[200];
    sprintf(mess, "Added item: %s, ID: %d, Price: %d, Quantity: %d", myItem->productname, myItem->productid, myItem->price, myItem->Qnt);
    log_activity(mess);
}

void log_delete_item(const struct item *myItem) { // Log Delete Item Function Declaration
    char mess[200];
    sprintf(mess, "Deleted item: %s, ID: %d, Price: %d, Quantity: %d", myItem->productname, myItem->productid, myItem->price, myItem->Qnt);
    log_activity(mess);
}

// Log Activity Function Implementation
void log_activity(const char *message) {
    FILE *log_file;
    log_file = fopen("activity_log.txt", "a");
    if (log_file == NULL) {
        setcolor(4);
        printf("Error: Could not log activity.\n");
        resetcolor();
        return;
    }

    time_t t;
    time(&t);
    fprintf(log_file, "\n%-30s | %-50s\n", ctime(&t), message);
    fclose(log_file);
}

// View Activity Log Function Implementation
void view_activity_log() {
    FILE *log_file;
    char log[200];

    log_file = fopen("activity_log.txt", "r");
    if (log_file == NULL) {
        setcolor(4);
        printf("Error: No activity log available.\n");
        resetcolor();
        getch();
        menu();
        return;
    }

    setcolor(1);
    printf("\n=========== Activity Log ===========\n");
    resetcolor();
    while (fgets(log, sizeof(log), log_file) != NULL) {

        printf("%s", log);
    }

    fclose(log_file);
    printf("\nPress any key to go to Main Menu!");
    getch();
    menu();
}



// Log Purchase Function Implementation
void log_purchase(const char *message) {
    FILE *log_file;
    log_file = fopen("purchase_log.txt", "a");
    if (log_file == NULL) {
        setcolor(4);
        printf("Error: Could not log purchase.\n");
        resetcolor(); 
        return;
    }

    time_t t;
    time(&t);
    fprintf(log_file, "\n%-30s | %-50s\n", ctime(&t), message);
    fclose(log_file);
}

void main(void)
{
wel_come(); //call for welcome screen function
login(); //call for login function
}



void wel_come(void) {
    time_t t;
    time(&t);

    printf("\n\n");
    printf("=========================================================================================================\n");
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t%s\t\t\t\t\t|\n", ctime(&t));
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("=========================================================================================================\n");
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t==========================================\t\t\t\t|\n");
    setcolor(1); // Set text color to blue
    printf("|\t\t\t\t|\t   WELCOME TO THE\t\t |\t\t\t\t|\n");
    printf("|\t\t\t\t|\tSTOCK MANAGEMENT SYSTEM\t\t |\t\t\t\t|\n");
    resetcolor(); // Reset to default color
    printf("|\t\t\t\t==========================================\t\t\t\t|\n");
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    setcolor(4); 
    printf("|\t\t\t\t\t    Project By:\t\t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t Sarthak Sabharwal \t\t\t\t\t\t|\n");
    printf("|\t\t\t\t\t Pranjal Srivastava \t\t\t\t\t\t|\n");
    resetcolor();
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    setcolor(10);
    printf("|\t\t\t\t  \"WE BELIEVE IN QUALITY PRODUCTS AND SERVICES!\"\t\t\t|\n");
    resetcolor();
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("=========================================================================================================\n");
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    setcolor(10);
    printf("|\t\t\t\t\tDelivering Excellence Since 2000\t\t\t\t|\n");
    resetcolor(10);
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("=========================================================================================================\n");
    printf("|\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("|  Press any key to continue...                                                                         |\n");
    printf("=========================================================================================================\n");

    getch();
    system("cls");
}


void title(void)
{
    printf("\n\n\n\t\t\t\t\tSTOCK MANAGEMENT SYSTEM\n");
    printf("\t\t\t\t\t===========================\n");
}

void login()
{
    int a = 0, i = 0;
    char uname[10], c = ' ';
    char pword[10], ID[10];
    char user[10] = "user";
    char pass[10] = "pass";
    
    do
    {
        setcolor(1);
        printf("\n  ========================  LOGIN   ========================  ");
        resetcolor();
        printf("\n                        USERNAME:-");
        scanf("%s", &uname);  // Get the username input from user
        
        printf("\n                        PASSWORD:-");
        while (i < 10)
        {
            pword[i] = getch();  // Get each character for password input
            c = pword[i];
            if (c == 13)  // If Enter key is pressed, exit password input loop
                break;
            else
                printf("*");  // Print * for each character typed (password masking)
            i++;
        }
        pword[i] = '\0';  // Null terminate the password string

        i = 0;  // Reset the index for the next password attempt
        
        // Check if the username and password match predefined values
        if (strcmp(uname, "user") == 0 && strcmp(pword, "pass") == 0)
        {
            setcolor(2);
            printf("\n=============================================================================");
            printf("\n|         WELCOME TO STOCK MANAGEMENT SYSTEM !!!! LOGIN IS SUCCESSFUL        |");
            printf("\n=============================================================================");
            resetcolor();
            printf("\n\n\n\t\t\t\tPress any key to continue...");
            getch();  // Hold the screen before moving forward
            break;  // Exit the loop if login is successful
        }

        else
        {
            setcolor(4);
            printf("\n        SORRY !!!!  LOGIN IS UNSUCCESSFUL");
            a++;  // Increment the failed login attempt counter
            
            // After a failed attempt, hold the screen for the user to see the message
            printf("\n       You have %d attempt(s) left", 3 - a);
            getch();
            resetcolor();
        }

    }
    while (a <= 2);  // Allow 3 attempts for login

    // If user exceeds the login attempts limit
    if (a > 2)
    {
        printf("\nSorry you have entered the wrong username and password for three times!!!");
        getch();  // Hold the screen before exiting or closing
        exit(0);  // Exit the program   

    }
    
    system("cls");  // Clear the console screen
    menu();  // Call the menu function if login is successful
}


void menu(void)
{
    int choice;
    system("cls");  // Clear the screen before showing the menu
    
    main:  // Label to return to in case of invalid choice
    setcolor(1);
    printf("\n=============================================================================");
    printf("\n|                      Stock Management System                               |");
    printf("\n=============================================================================");
    resetcolor();
    printf("\n\n");
    
    // Display the menu options in a table-like format
    printf("| %-5s | %-30s | \n", "Choice", "Action");
    printf("|-------|---------------------------------| \n");
    printf("| %-5s | %-30s | \n", "1", "Add Items");
    printf("| %-5s | %-30s | \n", "2", "Read Products");
    printf("| %-5s | %-30s | \n", "3", "Search Products");
    printf("| %-5s | %-30s | \n", "4", "Edit Items");
    printf("| %-5s | %-30s | \n", "5", "Update Items");
    printf("| %-5s | %-30s |\n", "6", "Purchase Items");
    printf("| %-5s | %-30s |\n", "7", "Delete Items");
    printf("| %-5s | %-30s |\n", "8", "View Activity Log");
    printf("| %-5s | %-30s |\n", "9", "Exit");
    printf("|-------|---------------------------------|\n");
    
    setcolor(13);
    printf("\n\t\tEnter your choice [1-9]: ");
    scanf("%i", &choice);  // Get user input for the menu choice
    resetcolor();

    system("cls");  // Clear the screen again to proceed to the next operation
    
    // Switch case to execute the corresponding function based on user choice
    switch(choice)
    {
        case 1:
            add_item();  // Call function to add products
            break;
        
        case 2:
            read_item();  // Call function to read items
            break;
        
        case 3:
            search_item();  // Call function to search products
            break;
        
        case 4:
            edit_item();  // Call function to edit items
            break;
        
        case 5:
            update_stock(); // Call function to update stock
            break;
        case 6:
            purchase_item();  // Call function to purchase items
            break;
      
        case 7 :
            deleteproduct(); // Call function to delete products
            break;

        case 8: view_activity_log(); // Call function to view activity log
            break;

        case 9: 
            setcolor(4);
            printf("\n+------------------------------------------+\n");
            printf("|              System Exit                 |\n");
            printf("+------------------------------------------+\n");
            resetcolor();
            exit(0);
            break;

        default:
            setcolor(4);
            printf("\nInvalid Choice! Please try again.\n");  // Handle invalid input
            resetcolor();
            getch();  // Hold the screen so the user can read the error message
            goto main;  // Return to the main menu for a new choice
    }
}



void add_item() {

    int index, valid;
    char c;
    int a = 0;
    FILE *fp;
    
    do {
        system("cls");
        setcolor(1);
        printf("=========== Enter Product Detail ===========\n");
        resetcolor();
        
        int ID;  // For comparing product ID if file is not NULL
        fp = fopen("NextFile.txt", "a+");  // Opening file and creating a new one if not exists

        // Check if file exists and open for appending or reading
        if ((fp = fopen("NextFile.txt", "a+")) != NULL) {
            I:  // Label to go back if product ID already exists
            printf("\n+-------------------------------------------+\n");
            printf("| Product ID : ");
            scanf("%i", &ID);  // Get product ID from the user

            // Read through the file to check if product ID already exists
            while (fscanf(fp, "%s %s %i %i %i", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
                if (ID == st.productid)  // Check if product ID already exists
                {
                    setcolor(4);
                    printf("\n\tTHE PRODUCT ID ALREADY EXISTS.\n");
                    goto I;  // Go back to re-enter product ID
                    resetcolor();
                }
            }
            st.productid = ID;  // Assign the entered product ID to the struct
        } else {  // Runs if the file is empty
            printf("\nProduct ID : ");
            scanf("%i", &st.productid);  // Directly get product ID if file is empty
        }
        
        // Add product name
        do {
            fflush(stdin);
            printf("\n+-------------------------------------------+\n");
            printf("| Product Name : ");
            gets(st.productname);  // Get product name input
            st.productname[0] = toupper(st.productname[0]);  // Capitalize first letter of product name
            
            // Iterate through each character to validate the product name
            for (index = 0; index < strlen(st.productname); ++index) {
                if (isalpha(st.productname[index]))  // Check if character is alphabetic
                    valid = 1;
                else {
                    valid = 0;
                    break;
                }
            }

            // If invalid character is found in name
            if (!valid) {
                printf("\n Name contains invalid characters. Please enter again.\n");
                getch();
            }
        } while (!valid);  // Loop until a valid product name is entered

        // Add product company name
        do {
            fflush(stdin);
            printf("\n+-------------------------------------------+\n");
            printf("| Product Company : ");
            gets(st.productcomp);  // Get product company input
            st.productcomp[0] = toupper(st.productcomp[0]);  // Capitalize first letter of company name

            // Iterate through each character to validate the company name
            for (index = 0; index < strlen(st.productcomp); ++index) {
                if (isalpha(st.productcomp[index]))  // Check if character is alphabetic
                    valid = 1;
                else {
                    valid = 0;
                    break;
                }
            }

            // If invalid character is found in company name
            if (!valid) {
                printf("\n Name contains invalid characters. Please enter again.\n");
                getch();
            }
        } while (!valid);  // Loop until a valid product company name is entered

        // Validate product price
        do {
            printf("\n+-------------------------------------------+\n");
            printf("| Price [10-5000] Rupees : ");
            scanf("%i", &st.price);
            if (st.price < 10 || st.price > 5000) {
                printf("\n\tYou cannot enter a price outside the range [10-5000]. Please re-enter.\n");
            }
        } while (st.price < 10 || st.price > 5000);  // Price should be between 10 and 5000
        
        // Validate product quantity
        do {
            printf("\n+-------------------------------------------+\n");
            printf("| Quantity [1-500] : ");
            scanf("%i", &st.Qnt);
            if (st.Qnt < 1 || st.Qnt > 500) {
                printf("\n\tEnter quantity between 1 and 500 only. Please re-enter.\n");
            }
        } while (st.Qnt < 1 || st.Qnt > 500);  // Quantity should be between 1 and 500
        
        // Write product details to file
        fp = fopen("NextFile.txt", "a");
        fprintf(fp, "%s %s %i %i %i\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
        fclose(fp);
    
        log_add_item(&st); 
        // Prompt user to add another item or return to the main menu
    setcolor(2);
    printf("\n=============================================================================");
    printf("\n|                      Item Added Successfully                              |");
    printf("\n=============================================================================");
    resetcolor();
        printf("\nPress 'Enter' to add another item or any other key to go to the main menu");
        
    } while ((c = getch()) == '\r');  // Continue adding items until a key other than 'Enter' is pressed
    
    menu();  // Return to the main menu
}



void search_item() {
    char target[40];
    int found = 0, choice;
    FILE *sfile;
    int id;

    // Open the file to read product data
    sfile = fopen("NextFile.txt", "r");

    if (sfile == NULL)  // Check if file couldn't be opened
    {
        printf("\nError: Could not open file.\n");
        getch();
        menu();  // Return to the menu if file can't be opened
        return;
    }

    // Prompt user to choose search type: by product name or ID
    system("cls");
    setcolor(1);
    printf("\n=========== Search Product ===========\n");
    printf("+------------------------------------------+\n");
    printf("| 1. Search by Product Name               |\n");
    printf("| 2. Search by Product ID                 |\n");
    printf("+------------------------------------------+\n");
    resetcolor();
    setcolor(13);
    printf("| Enter your choice (1/2): ");
    resetcolor();
    scanf("%d", &choice);

    // Perform search based on user's choice
    if (choice == 1) {
        // Search by product name
        printf("\nEnter product name to search: ");
        fflush(stdin);
        gets(target);  // Read product name to search for
        
        target[0] = toupper(target[0]);  // Capitalize first letter for consistency
        
        // Loop through file to search for product
        while (!feof(sfile) && found == 0)
        {
            fscanf(sfile, "%s %s %i %i %i", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt);
            
            // Check if the entered product name matches the one in the file
            if (strcmp(target, st.productname) == 0)
            {
                found = 1;  // Product found, break out of the loop
            }
        }
        
        // If product is found, display the product details
        if (found)
        {
            setcolor(2);
            printf("\n+------------------------------------------+\n");
            printf("|              Record Found               |\n");
            printf("+------------------------------------------+\n");
            resetcolor();
            printf("| Product Name     : %-20s |\n", st.productname);
            printf("| Product Company  : %-20s |\n", st.productcomp);
            printf("| Product Price    : %-20i |\n", st.price);
            printf("| Product ID       : %-20i |\n", st.productid);
            printf("| Product Quantity : %-20i |\n", st.Qnt);
            printf("+------------------------------------------+\n");
        }
        else
        {
            setcolor(4);
            printf("\nNo record found with the name '%s'.\n", target);
            resetcolor();
        }
    } else if (choice == 2) {
        // Search by product ID
        printf("\nEnter product ID to search: ");
        scanf("%d", &id);

        // Reset file pointer to the beginning
        rewind(sfile);

        // Loop through file to search for product ID
        found = 0;  // Reset found flag
        while (!feof(sfile) && found == 0)
        {
            fscanf(sfile, "%s %s %i %i %i", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt);
            
            // Check if the entered product ID matches the one in the file
            if (id == st.productid)
            {
                found = 1;  // Product found, break out of the loop
            }
        }
        
        // If product is found, display the product details
        if (found)
        {
            setcolor(2);
            printf("\n+------------------------------------------+\n");
            printf("|              Record Found                |\n");
            printf("+------------------------------------------+\n");
            resetcolor();
            printf("| Product Name     : %-20s |\n", st.productname);
            printf("| Product Company  : %-20s |\n", st.productcomp);
            printf("| Product Price    : %-20i |\n", st.price);
            printf("| Product ID       : %-20i |\n", st.productid);
            printf("| Product Quantity : %-20i |\n", st.Qnt);
            printf("+------------------------------------------+\n");
        }
        else
        {
            setcolor(4);
            printf("\nNo record found with the ID '%d'.\n", id);
            resetcolor();
        }
    } else {
        // Invalid choice, prompt user again
        printf("\nInvalid choice! Please select 1 or 2.");
    }

    // Close the file after reading
    fclose(sfile);
    
    // Prompt user to return to the main menu
    printf("\nPress any key to go to Main Menu!");
    
    // Wait for the user to press a key to continue
    while ((st.c = getch()) == '\r');
    
    menu();  // Go back to the main menu after the search operation
}



void deleteproduct(void)
{
    char target[40];
    int found = 0;
    FILE *sfile, *tfile;

    // Open the original file in read mode and a temporary file in write mode
    sfile = fopen("NextFile.txt", "r");
    tfile = fopen("TempFile.dat", "w+");

    // Check if the files were opened successfully
    if (sfile == NULL || tfile == NULL)
    {
        setcolor(4);
        printf("\nError: Could not open file.\n");
        resetcolor();
        getch();
        menu();  // Return to the menu if file can't be opened
        return;
    }

    // Prompt the user to enter the name of the product to delete
    system("cls");
    setcolor(1);
    printf("\n=========== Delete Product ===========\n");
    resetcolor();
    printf("+------------------------------------------+\n");
    printf("| Enter product name to delete: ");
    fflush(stdin);
    scanf("%s", target);
    target[0] = toupper(target[0]);  // Capitalize the first letter for consistent search

    // Read through the file and search for the product to delete
    while (fscanf(sfile, "%s %s %i %i %i\n", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF)
    {
        // If the product is found, mark it as found and skip writing it to the temp file
        if (strcmp(target, st.productname) == 0)
        {
            found = 1;
            log_delete_item(&st);  // Log the deletion activity
        }
        else
        {
            // If the product is not found, copy it to the temporary file
            fprintf(tfile, "%s %s %i %i %i\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
        }
    }

    // Display the result of the operation
    if (!found)
    {
        // If the product is not found, notify the user
        setcolor(4);
        printf("\n+------------------------------------------+\n");
        printf("|        Product Not Found                  |\n");
        printf("+------------------------------------------+\n");
        resetcolor();
    }
    else
    {
        // If the product is found and deleted, notify the user
        setcolor(2);
        printf("\n+------------------------------------------+\n");
        printf("|        Product Deleted Successfully      |\n");
        printf("+------------------------------------------+\n");
        resetcolor();
    }

    // Close the opened files
    fclose(sfile);
    fclose(tfile);

    // Remove the original file and rename the temporary file as the original file
    remove("NextFile.txt");
    rename("TempFile.dat", "NextFile.txt");

    // Prompt the user to return to the main menu
    printf("\nPress any key to go to Main Menu!\n");

    // Wait for the user to press a key before returning to the menu
    while ((st.c = getch()) == '\r');
    
    menu();  // Go back to the main menu after the operation
}

void read_item()
{
    FILE *f;  // File pointer for reading product data
    int i, q;  // Iteration variables

    // Open the file "NextFile.txt" for reading
    if ((f = fopen("NextFile.txt", "r")) == NULL)
    {
        // If file is not found or empty, notify the user
        setcolor(4);
        system("cls");
        printf("\n=========== No Records Found ===========\n");
        printf("|   No records available in the database  |\n");
        printf("+-----------------------------------------+\n");
        resetcolor();
        printf("\nPress any key to go to Main Menu!\n");
        getch();
        menu();  // Return to the main menu if no records are found
        return;
    }

    else

    {
        // Display a header for the product list
        system("cls");
        setcolor(1);
        printf("\n=========== Item List ===========\n");
        resetcolor();
        printf("+-----------------------------------------------------------------------------------------+\n");

        // Display column headers for product information
        printf("| %-20s | %-15s | %-20s | %-10s | %-10s |\n", "Product Name", "Price (Rupees)", "Company", "Product ID", "Quantity");
        printf("+-----------------------------------------------------------------------------------------+\n");

        // Start reading the data from the file and display it
        q = 8;  // Start from the 8th row for displaying product data
        while (fscanf(f, "%s %s %i %i %i\n", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF)
        {
            // Print each product's information in the respective columns
            printf("| %-20s | %-15i | %-20s | %-10i | %-10i |\n", st.productname, st.price, st.productcomp, st.productid, st.Qnt);
        }

        // Print a separator line after all records are displayed
        printf("+-----------------------------------------------------------------------------------------+\n");
    }

    // Close the file after reading
    fclose(f);

    // Prompt the user to return to the main menu
    printf("\nPress any key to go to Main Menu!\n");
    getch();  // Wait for the user to press a key
    menu();  // Return to the main menu
}

void edit_item()
{

    int index, valid;  // Index for iterating and validation flag
    char target[40];  // Target for user input (though not used directly here)
    FILE *fp, *rp;  // File pointers for reading the original file and writing to a temporary file
    int a = 0;  // Flag to check if the record was found
    int id;  // Variable to hold the product ID entered by the user
    char edit;  // User input to confirm if they want to edit the record

    // Try opening the product file in read and write mode
    if ((fp = fopen("NextFile.txt", "r+")) == NULL)
    {
        // If the file does not exist or cannot be opened, notify the user and return to the menu
        setcolor(4);
        printf("Error: No records available or file not found.\n");
        resetcolor();
        menu();
        return;
    }
    else
    {


        // Open a temporary file to store edited records
        rp = fopen("TempFile.dat", "w+");
        system("cls");  // Clear the console screen
        setcolor(1);
        printf("\n=========== Edit Item ===========\n\n");
        resetcolor();
        // Ask user for the product ID to edit
        printf("Enter Product ID for edit: ");
        scanf("%i", &id);
        fflush(stdin);  // Clear input buffer

        // Read records from the file to find the one to edit
        while (fscanf(fp, "%s %s %i %i %i\n", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF)
        {
            // If the product ID matches, allow the user to edit it
            if (id == st.productid)
            {
                a = 1;  // Mark that the record was found

                // Display the existing product details
                setcolor(2);
                printf("\n+------------------------------------------+\n");
                printf("|              Record Found                |\n");
                printf("+------------------------------------------+\n");
                resetcolor();
                printf("\nProduct Name\t\t: %s", st.productname);
                printf("\nProduct Company\t\t: %s", st.productcomp);
                printf("\nPrice\t\t\t: %i", st.price);
                printf("\nProduct ID\t\t: %i", st.productid);
                printf("\nProduct Quantity\t: %i", st.Qnt);
                

                setcolor(2);
                printf("\n+------------------------------------------+\n");
                printf("|              New Record                  |\n");
                printf("+------------------------------------------+\n");
                resetcolor();

                // Get new product details from the user, with validation where necessary

                // Validate product name (only alphabetic characters)
                do
                {
                    fflush(stdin);
                    printf("\nNew Product Name\t\t: ");
                    gets(st.productname);  // Get the new product name
                    st.productname[0] = toupper(st.productname[0]);  // Capitalize the first letter

                    // Check if the product name contains only alphabetic characters
                    for (index = 0; index < strlen(st.productname); ++index)
                    {
                        if (isalpha(st.productname[index]))
                            valid = 1;
                        else
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (!valid)
                    {
                        // If invalid input, prompt the user to re-enter
                        setcolor(4);
                        printf("\n Name contains invalid character. Please enter again.");
                        resetcolor();
                        getch();
                    }
                } while (!valid);

                // Validate product company (only alphabetic characters)
                do
                {
                    fflush(stdin);
                    printf("\nNew Product Company\t\t: ");
                    gets(st.productcomp);  // Get the new product company
                    st.productcomp[0] = toupper(st.productcomp[0]);  // Capitalize the first letter

                    // Check if the company name contains only alphabetic characters
                    for (index = 0; index < strlen(st.productcomp); ++index)
                    {
                        if (isalpha(st.productcomp[index]))
                            valid = 1;
                        else
                        {
                            valid = 0;
                            break;
                        }
                    }
                    if (!valid)
                    {
                        // If invalid input, prompt the user to re-enter
                        setcolor(4);
                        printf("\n Name contains invalid character. Please enter again.");
                        resetcolor();
                        getch();
                    }
                } while (!valid);

                // Validate product price (between 10 and 5000)
                do
                {
                    printf("\nNew Price [10-5000] Rupees\t: ");
                    scanf("%i", &st.price);
                    if (st.price < 10 || st.price > 5000)
                    {
                        setcolor(4);
                        printf("\n\tYou Cannot Enter the price limit [10-5000]. Re-Enter.");
                        resetcolor();
                    }
                } while (st.price < 10 || st.price > 5000);

                // Get new product ID
                printf("\nEnter New Product ID\t\t: ");
                scanf("%i", &st.productid);

                // Validate product quantity (between 1 and 500)
                do
                {
                    printf("\nNew Quantity [1-500]\t\t: ");
                    scanf("%i", &st.Qnt);
                    if (st.Qnt < 1 || st.Qnt > 500)
                    {
                        printf("\n\tEnter New Quantity [1-500] only. Re-Enter.");
                    }
                } while (st.Qnt < 1 || st.Qnt > 500);

                // Confirm if the user wants to edit the record
                setcolor(13);
                printf("\nPress 'y' to edit the existing record or any key to cancel...");
                edit = getche();  // Read user's choice to confirm or cancel the edit

                if (edit == 'y' || edit == 'Y')
                {
                    // If confirmed, write the edited record to the temporary file
                    fprintf(rp, "%s %s %i %i %i\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
                    fflush(stdin);  // Clear input buffer
                    setcolor(2);
                    printf("\n+---------------------------------------------------------------+\n");
                    printf("|\t\tYOUR RECORD IS SUCCESSFULLY EDITED!!!\t\t|\n");
                    printf("+---------------------------------------------------------------+\n");
                    resetcolor();
                }
            }
            else
            {
                // If the record is not being edited, copy it to the temporary file as is
                fprintf(rp, "%s %s %i %i %i\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
                fflush(stdin);  // Clear input buffer
            }
        }

        // If no record was found, notify the user
        if (!a)
        {   
            setcolor(4);
            printf("\n\nTHIS PRODUCT DOESN'T EXIST!!!!");
            resetcolor();
        }

        // Close both the files
        fclose(rp);
        fclose(fp);

        // Delete the original file and rename the temporary file to the original file name
        remove("NextFile.txt");
        rename("TempFile.dat", "NextFile.txt");

        getch();  // Wait for the user to press a key before returning to the menu
    }

    menu();  // Return to the main menu
}

// Purchase Function Implementation
void purchase_item() {
    int id, quantity, found = 0;
    FILE *fp, *temp, *bill;

    // Open the inventory file
    fp = fopen("NextFile.txt", "r");
    if (fp == NULL) {
        setcolor(4);
        printf("Error: No records available for purchase.\n");
        resetcolor();
        getch();
        menu();
        return;
    }

    temp = fopen("TempFile.dat", "w");  // Temp file for updated inventory
    bill = fopen("bill.txt", "w");     // Bill file to record purchase details
    setcolor(1);
    printf("\n=========== Search Product ===========\n");
    resetcolor();
    printf("Enter the Product ID to purchase: ");
    scanf("%d", &id);

    // Search for the product in the inventory
    while (fscanf(fp, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
        if (id == st.productid) {
            found = 1;
            printf("Enter quantity to purchase: ");
            scanf("%d", &quantity);

            if (quantity <= st.Qnt) {
                // Update inventory
                st.Qnt -= quantity;

                // Generate bill
                fprintf(bill, "=========== BILL ===========\n");
                fprintf(bill, "Product Name : %s\n", st.productname);
                fprintf(bill, "Company      : %s\n", st.productcomp);
                fprintf(bill, "Price (Each) : %d\n", st.price);
                fprintf(bill, "Quantity     : %d\n", quantity);
                fprintf(bill, "Total Price  : %d\n", quantity * st.price);
                fprintf(bill, "============================\n");

                // Log purchase
                char log_message[200];
                snprintf(log_message, sizeof(log_message), "Purchased %d of %s (ID: %d) for %d.",
                         quantity, st.productname, st.productid, quantity * st.price);
                log_purchase(log_message);
                log_activity(log_message);

                setcolor(2);
                printf("\n=======================================================================");
                printf("\n|                      Purchase Successfull                           |");
                printf("\n=======================================================================\n");
                resetcolor();
                printf("| Product ID   : %d\n", st.productid);
                printf("| Product Name : %s\n", st.productname);
                printf("| Company      : %s\n", st.productcomp);
                printf("| Price (Each) : %d\n", st.price);
                printf("| Quantity     : %d\n", quantity);
                printf("| Total Price  : %d\n", quantity * st.price);
                setcolor(2);
                printf("\n=======================================================================");
                printf("\n|                      Bill Generated in 'bill.txt'                   |");
                printf("\n=======================================================================\n");
                resetcolor();
                // Prompt user to return to the main menu
                printf("\nPress any key to go to Main Menu!");
            } else {
                setcolor(4);
                printf("\n=======================================================================");
                printf("\n|          Error: Insufficient stock. Available quantity: %d           |", st.Qnt);
                printf("\n=======================================================================\n");
                resetcolor();
                // Prompt user to return to the main menu  
                printf("\nPress any key to go to Main Menu!");
            }
        }
        // Write product details (updated or unchanged) to the temp file
        fprintf(temp, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
    }

    if (!found) {
        setcolor(4);
        printf("Error: Product not found.\n");
        resetcolor();
    }

    // Close all files
    fclose(fp);
    fclose(temp);
    fclose(bill);

    // Replace original inventory file with updated file
    remove("NextFile.txt");
    rename("TempFile.dat", "NextFile.txt");

    getch();
    menu();
}



// Update Stock Function Implementation
void update_stock() {
    FILE *fp, *temp;
    int id, found = 0, choice, value;

    // Open the original file for reading and a temporary file for updating stock
    fp = fopen("NextFile.txt", "r");
    if (fp == NULL) {
        setcolor(4);
        printf("Error: No records found. Cannot update stock.\n");
        resetcolor();
        printf("Press any key to return to the menu...");
        getch();
        menu();
        return;
    }

    temp = fopen("TempFile.dat", "w");
    if (temp == NULL) {
        setcolor(4);
        printf("Error: Could not create temporary file.\n");
        resetcolor();
        fclose(fp);
        printf("Press any key to return to the menu...");
        getch();
        menu();
        return;
    }

    // Display header
    system("cls");
    setcolor(1);
    printf("=========== UPDATE STOCK ===========\n");
    resetcolor();
    printf("| %-20s | %-10s | %-20s | %-10s | %-10s |\n", 
           "Name", "Price", "Company", "ID", "Quantity");
    printf("+----------------------+------------+----------------------+------------+------------+\n");

    // Show current inventory
    while (fscanf(fp, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
        printf("| %-20s | %-10d | %-20s | %-10d | %-10d |\n", 
               st.productname, st.price, st.productcomp, st.productid, st.Qnt);
    }
    printf("+----------------------+------------+----------------------+------------+------------+\n");

    // Close and reopen the file for modification
    fclose(fp);
    fp = fopen("NextFile.txt", "r");

    // Ask the user for the product ID
    printf("\nEnter the Product ID to update stock: ");
    scanf("%d", &id);

    // Search for the product and update stock
    while (fscanf(fp, "%s %s %d %d %d", st.productname, st.productcomp, &st.price, &st.productid, &st.Qnt) != EOF) {
        if (id == st.productid) {
            found = 1;

            // Ask the user to add or decrease stock
            setcolor(2 );
            printf("\nProduct Found: %s (ID: %d)\n", st.productname, st.productid);
            printf("Current Stock: %d\n", st.Qnt);
            resetcolor();
            setcolor(13);
            printf("\nChoose an option:\n");
            resetcolor();
            setcolor(1);
            printf("+------------------------------------------+\n");
            printf("| 1. Increase Stock                        |\n");
            printf("| 2. Decrease Stock                        |\n");
            printf("+------------------------------------------+\n");
            resetcolor();
            setcolor(13);
            printf("\nEnter your choice: ");
            resetcolor();
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter the quantity to add: ");
                scanf("%d", &value);
                if (value < 0) {
                    printf("Error: Cannot add a negative value.\n");
                    fprintf(temp, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
                    continue;
                }
                st.Qnt += value;
                setcolor(2);
                printf("+------------------------------------------+\n");
                printf("|Stock updated! New Stock: %d              |\n", st.Qnt);
                printf("+------------------------------------------+\n");
                resetcolor();
            } else if (choice == 2) {
                printf("Enter the quantity to decrease: ");
                scanf("%d", &value);
                if (value < 0 || value > st.Qnt) {
                    setcolor(4);
                    printf("Error: Invalid quantity. Cannot decrease.\n");
                    resetcolor();
                    fprintf(temp, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
                    continue;
                }
                st.Qnt -= value;

                setcolor(2);
                printf("+------------------------------------------+\n");
                printf("|Stock updated! New Stock: %d              |\n", st.Qnt);
                printf("+------------------------------------------+\n");
                resetcolor();
            } else {
                setcolor(4);
                printf("Invalid choice. Stock not updated.\n");
                resetcolor();
            }

            // Log activity
            char log_message[200];
            snprintf(log_message, sizeof(log_message), "Updated stock for %s (ID: %d). New Quantity: %d.", 
                     st.productname, st.productid, st.Qnt);
            log_activity(log_message);
        }

        // Write the updated or unchanged record to the temp file
        fprintf(temp, "%s %s %d %d %d\n", st.productname, st.productcomp, st.price, st.productid, st.Qnt);
    }

    if (!found) {
        setcolor(4);
        printf("Error: Product ID not found.\n");
        resetcolor();
    }

    // Close all files and replace the original file with the updated one
    fclose(fp);
    fclose(temp);
    remove("NextFile.txt");
    rename("TempFile.dat", "NextFile.txt");

    printf("Press any key to return to the menu...");
    getch();
    menu();
}


// End of the program
