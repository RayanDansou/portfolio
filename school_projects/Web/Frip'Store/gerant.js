$(document).ready(function() {
    $("#gerant-login-form").on("submit", function(event) {
        event.preventDefault();

        const idGerant = $("#id_gerant").val();
        const motDePasse = $("#mot_de_passe").val();

        $.ajax({
            type: "POST",
            url: "http://localhost:3000/login_gerant",
            data: {
                id_gerant: idGerant,
                mot_de_passe: motDePasse
            },
            success: function(response) {
                alert("Connexion réussie.");
                window.location.href = "gerant_dashboard.html"; // Rediriger vers la page du tableau de bord
            },
            error: function(error) {
                alert("Identifiant ou mot de passe incorrect.");
            }
        });
    });
});
