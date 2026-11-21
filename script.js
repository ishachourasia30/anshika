
const message = document.getElementById("message");

// Show message
function showMessage(text) {
    message.innerText = text;
}

// Add product
document.getElementById("addForm").addEventListener("submit", async function(e) {
    e.preventDefault();

    const data = new URLSearchParams({
        id: document.getElementById("productId").value,
        name: document.getElementById("productName").value,
        price: document.getElementById("price").value,
        quantity: document.getElementById("quantity").value
    });

    const response = await fetch("/api/add", {
        method: "POST",
        body: data
    });

    showMessage(await response.text());

    if (response.ok) {
        this.reset();
        loadProducts();
    }
});

// Update quantity
document.getElementById("updateForm").addEventListener("submit", async function(e) {
    e.preventDefault();

    const data = new URLSearchParams({
        id: document.getElementById("updateId").value,
        quantity: document.getElementById("newQuantity").value
    });

    const response = await fetch("/api/update", {
        method: "POST",
        body: data
    });

    showMessage(await response.text());

    if (response.ok) {
        this.reset();
        loadProducts();
    }
});

// Place order
document.getElementById("orderForm").addEventListener("submit", async function(e) {
    e.preventDefault();

    const data = new URLSearchParams({
        id: document.getElementById("orderProductId").value,
        quantity: document.getElementById("orderQuantity").value
    });

    const response = await fetch("/api/order", {
        method: "POST",
        body: data
    });

    showMessage(await response.text());

    if (response.ok) {
        this.reset();
        loadProducts();
        loadOrders();
    }
});

// Load products
async function loadProducts() {
    const response = await fetch("/api/products");
    const result = await response.text();

    let html = "<table><tr><th>ID</th><th>Name</th><th>Price</th><th>Quantity</th></tr>";

    if (result.trim() === "") {
        html += "<tr><td colspan='4'>No products available</td></tr>";
    } else {
        result.trim().split("\n").forEach(item => {
            const p = item.split("|");

            html += `<tr>
                <td>${p[0]}</td>
                <td>${p[1]}</td>
                <td>${Number(p[2]).toFixed(2)}</td>
                <td>${p[3]}</td>
            </tr>`;
        });
    }

    html += "</table>";

    document.getElementById("productList").innerHTML = html;
}

// Load orders
async function loadOrders() {
    const response = await fetch("/api/orders");
    const result = await response.text();

    let html = "<table><tr><th>Order ID</th><th>Product ID</th><th>Quantity</th><th>Status</th></tr>";

    if (result.trim() === "") {
        html += "<tr><td colspan='4'>No orders available</td></tr>";
    } else {
        result.trim().split("\n").forEach(item => {
            const o = item.split("|");

            html += `<tr>
                <td>${o[0]}</td>
                <td>${o[1]}</td>
                <td>${o[2]}</td>
                <td>${o[3]}</td>
            </tr>`;
        });
    }

    html += "</table>";

    document.getElementById("orderList").innerHTML = html;
}

// Load data when website opens
loadProducts();
loadOrders();
