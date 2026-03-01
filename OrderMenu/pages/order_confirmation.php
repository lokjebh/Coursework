<?php
/* * Project Name: Food Order Management System
 * File: pages/order_summary.php
 * Purpose: Displays the details and total price of a newly placed order.
 */

include '../config/db.php';
include '../includes/header.php';

// Get the Order ID from the URL
$orderid = $_GET['orderid'];

// 1. Fetch main order details (Address, Date, Customer Name)
$sqlMain = "SELECT o.orderid, o.deladdress, o.dateplaced, c.firstname, c.lastname 
            FROM cusorder o 
            JOIN customer c ON o.cusid = c.cusid 
            WHERE o.orderid = '$orderid'";
$mainResult = mysqli_query($conn, $sqlMain);
$orderData = mysqli_fetch_assoc($mainResult);

// 2. Fetch the items in this order and their prices
$sqlItems = "SELECT m.dishname, m.price, ov.quantity 
             FROM overallorder ov 
             JOIN menuitem m ON ov.menuitemid = m.menuitemid 
             WHERE ov.orderid = '$orderid'";
$itemsResult = mysqli_query($conn, $sqlItems);
?>

<main>
    <h2>Order Confirmation</h2>
    <div style="background: #fff; padding: 20px; border: 1px solid #ccc; display: inline-block; text-align: left;">
        <p><strong>Order ID:</strong> <?php echo $orderData['orderid']; ?></p>
        <p><strong>Customer:</strong> <?php echo $orderData['firstname'] . " " . $orderData['lastname']; ?></p>
        <p><strong>Address:</strong> <?php echo $orderData['deladdress']; ?></p>
        <p><strong>Date:</strong> <?php echo $orderData['dateplaced']; ?></p>

        <h3>Items Ordered</h3>
        <table border="1" style="width: 100%; border-collapse: collapse;">
            <thead>
                <tr style="background: #eee;">
                    <th>Dish Name</th>
                    <th>Price</th>
                    <th>Quantity</th>
                    <th>Subtotal</th>
                </tr>
            </thead>
            <tbody>
                <?php 
                $totalOrderPrice = 0;
                while($item = mysqli_fetch_assoc($itemsResult)): 
                    $subtotal = $item['price'] * $item['quantity'];
                    $totalOrderPrice += $subtotal;
                ?>
                <tr>
                    <td><?php echo $item['dishname']; ?></td>
                    <td>$<?php echo number_format($item['price'], 2); ?></td>
                    <td><?php echo $item['quantity']; ?></td>
                    <td>$<?php echo number_format($subtotal, 2); ?></td>
                </tr>
                <?php endwhile; ?>
            </tbody>
            <tfoot>
                <tr style="font-weight: bold; background: #f2f2f2;">
                    <td colspan="3" style="text-align: right;">Total Price:</td>
                    <td>$<?php echo number_format($totalOrderPrice, 2); ?></td>
                </tr>
            </tfoot>
        </table>
    </div>
    <br><br>
    <a href="../mainmenu.php" class="btn-update" style="text-decoration: none;">Return to Main Menu</a>
</main>

<?php include '../includes/footer.php'; ?>