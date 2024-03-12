$("#order-form").on("submit", function (event) {
    event.preventDefault();

    const email = $("input[name='Adresse_email']").val();
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    if (!emailRegex.test(email)) {
        alert("L'adresse email non valide");
        return;
    }

    // Vérification de la validité du numéro de téléphone
    const phone = $("input[name='Téléphone']").val();
    const phoneRegex = /^\d{10}$/;
    if (!phoneRegex.test(phone)) {
        alert("Le numéro de téléphone doit être composé de 10 chiffres");
        return;
    }
  
      // Récupérer les données du formulaire
      var nom=document.getElementsByName('Nom')[0].value;
      var prenom=document.getElementsByName('Prénom')[0].value;
      var adresse=document.getElementsByName('Adresse')[0].value;
      var tel=document.getElementsByName('Téléphone')[0].value;
      var mail=document.getElementsByName('Adresse_email')[0].value;

      //on calcule ensuite le montant total du panier
      const cart = JSON.parse(sessionStorage.getItem("cart")) || [];
      let total = 0;
      for (let i = 0; i < cart.length; i++) {
      total += cart[i].price * cart[i].quantity;
      }

      //on met atomtiquement la date de Livraison a une semaine apres la commande
      var dateActuelle = new Date();
      dateActuelle.setDate(dateActuelle.getDate() + 7);
      var dateLivraison = dateActuelle.toISOString().slice(0, 19);
    
      // les informations sont mises dans le format JSON pour l'ajout dans la bdd
      var donneesFormulaire = {
        "Nom": nom,
        "Prenom": prenom,
        "Adresse": adresse,
        "Telephone": tel,
        "Adresse_email": mail,
        "datelivraison":dateLivraison,
        "statut": 2,
        "total" : total.toFixed(2)
      };

      const formData = JSON.stringify(donneesFormulaire);

    // ajout de commande
    fetch('http://localhost:3000/ajouter_commande', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: formData
    })
    .then(response => {
      if (response.ok) {
        // La commande a été ajoutée avec succès à la base de données
        var annee = dateActuelle.getFullYear();
        var mois = dateActuelle.getMonth() + 1; 
        var jour = dateActuelle.getDate();
        var dateEstimée = `${jour < 10 ? '0' + jour : jour}-${mois < 10 ? '0' + mois : mois}-${annee}`;
        alert("Commande ajoutée avec succès ! Livraison estimée: "+dateEstimée);
      } else {
        // si erreur
        alert("Une erreur s'est produite lors de l'ajout de la commande.");
      }
    })
    .catch(error => {
      console.error("Erreur :", error);
      alert("Une erreur s'est produite lors de l'ajout de la commande.");
    });

    sessionStorage.removeItem("cart");
    $("#order-form")[0].reset();
  });
  
  