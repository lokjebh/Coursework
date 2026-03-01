<?php
/* * Project Name: Food Order Management System
 * File: pages/add_menuitem.php
 */

include '../config/db.php';
include '../includes/header.php';

$message = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $id = $_POST['menuitemid'];
    $name = $_POST['dishname'];
    $price = $_POST['price'];
    $calories = $_POST['caloriecount'];
    $veggie = $_POST['veggie'];

    // Check if ID already exists
    $check = mysqli_query($conn, "SELECT menuitemid FROM menuitem WHERE menuitemid = '$id'");
    
    if (mysqli_num_rows($check) > 0) {
        $message = "<p style='color:red;'>Error: Menu Item ID '$id' is already in use.</p>";
    } else {
        $sql = "INSERT INTO menuitem (menuitemid, dishname, price, caloriecount, veggie) 
                VALUES ('$id', '$name', $price, $calories, '$veggie')";
        
        if (mysqli_query($conn, $sql)) {
            header("Location: view_menu.php?msg=added");
            exit();
        } else {
            $message = "<p style='color:red;'>Database Error: " . mysqli_error($conn) . "</p>";
        }
    }
}
?>

<main>
    <h2>Add New Dish to Menu</h2>
    <?php echo $message; ?>

    <form method="post" class="sort-form" style="text-align: left; display: inline-block;">
        <label>Item ID (4 chars):</label><br>
        <input type="text" name="menuitemid" maxlength="4" required placeholder="e.g. MZZZ"><br><br>

        <label>Dish Name:</label><br>
        <input type="text" name="dishname" required><br><br>

        <label>Price ($):</label><br>
        <input type="number" name="price" step="0.01" required><br><br>

        <label>Calorie Count:</label><br>
        <input type="number" name="caloriecount" required><br><br>

        <label>Vegetarian?</label><br>
        <select name="veggie">
            <option value="N">No</option>
            <option value="Y">Yes</option>
        </select><br><br>

        <input type="submit" value="Save New Item" class="btn-update">
        <a href="view_menu.php" style="margin-left: 10px;">Cancel</a>
    </form>
</main>

<?php include '../includes/footer.php'; ?>