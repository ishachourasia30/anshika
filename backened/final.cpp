
#include "httplib.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

using namespace httplib;

struct Product {
    int id;
    string name;
    float price;
    int quantity;
};

struct Order {
    int orderId;
    int productId;
    int quantity;
    string status;
};

Product products[100];
Order orders[100];

int productCount = 0;
int orderCount = 0;
int nextOrderId = 1001;

// View all products
string getProducts() {
    string result = "";

    for (int i = 0; i < productCount; i++) {
        result += to_string(products[i].id) + "|";
        result += products[i].name + "|";
        result += to_string(products[i].price) + "|";
        result += to_string(products[i].quantity) + "\n";
    }

    return result;
}

// View all orders
string getOrders() {
    string result = "";

    for (int i = 0; i < orderCount; i++) {
        result += to_string(orders[i].orderId) + "|";
        result += to_string(orders[i].productId) + "|";
        result += to_string(orders[i].quantity) + "|";
        result += orders[i].status + "\n";
    }

    return result;
}

int main() {

    Server server;

    // Show website
    server.set_mount_point("/", "./public");

    // View products
    server.Get("/api/products", [](const Request& req, Response& res) {
        res.set_content(getProducts(), "text/plain");
    });

    // Add product
    server.Post("/api/add", [](const Request& req, Response& res) {

        if (!req.has_param("id") ||
            !req.has_param("name") ||
            !req.has_param("price") ||
            !req.has_param("quantity")) {
            res.status = 400;
            res.set_content("Missing product details", "text/plain");
            return;
        }

        try {
            int id = stoi(req.get_param_value("id"));
            string name = req.get_param_value("name");
            float price = stof(req.get_param_value("price"));
            int quantity = stoi(req.get_param_value("quantity"));

            if (id <= 0 || name.empty() || price <= 0 ||
                quantity < 0 || productCount >= 100) {
                res.status = 400;
                res.set_content("Invalid product details", "text/plain");
                return;
            }

            for (int i = 0; i < productCount; i++) {
                if (products[i].id == id) {
                    res.status = 400;
                    res.set_content("Product ID already exists", "text/plain");
                    return;
                }
            }

            products[productCount] = {id, name, price, quantity};
            productCount++;

            res.set_content("Product added successfully!", "text/plain");

        } catch (...) {
            res.status = 400;
            res.set_content("Please enter valid details", "text/plain");
        }
    });

    // Update quantity
    server.Post("/api/update", [](const Request& req, Response& res) {

        if (!req.has_param("id") || !req.has_param("quantity")) {
            res.status = 400;
            res.set_content("Missing details", "text/plain");
            return;
        }

        try {
            int id = stoi(req.get_param_value("id"));
            int quantity = stoi(req.get_param_value("quantity"));

            if (quantity < 0) {
                res.status = 400;
                res.set_content("Invalid quantity", "text/plain");
                return;
            }

            for (int i = 0; i < productCount; i++) {
                if (products[i].id == id) {
                    products[i].quantity = quantity;
                    res.set_content("Quantity updated!", "text/plain");
                    return;
                }
            }

            res.status = 404;
            res.set_content("Product not found", "text/plain");

        } catch (...) {
            res.status = 400;
            res.set_content("Invalid details", "text/plain");
        }
    });

    // Place order
    server.Post("/api/order", [](const Request& req, Response& res) {

        if (!req.has_param("id") || !req.has_param("quantity")) {
            res.status = 400;
            res.set_content("Missing order details", "text/plain");
            return;
        }

        try {
            int id = stoi(req.get_param_value("id"));
            int quantity = stoi(req.get_param_value("quantity"));

            if (quantity <= 0 || orderCount >= 100) {
                res.status = 400;
                res.set_content("Invalid order quantity", "text/plain");
                return;
            }

            for (int i = 0; i < productCount; i++) {

                if (products[i].id == id) {

                    if (products[i].quantity < quantity) {
                        res.status = 400;
                        res.set_content("Not enough stock!", "text/plain");
                        return;
                    }

                    products[i].quantity -= quantity;

                    orders[orderCount] = {
                        nextOrderId, id, quantity, "Placed"
                    };

                    orderCount++;

                    res.set_content(
                        "Order placed! Order ID: " +
                        to_string(nextOrderId),
                        "text/plain"
                    );

                    nextOrderId++;
                    return;
                }
            }

            res.status = 404;
            res.set_content("Product not found", "text/plain");

        } catch (...) {
            res.status = 400;
            res.set_content("Invalid order details", "text/plain");
        }
    });

    // Track orders
    server.Get("/api/orders", [](const Request& req, Response& res) {
        res.set_content(getOrders(), "text/plain");
    });

    // Start server
    const char* portValue = getenv("PORT");
    int port = portValue ? stoi(portValue) : 8080;

    cout << "Inventory Server Started!" << endl;

    server.listen("0.0.0.0", port);

    return 0;
}
