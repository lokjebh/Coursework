/* * Project Name: Food Order Management System
 * File: assets/js/order_logic.js
 * Purpose: Handles UI logic for the order form, such as toggling 
 * delivery fields based on pickup selection.
 */

function toggleFields(isPickup) {
    const deliveryFields = document.getElementById('delivery_fields');
    const driverSection = document.getElementById('driver_section');
    const timeDel = document.getElementById('time_del');
    const rateVal = document.getElementById('rate_val');
    const driverSelect = document.getElementById('driver_select');

    if (isPickup) {
        // Hide delivery sections
        deliveryFields.style.display = 'none';
        driverSection.style.display = 'none';
        
        // Clear values so they don't get submitted accidentally
        timeDel.value = "";
        rateVal.value = "";
        driverSelect.value = "";
    } else {
        // Show delivery sections
        deliveryFields.style.display = 'block';
        driverSection.style.display = 'block';
    }
}