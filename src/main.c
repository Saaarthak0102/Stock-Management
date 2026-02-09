/*
 * Stock Management System - Main Entry Point
 * 
 * A professional inventory management system with transaction tracking,
 * visual analytics, and comprehensive reporting capabilities.
 * 
 * Features:
 *   - Stock CRUD operations with validation
 *   - Sales processing with automatic billing
 *   - Transaction history tracking
 *   - ASCII trend graphs and visualizations
 *   - Low stock alert system
 *   - Comprehensive reporting
 *   - Data export (CSV)
 *   - Activity logging
 * 
 * Architecture:
 *   - Modular design with separated concerns
 *   - File-based persistence (binary format)
 *   - UI-agnostic core logic (future-ready for ncurses)
 *   - Comprehensive error handling
 *   - Input validation throughout
 * 
 * Build:
 *   Use Makefile, build.bat, or VS Code tasks
 * 
 * Usage:
 *   ./stock_system.exe
 *   Default credentials: admin/admin
 * 
 * Author: [Your Name]
 * Version: 2.0
 * Date: February 2026
 * Language: C (C11 Standard)
 */

#include <stdio.h>
#include <stdlib.h>
#include "../include/auth.h"
#include "../include/menu.h"
#include "../include/utils.h"

/*
 * Main entry point
 * 
 * Flow:
 *   1. Display welcome banner
 *   2. Initialize user database (create default admin if needed)
 *   3. Authenticate user (3 attempts allowed)
 *   4. Display main menu (loops until logout)
 * 
 * Returns: 0 on normal exit
 */
int main(void) {
    display_banner();
    init_users_file();
    
    if (!authenticate_user()) {
        exit(0);
    }
    
    display_main_menu();
    return 0;
}
