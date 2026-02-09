# Stock Management System

A modular, terminal-based Stock Management System in C using binary file storage.

## Features
- Authentication with password masking
- Stock management (add, search, update, delete)
- Supplier management and linking
- Sales and billing with auto stock updates
- Reports and analytics
- Activity logs
- Backup and restore

## Build

Using GCC:
```
gcc -I. -Wall -Wextra -std=c99 -o stock_system.exe src/main.c src/auth.c src/menu.c src/stock.c src/supplier.c src/sales.c src/reports.c src/utils.c
```

Using Makefile (if make is available):
```
make build
make run
```

## Run
```
stock_system.exe
```

## Default Login
- Username: user
- Password: pass

## Data Files
Stored in data/ as binary files:
- users.dat
- stock.dat
- suppliers.dat
- sales.dat
- logs.dat

## Screenshots (ASCII)

```
=====================
STOCK MANAGEMENT MENU
=====================
1. Add Product
2. View Products
3. Search Product
...
```
