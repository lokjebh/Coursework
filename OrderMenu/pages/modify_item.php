<?php
include '../config/db.php';
include '../includes/header.php';

$id = $_GET['id'];
$message = "";

// 1. Fetch current data
$result = mysqli_query($conn, "SELECT * FROM menuitem WHERE menuitemid = '$id'");
$item = mysqli_fetch_assoc($result);

// 2. Handle Update Logic
if (isset($_POST['update'])) {
    $price = $_POST['price'];
    $calories = $_POST['calories'];
    
    $updateSql = "UPDATE menuitem SET price = $price, caloriecount = $calories WHERE menuitemid = '$id'";
    if (mysqli_query($conn, $updateSql)) {
        header("Location: view_menu.php?msg=updated");
        exit();
    }
}

// 3. Handle Delete Logic
if (isset($_POST['delete'])) {
    // Check if item is used in any orders first 
    $checkOrder = mysqli_query($conn, "SELECT * FROM overallorder WHERE menuitemid = '$id'");
    
    if (mysqli_num_rows($checkOrder) > 0) {
        $message = "<p style='color:red;'>Error: Cannot delete. This item is part of existing customer orders.</p>";
    } else {
        mysqli_query($conn, "DELETE FROM menuitem WHERE menuitemid = '$id'");
        header("Location: view_menu.php?msg=deleted");
        exit();
    }
}
?>

<main>
    <h2>Modify: <?php echo $item['dishname']; ?></h2>
    <?php echo $message; ?>

    <form method="post" onsubmit="return confirmDelete(this);">
        <p>Item ID: <strong><?php echo $item['menuitemid']; ?></strong></p>
        
        <label>Price ($):</label>
        <input type="number" name="price" step="0.01" value="<?php echo $item['price']; ?>" required><br><br>
        
        <label>Calories:</label>
        <input type="number" name="calories" value="<?php echo $item['caloriecount']; ?>" required><br><br>

        <input type="submit" name="update" value="Update Item" class="btn-update">
        <input type="submit" name="delete" value="Delete Item" id="delete_btn" style="background-color: red; color: white; border: none; padding: 10px; cursor: pointer;">
        
        
        <br><br>
        <a href="view_menu.php">Cancel and go back</a>
    </form>
</main>

<script src="../assets/js/modify_logic.js"></script>

<?php include '../includes/footer.php'; ?>