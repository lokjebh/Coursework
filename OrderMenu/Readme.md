# Food Order Management System

## Project Description
The Food Order Management System is a web-based application designed to manage food orders, menu items, customers, and drivers. Built with PHP and MySQL, it allows users to interact with the `a2db` database to handle menu items, track customer orders, and manage driver deliveries.

The system features a modular architecture, keeping database logic, styling, and user interface components separate for better maintainability.

---
## Features
- Neatly list all the menu items (every field).
- Insert a new order.
- Delete an existing menu item.
- Modify an existing menu item.
- Display drivers who have not made any deliveries.
- Allow a user to select an order and see everything about that order.

---

## Installation & Setup
1. Database Setup:
- Import the provided `a2db_script.sql` into MySQL server .
- Ensure the database is named `a2db`.

2. Configuration:
- Open `config/db.php` and update the database credentials (username/password) to match local environment.

3. Start Apache and MySQL services:
- `sudo service apache2 start`
- `sudo service mysql start`

4. Access:
- Open browser and navigate to `http://localhost/OrderMenu/mainmenu.php`.
