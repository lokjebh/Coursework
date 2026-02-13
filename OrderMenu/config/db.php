<?php 
/* Project: Food Order Management System
 * File: config/db.php
 * Purpose: This file handles the connection to the MySQL database.
 * It is included in other files to avoid repeating code.
 */

// Database credentials
$host = "localhost";
$user = "root";
$pass = ""; // Enter password
$dbname = "a2db";

// Connect to database
$conn = mysqli_connect($host, $user, $pass, $dbname);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
echo "Connected sucessfully";

/* Purpose: This function closes the database connection.*/
function close_db($connection) {
    if ($connection) {
        mysqli_close($connection);
    }
}
?>