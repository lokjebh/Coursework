<?php
/**
 * Main navigation hub for the Food Order Management System.
 *
 * This file handles the main menu and links to other parts of the app.
 *
 * @package OrderMenu
 * @file    mainmenu.php
 */

// Include the database connection
include 'config/db.php';

// Include the visual header
include 'includes/header.php';
?>

<main>
    <h2>main menu</h2>
    <!-- <p>Please select a task from the options below:</p> -->
    
    <div class="menu-options">
        <ul>
            <li><a href="pages/view_menu.php">View Items</a></li>
            <li><a href="pages/add_order.php">Add Order</a></li>
            <li><a href="pages/Modify_item.php">Modify Items</a></li>
            <li><a href="pages/delete_item.php">Delete Items</a></li>
            <li><a href="pages/driver_report.php">View Drivers</a></li>
            <li><a href="pages/order_details.php">View Orders</a></li>
        </ul>
    </div>
</main>

<?php 
// Include the footer which also closes the DB connection
include 'includes/footer.php'; 
?>