<hr>
    <footer>
        <p>&copy; 2026 Food Order Management System</p>
    </footer>
</body>
</html>
<?php

// Disconnect from the database at the end of the page
if (isset($conn)) {
    close_db($conn);
}
?>