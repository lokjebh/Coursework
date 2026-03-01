<?php
/* * Project Name: Food Order Management System
 * File: pages/driver_report.php
 * Purpose: Lists all drivers who have not yet made a delivery.
 */

include '../config/db.php';
include '../includes/header.php';

// SQL to find drivers not present in the cusorder table
$sql = "SELECT d.driverid, d.firstname, d.lastname, d.cellnum 
        FROM driver d 
        LEFT JOIN cusorder o ON d.driverid = o.driverid 
        WHERE o.driverid IS NULL";

$result = mysqli_query($conn, $sql);
?>

<main>
    <h2>Drivers with No Deliveries</h2>
    <p>The following drivers are currently in the system but have not been assigned to any orders:</p>

    <table>
        <thead>
            <tr>
                <th>Driver ID</th>
                <th>First Name</th>
                <th>Last Name</th>
                <th>Cell Number</th>
            </tr>
        </thead>
        <tbody>
            <?php
            if (mysqli_num_rows($result) > 0) {
                while($row = mysqli_fetch_assoc($result)) {
                    echo "<tr>";
                    echo "<td>" . $row["driverid"] . "</td>";
                    echo "<td>" . $row["firstname"] . "</td>";
                    echo "<td>" . $row["lastname"] . "</td>";
                    echo "<td>" . $row["cellnum"] . "</td>";
                    echo "</tr>";
                }
            } else {
                echo "<tr><td colspan='4'>All drivers have made at least one delivery.</td></tr>";
            }
            ?>
        </tbody>
    </table>

    <br>
    <a href="../mainmenu.php" class="btn-update" style="text-decoration: none;">Return to Main Menu</a>
</main>

<?php include '../includes/footer.php'; ?>