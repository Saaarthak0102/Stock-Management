@echo off
echo Building Stock Management System...
gcc -I. -Wall -Wextra -std=c99 -o stock_system.exe src/main.c src/auth.c src/menu.c src/stock.c src/supplier.c src/sales.c src/reports.c src/utils.c

if %errorlevel% equ 0 (
    echo.
    echo Build successful! Launching application...
    echo.
    stock_system.exe
) else (
    echo.
    echo Build failed with errors.
    pause
)
