# Quick Start Guide - Stock Management System

## 🚀 Getting Started in 5 Minutes

### Prerequisites Check
✅ Windows OS  
✅ GCC Compiler (MinGW)  
✅ Command Prompt or Terminal

### Installation

**Step 1: Build the Application**

Open Command Prompt in the project directory and run:
```bash
build.bat
```

Or using Makefile:
```bash
make build
```

**Step 2: Run the Application**
```bash
stock_system.exe
```

### First Login

**Default Credentials:**
- Username: `admin`
- Password: `admin`

⚠️ **Note**: Change default password in production use!

---

## 📚 Basic Usage

### Adding Your First Product

1. From Main Menu, select `1` (Stock Management)
2. Select `1` (Add New Product)
3. Enter product details:
   - Name: e.g., "Laptop"
   - Category: e.g., "Electronics"
   - Price: e.g., 999.99
   - Quantity: e.g., 25
   - Supplier ID: 0 (if none)
4. Confirm with `y`

✅ **Result**: Product added and transaction recorded!

### Making Your First Sale

1. From Main Menu, select `3` (Sales and Billing)
2. Select `1` (Create New Sale)
3. Enter Product ID: (use ID from previous step)
4. Enter quantity to sell
5. Confirm sale with `y`

✅ **Result**: Stock updated, bill generated in `data/` folder!

### Viewing Stock Trends

1. Go to Stock Management → `11` (View Stock Trend Graph)
2. Enter Product ID
3. View ASCII graph showing stock changes over time

---

## 🎯 Common Tasks

### Check Low Stock Items
**Main Menu → 1 → 9**

Shows all products below threshold (default: 10 units)

### View Transaction History
**Main Menu → 1 → 10**

Enter Product ID to see complete history of stock movements

### Generate Reports
**Main Menu → 4** (Reports and Analytics)

Options:
- `1` - Daily Sales Report
- `2` - Monthly Sales Report
- `3` - Revenue Summary
- `4` - Stock Valuation

### Export Data to CSV
**Main Menu → 4 → 9**

Exports stock and sales data to `data/stock_export.csv` and `data/sales_export.csv`

### Backup Data
**Main Menu → 5 → 1**

Creates backup of all data files in `data/backup/` directory

---

## 📊 Understanding the Interface

### Menu Navigation
- Enter number corresponding to your choice
- Press Enter to confirm
- Use menu option to return to previous screen

### Color Coding
- 🟢 **Green**: Success messages
- 🔴 **Red**: Errors and warnings
- 🔵 **Blue/Cyan**: Headers and titles
- 🟡 **Yellow**: Warnings

---

## 💡 Pro Tips

### Tip 1: Quick Stock Check
Use the Stock Bar Graph (Stock Management → 12) for a visual overview of all product quantities.

### Tip 2: Monitor Trends
Regularly check trend graphs to identify:
- Fast-moving products
- Slow-moving inventory
- Seasonal patterns

### Tip 3: Transaction History
Before making stock adjustments, check transaction history to understand past movements.

### Tip 4: Low Stock Alerts
Check Low Stock Report daily to prevent stockouts.

### Tip 5: Activity Logs
Use Activity Logs (Reports → 8) for audit trails and troubleshooting.

---

## 🔧 Troubleshooting

### Problem: "No inventory records found"
**Solution**: Add some products first using Add New Product option.

### Problem: "Product not found"
**Solution**: 
- Check Product ID by viewing all products (option 2)
- Ensure you're entering the correct numeric ID

### Problem: "Insufficient stock"
**Solution**: 
- Check current quantity first
- Update stock quantity if needed (Stock Management → 6)

### Problem: Build fails
**Solution**:
- Ensure GCC is installed: `gcc --version`
- Check all source files are present in `src/` directory
- Verify `data/` directory exists

### Problem: "Unable to open file"
**Solution**:
- Create `data/` directory if it doesn't exist
- Check file permissions
- Run application from correct directory

---

## 📁 File Structure

```
stock/
├── stock_system.exe    ← Run this
├── data/              ← All data stored here
│   ├── stock.dat
│   ├── sales.dat
│   ├── transactions.dat
│   └── bill_*.txt     ← Generated bills
├── assets/
│   └── banner.txt
└── src/               ← Source code
```

---

## 🎓 Example Workflow

### Scenario: Setting Up a Small Electronics Shop

**Day 1: Initial Setup**
1. Add suppliers (Supplier Management)
2. Add products:
   - Laptop - $999.99 - Qty: 10
   - Mouse - $25.00 - Qty: 50
   - Keyboard - $45.00 - Qty: 30
   - Monitor - $250.00 - Qty: 15

**Day 1: First Sale**
1. Customer buys 2 mice
2. Process sale
3. Bill generated automatically
4. Stock updated: Mice qty becomes 48

**Week 1: Stock Check**
1. View Stock Bar Graph
2. Check Low Stock Alerts
3. Reorder products below threshold

**Month 1: Analysis**
1. Generate Monthly Sales Report
2. View Most Sold Items
3. Check Stock Valuation
4. Review Stock Trends

**Month End: Data Management**
1. Export to CSV for external analysis
2. Create data backup
3. Review Activity Logs

---

## 📈 Understanding Graphs

### Stock Trend Graph
```
  45 |           *        ← Peak stock level
  40 |          *|
  35 |        * ||
  30 |       *| ||  *    ← Trend line
  25 |      *|| || *|
  20 |    * ||| ||*||
  15 |   *| |||*||||
  10 |  *||*|||||||
   5 | *||||||||||        ← Minimum level
     +-------------------
      01/15  01/20  01/25  ← Timeline
```

**Interpretation**:
- `*` = Stock level at that point
- `|` = Vertical fill showing quantity
- Rising trend = Stock increasing
- Falling trend = Stock depleting

### Bar Graph
```
Laptop      | ███████████ 10
Mouse       | ████████████████████ 48
Keyboard    | ███████████████ 30
Monitor     | ████████████ 15
```

**Color Coding**:
- 🟢 Green: Healthy stock
- 🟡 Yellow: Getting low
- 🔴 Red: Below threshold

---

## ⚙️ System Configuration

### Default Settings
- **Low Stock Threshold**: 10 units
- **Price Range**: 10.00 - 5000.00
- **Quantity Range**: 1 - 500
- **Login Attempts**: 3

### Changing Defaults
Edit in respective header files (requires recompilation):
- `LOW_STOCK_THRESHOLD` in `include/stock.h`
- Other limits in validation functions

---

## 🔒 Security Best Practices

1. **Change default password** after first login
2. **Regular backups** of data directory
3. **Monitor activity logs** for unauthorized access
4. **Secure data directory** with appropriate permissions
5. **Don't share** executable with embedded credentials

---

## 📞 Support

### For Issues:
1. Check this guide first
2. Review error messages carefully
3. Check Activity Logs for details
4. Ensure data files aren't corrupted

### For Enhancement Requests:
See `docs/DEVELOPER.md` for contribution guidelines

---

## 🎯 Next Steps

After mastering basics:
1. Explore advanced reporting features
2. Set up supplier relationships
3. Analyze sales patterns with graphs
4. Customize for your business needs
5. Consider contributing improvements

---

## 📋 Keyboard Shortcuts

- Enter number + Enter: Select menu option
- y/n: Confirm actions
- Ctrl+C: Emergency exit (not recommended)

---

## ⏱️ Quick Reference Card

| Task | Navigation |
|------|------------|
| Add Product | 1 → 1 |
| View Stock | 1 → 2 |
| Make Sale | 3 → 1 |
| Low Stock Alert | 1 → 9 |
| Transaction History | 1 → 10 |
| Trend Graph | 1 → 11 |
| Daily Report | 4 → 1 |
| Export CSV | 4 → 9 |
| Backup | 5 → 1 |

---

**Happy Stock Managing! 📦**

For detailed technical information, see [`docs/DEVELOPER.md`](DEVELOPER.md)  
For feature overview, see [`README.md`](../README.md)
