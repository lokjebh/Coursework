<?php 
include '../config/db.php';

// Test: connection to database
if ($conn) {
    echo "<h1>Test connection Success!</h1>";
    echo "<p>The Food Order Management System is connected to the database.</p>";
}

// Test: Close the connection using close_db function
close_db($conn);
?>