
# Stock Management System  

A C-language-based project designed to efficiently manage stock information, streamline inventory tracking, and optimize resource usage. This system enables businesses to maintain a structured and up-to-date record of their stock, helping them make informed decisions.

---

## Features  
- **Stock Registration**: Add and manage items in inventory with unique identifiers.  
- **Search Functionality**: Quickly search for stock items based on various criteria.  
- **Stock Updates**: Modify existing stock details such as quantity and price.  
- **Stock Removal**: Remove items that are no longer in inventory.  
- **Reports Generation**: View summarized stock data for business analysis.

---

## Table of Contents  
1. [Introduction](#introduction)  
2. [Project Objectives](#project-objectives)  
3. [System Requirements](#system-requirements)  
4. [Project Structure](#project-structure)  
5. [File Descriptions](#file-descriptions)  
6. [Installation](#installation)  
7. [Usage](#usage)  
8. [Code Structure](#code-structure)  
9. [Contributing](#contributing)  
10. [License](#license)  

---

## Introduction  
The **Stock Management System** is a mini-project developed using C language. This project simulates a basic inventory management system and serves as an excellent learning tool for beginners aiming to understand file handling and basic C programming constructs.

---

## Project Objectives  
- Efficiently manage stock data for a business.  
- Provide real-time inventory updates.  
- Simplify stock search and tracking processes.  
- Log stock activities for audit and analysis purposes.

---

## System Requirements  
- A system capable of running C programs (Linux/Windows).  
- GCC compiler or any compatible C compiler.  

---

## Project Structure  
The project consists of the following components:  
- **Main Data File (`NextFile.txt`)**: Stores stock item details (product name, ID, quantity, price, etc.).  
- **Bill File (`bill.txt`)**: Records details of bills generated after purchases.  
- **Activity Log File (`activity_log.txt`)**: Logs actions performed on stock items.  
- **Purchase Log File (`purchase_log.txt`)**: Keeps track of purchase transactions.  

---

## File Descriptions  
1. **NextFile.txt**: Contains the main inventory data, updated dynamically as operations are performed.  
2. **bill.txt**: Stores generated bills for future reference and tracking.  
3. **activity_log.txt**: Logs all major activities such as stock additions, deletions, and updates.  
4. **purchase_log.txt**: Captures details of purchase operations.  

---

## Installation  
1. Clone the repository or download the project files.  
2. Compile the `Stock Management System` code using a C compiler (e.g., `gcc main.c -o stock_management`).  
3. Run the executable file to start the program.  

---

## Usage  
1. **Login**: Authenticate to access the system.  
2. **Menu Options**:  
   - Add, update, or delete stock items.  
   - Search for products by name or ID.  
   - Generate bills for purchases.  
   - View logs for auditing.  
3. Follow on-screen instructions for seamless navigation.  

---

## Code Structure  
The project structure and key functions are outlined below:  
- **Core Files**:  
  - `main.c`: Contains the core logic and function calls.  
- **Functions**:  
  - `wel_come()`: Displays the welcome screen.  
  - `login()`: Handles user authentication.  
  - `menu()`: Provides menu options to the user.  
  - `add_item()`, `edit_item()`, `deleteproduct()`: Manage stock items.  
  - `search_item()`: Search for stock by name or ID.  
  - `view_activity_log()`, `log_activity()`: Handle activity logs.  
  - `purchase_item()`: Manage stock purchases.  

Refer to the code for additional details on function implementation.

---

## Contributing  
Contributions are welcome! Please follow these steps:  
1. Fork the repository.  
2. Create a new branch.  
3. Make your changes and test them.  
4. Submit a pull request for review.  

---

## License  
This project is licensed under the MIT License.  
"""

