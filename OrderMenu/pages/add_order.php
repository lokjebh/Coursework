PHP
<?php
/* * Project Name: Food Order Management System
 * File: pages/add_order.php
 * Purpose: Allows users to insert a new order, ensuring unique IDs, 
 * valid timestamps, and linking customers, drivers, and items.
 */

include '../config/db.php';
include '../includes/header.php';

$message = "";

// Handle Form Submission
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $orderid = $_POST['orderid'];
    $cusid = $_POST['cusid'];
    $driverid = $_POST['driverid'] === "" ? NULL : $_POST['driverid'];
    $deladdress = $_POST['deladdress'];
    $dateplaced = $_POST['dateplaced'];
    $timeplaced = $_POST['timeplaced'];
    $timedelivered = $_POST['timedelivered'] === "" ? NULL : $_POST['timedelivered'];
    $pickup = $_POST['pickuporder'];
    $rating = $_POST['rating'] === "" ? NULL : $_POST['rating'];

    // --- If pickup is 'Yes', clear delivery-only fields ---
    if ($pickup == 'Y') {
        $driverid = NULL;
        $timedelivered = NULL;
        $rating = NULL;
    } else {
        // Handle empty strings for optional delivery fields
        $driverid = ($driverid === "") ? NULL : $driverid;
        $timedelivered = ($timedelivered === "") ? NULL : $timedelivered;
        $rating = ($rating === "") ? NULL : $rating;
    }
    
    // 1. Unique Order ID Check
    $checkID = mysqli_query($conn, "SELECT orderid FROM cusorder WHERE orderid = '$orderid'");
    
    // 2. Time Logic Check
    $validTime = true;
    if ($timedelivered && $timeplaced >= $timedelivered) {
        $validTime = false;
    }

    if (mysqli_num_rows($checkID) > 0) {
        $message = "<p style='color:red;'>Error: Order ID already in use.</p>";
    } elseif (!$validTime) {
        $message = "<p style='color:red;'>Error: Delivery time must be after order time.</p>";
    } else {
        // Prepared values for SQL (handling NULLs correctly)
        $driverVal = ($driverid) ? "'$driverid'" : "NULL";
        $timeDelVal = ($timedelivered) ? "'$timedelivered'" : "NULL";
        $ratingVal = ($rating !== NULL) ? $rating : "NULL";        

        // Insert into cusorder
        $sqlOrder = "INSERT INTO cusorder (orderid, deladdress, dateplaced, timeplaced, timedelivered, pickuporder, deliveryrating, driverid, cusid) 
                     VALUES ('$orderid', '$deladdress', '$dateplaced', '$timeplaced', '$timedelivered', '$pickup', " . ($rating ?? "NULL") . ", " . ($driverid ? "'$driverid'" : "NULL") . ", '$cusid')";
        
        if (mysqli_query($conn, $sqlOrder)) {
            // Insert selected items into overallorder
            foreach ($_POST['items'] as $menuitemid => $quantity) {
                if ($quantity > 0) {
                    mysqli_query($conn, "INSERT INTO overallorder (orderid, menuitemid, quantity) VALUES ('$orderid', '$menuitemid', $quantity)");
                }
            }
            // Redirect to summary (a requirement to show the inserted order)
            header("Location: order_summary.php?orderid=$orderid");
            exit();
        } else {
            $message = "<p style='color:red;'>Database Error: " . mysqli_error($conn) . "</p>";
        }
    }
}

// Fetch lists for dropdowns
$customers = mysqli_query($conn, "SELECT cusid, firstname, lastname FROM customer");
$drivers = mysqli_query($conn, "SELECT driverid, firstname, lastname FROM driver");
$menuItems = mysqli_query($conn, "SELECT menuitemid, dishname, price FROM menuitem");
?>

<main>
    <h2>Insert New Order</h2>
    <?php echo $message; ?>

    <form method="post" action="add_order.php">
        <fieldset>
            <legend>Order Details</legend>
            Order ID: <input type="text" name="orderid" required placeholder="e.g. C205"><br>
            Delivery Address: <input type="text" name="deladdress" required><br>
            Date: <input type="date" name="dateplaced" required value="<?php echo date('Y-m-d'); ?>"><br>
            Time Placed: <input type="time" name="timeplaced" required><br>

            Pickup? 
            <input type="radio" name="pickuporder" value="Y" onclick="toggleFields(true)"> Yes 
            <input type="radio" name="pickuporder" value="N" onclick="toggleFields(false)" checked> No<br>

            <div id="delivery_fields">
                Time Delivered: <input type="time" name="timedelivered" id="time_del"><br>
                Rating (1-5): <input type="number" name="rating" id="rate_val" min="1" max="5">
            </div>
        </fieldset>

        <fieldset>
            <legend>People</legend>
            Customer: 
            <select name="cusid" required>
                <option value="">--Select Customer--</option>
                <?php while($c = mysqli_fetch_assoc($customers)) echo "<option value='{$c['cusid']}'>{$c['firstname']} {$c['lastname']} ({$c['cusid']})</option>"; ?>
            </select><br>
            
            <div id="driver_section">
                Driver: 
                <select name="driverid" id="driver_select">
                    <option value="">--Select Driver--</option>
                    <?php while($d = mysqli_fetch_assoc($drivers)) echo "<option value='{$d['driverid']}'>{$d['firstname']} {$d['lastname']} ({$d['driverid']})</option>"; ?>
                </select>
            </div>
        </fieldset>

        <fieldset>
            <legend>Menu Items & Quantities</legend>
            <?php while($item = mysqli_fetch_assoc($menuItems)): ?>
                <div style="margin-bottom: 5px;">
                    <?php echo $item['dishname']; ?> ($<?php echo $item['price']; ?>): 
                    <input type="number" name="items[<?php echo $item['menuitemid']; ?>]" value="0" min="0" style="width: 50px;">
                </div>
            <?php endwhile; ?>
        </fieldset>

        <br>
        <input type="submit" value="Place Order" class="btn-update">
    </form>
</main>

<script src="../assets/js/order_logic.js"></script>

<?php include '../includes/footer.php'; ?>