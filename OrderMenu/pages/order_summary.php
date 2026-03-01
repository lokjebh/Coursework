<?php
/* * Project Name: Food Order Management System
 * File: pages/order_summary.php
 * Purpose: Master-Detail view of all orders with sorting and side-panel details.
 */
include '../config/db.php';
include '../includes/header.php';

// 1. Handle Sorting
$sort = isset($_GET['sort']) ? $_GET['sort'] : 'orderid';
$dir = isset($_GET['dir']) ? $_GET['dir'] : 'ASC';
$nextDir = ($dir == 'ASC') ? 'DESC' : 'ASC';
$icon = ($dir == 'ASC') ? '▲' : '▼';

// 2. Fetch All Orders for the list
$sqlList = "SELECT orderid, deladdress, dateplaced FROM cusorder ORDER BY $sort $dir";
$listResult = mysqli_query($conn, $sqlList);

// 3. Fetch Details if an order is selected
$selectedOrder = isset($_GET['view']) ? $_GET['view'] : null;
$details = null;
$items = null;

if ($selectedOrder) {
    $sqlDet = "SELECT o.*, c.firstname as cf, c.lastname as cl, d.firstname as df, d.lastname as dl 
               FROM cusorder o 
               JOIN customer c ON o.cusid = c.cusid 
               LEFT JOIN driver d ON o.driverid = d.driverid 
               WHERE o.orderid = '$selectedOrder'";
    $details = mysqli_fetch_assoc(mysqli_query($conn, $sqlDet));

    $sqlItems = "SELECT m.dishname, m.price, ov.quantity 
                 FROM overallorder ov 
                 JOIN menuitem m ON ov.menuitemid = m.menuitemid 
                 WHERE ov.orderid = '$selectedOrder'";
    $items = mysqli_query($conn, $sqlItems);
}
?>

<main style="display: flex; gap: 20px; align-items: flex-start; padding: 10px;">
    <section style="flex: 1; background: #fff; padding: 15px; border: 1px solid #ccc;">
        <h3>All Orders</h3>
        <table style="width: 100%; font-family: Consolas, monospace;">
            <thead>
                <tr style="background: #333; color: white;">
                    <th><a href="?sort=orderid&dir=<?php echo $nextDir; ?>" style="color:white; text-decoration:none;">ID <?php if($sort=='orderid') echo $icon; ?></a></th>
                    <th>Address</th>
                    <th><a href="?sort=dateplaced&dir=<?php echo $nextDir; ?>" style="color:white; text-decoration:none;">Date <?php if($sort=='dateplaced') echo $icon; ?></a></th>
                </tr>
            </thead>
            <tbody>
                <?php while($row = mysqli_fetch_assoc($listResult)): ?>
                <tr onclick="window.location='?view=<?php echo $row['orderid']; ?>&sort=<?php echo $sort; ?>&dir=<?php echo $dir; ?>'" 
                    style="cursor:pointer; <?php if($selectedOrder == $row['orderid']) echo 'background:#d4edda;'; ?>"
                    onmouseover="this.style.background='#e2e2e2'" onmouseout="this.style.background='<?php echo ($selectedOrder == $row['orderid']) ? '#d4edda' : 'transparent'; ?>'">
                    <td><?php echo $row['orderid']; ?></td>
                    <td><?php echo $row['deladdress']; ?></td>
                    <td><?php echo $row['dateplaced']; ?></td>
                </tr>
                <?php endwhile; ?>
            </tbody>
        </table>
    </section>

    <section style="flex: 1; background: #fff; padding: 15px; border: 1px solid #ccc; min-height: 400px;">
        <?php if ($details): ?>
            <h3>Details for Order: <?php echo $details['orderid']; ?></h3>
            <p><strong>Customer:</strong> <?php echo $details['cf'] . " " . $details['cl']; ?></p>
            <p><strong>Driver:</strong> <?php echo $details['df'] ? $details['df'] . " " . $details['dl'] : "N/A (Pickup)"; ?></p>
            <p><strong>Placed:</strong> <?php echo $details['timeplaced']; ?> | <strong>Delivered:</strong> <?php echo $details['timedelivered'] ?? "Pending"; ?></p>
            <p><strong>Rating:</strong> <?php echo $details['deliveryrating'] ?? "None"; ?> / 5</p>
            
            <h4>Items</h4>
            <table style="width: 100%; border-collapse: collapse;">
                <tr style="border-bottom: 2px solid #333;">
                    <th>Item</th><th>Qty</th><th>Price</th><th>Subtotal</th>
                </tr>
                <?php 
                $grandTotal = 0;
                while($i = mysqli_fetch_assoc($items)): 
                    $sub = $i['price'] * $i['quantity'];
                    $grandTotal += $sub;
                ?>
                <tr>
                    <td><?php echo $i['dishname']; ?></td>
                    <td><?php echo $i['quantity']; ?></td>
                    <td>$<?php echo number_format($i['price'], 2); ?></td>
                    <td>$<?php echo number_format($sub, 2); ?></td>
                </tr>
                <?php endwhile; ?>
                <tr style="font-weight:bold; background:#eee;">
                    <td colspan="3" style="text-align:right;">Grand Total:</td>
                    <td>$<?php echo number_format($grandTotal, 2); ?></td>
                </tr>
            </table>
        <?php else: ?>
            <p style="color: #666; margin-top: 50px;">Click an order on the left to view full details.</p>
        <?php endif; ?>
    </section>
</main>

<?php include '../includes/footer.php'; ?>