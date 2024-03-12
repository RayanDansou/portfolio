$(document).ready(function() {
    $("#update-stock-form").on("submit", function(event) {
        event.preventDefault();

        const articleId = $("#article_id").val();
        const stockChange = $("#stock_change").val();

        $.ajax({
            type: "POST",
            url: "http://localhost:3000/update_stock",
            data: {
                article_id: articleId,
                stock_change: stockChange
            },
            success: function(response) {
                alert("Le stock a été mis à jour.");
            },
            error: function(error) {
                alert("Une erreur s'est produite lors de la mise à jour du stock.");
            }
        });
    });

    $("#get-article-info").on("click", function() {
        const articleId = $("#article_id").val();
    
        $.ajax({
            type: "GET",
            url: `http://localhost:3000/article_info/${articleId}`,
            success: function(response) {
                const info = `Article ID: ${response.id_article}, Designation: ${response.designation}, Stock: ${response.stock_article}`;
                $("#article-info").text(info);
            },
            error: function(error) {
                alert("Une erreur s'est produite lors de la récupération des informations de l'article.");
            }
        });
    });

    function loadOrders() {
        $.ajax({
            type: "GET",
            url: "http://localhost:3000/get_orders",
            success: function(response) {
                const orders = response;
                const ordersTable = $("#orders-table tbody");
    
                ordersTable.empty();
                orders.forEach(order => {
                    const row = $("<tr></tr>");
                    row.append(`<td>${order.id_commande}</td>`);
                    row.append(`<td>${new Date(order.heure_livraison).toLocaleString()}</td>`);
                    row.append(`<td>${order.enum_statut}</td>`);
                    
                    const deleteButton = $("<button>Supprimer</button>");
                    if (order.enum_statut === "recue") {
                        deleteButton.click(() => deleteOrder(order.id_commande));
                    } else {
                        deleteButton.attr("disabled", true);
                    }
                
                    const buttonCell = $("<td></td>");
                    buttonCell.append(deleteButton);
                
                    row.append(buttonCell);
                    ordersTable.append(row);
                });
            },
            error: function(error) {
                alert("Une erreur s'est produite lors de la récupération des commandes.");
            }
        });
    }

    function deleteOrder(orderId) {
        $.ajax({
            type: "POST",
            url: "http://localhost:3000/delete_order",
            data: {
                order_id: orderId
            },
            success: function(response) {
                alert("La commande a été marquée comme supprimée.");
                loadOrders(); // Recharge la liste des commandes après la suppression
            },
            error: function(error) {
                alert("Une erreur s'est produite lors de la suppression de la commande.");
            }
        });
    }
    
    $("#load-orders").on("click", function() {
        loadOrders();
    });
});
