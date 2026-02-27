<?php
/* * Project Name: Food Order Management System
 * File: pages/view_menu.php
 * Purpose: Lists all menu items and allows sorting by name or price.
 */

include '../config/db.php';
include '../includes/header.php';

// 1. Set default sorting values
$orderBy = "dishname"; 
$orderDir = "ASC";     

// 2. Handle user submission
if (isset($_POST['sort_by'])) {
    $orderBy = $_POST['sort_by'];
}
if (isset($_POST['sort_dir'])) {
    $orderDir = $_POST['sort_dir'];
}

// 3. Query the database
// We use $orderBy and $orderDir to dynamically change the order
$sql = "SELECT * FROM menuitem ORDER BY $orderBy $orderDir";
$result = mysqli_query($conn, $sql);
?>

<main>
    <h2>Menu</h2>

    <div class="sort-container">
        <form method="post" action="view_menu.php">
            <strong>Sort by:</strong>
            <input type="radio" name="sort_by" value="dishname" <?php if($orderBy == "dishname") echo "checked"; ?>> Dish Name
            <input type="radio" name="sort_by" value="price" <?php if($orderBy == "price") echo "checked"; ?>> Price
            <br>
            <strong>Order:</strong>
            <input type="radio" name="sort_dir" value="ASC" <?php if($orderDir == "ASC") echo "checked"; ?>> Ascending
            <input type="radio" name="sort_dir" value="DESC" <?php if($orderDir == "DESC") echo "checked"; ?>> Descending
            <br><br>
            <input type="submit" value="Update List" class="btn-update">
        </form>
    </div>

    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>Dish Name</th>
                <th>Calories</th>
                <th>Price</th>
                <th>Veggie</th>
            </tr>
        </thead>
        <tbody>
            <?php
            if (mysqli_num_rows($result) > 0) {
                while($row = mysqli_fetch_assoc($result)) {
                    echo "<tr>";
                    echo "<td>" . $row["menuitemid"] . "</td>";
                    echo "<td>" . $row["dishname"] . "</td>";
                    echo "<td>" . $row["caloriecount"] . "</td>";
                    echo "<td>$" . number_format($row["price"], 2) . "</td>";
                    echo "<td>" . $row["veggie"] . "</td>";
                    echo "</tr>";
                }
            } else {
                echo "<tr><td colspan='5'>No menu items found.</td></tr>";
            }
            ?>
        </tbody>
    </table>
</main>

<?php include '../includes/footer.php'; ?>