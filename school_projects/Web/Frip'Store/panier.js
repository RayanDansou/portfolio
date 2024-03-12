function addHiddenFieldsToCheckoutForm(cartItems) {
	const form = document.getElementById("checkout-form");

	cartItems.forEach((item, index) => {
		const idInput = document.createElement("input");
		idInput.type = "hidden";
		idInput.name = "id_article[]";
		idInput.value = item.id;

		const quantityInput = document.createElement("input");
		quantityInput.type = "hidden";
		quantityInput.name = "quantite[]";
		quantityInput.value = item.quantity;

		form.appendChild(idInput);
		form.appendChild(quantityInput);
	});
}

function displayCartItems() {
	let cartItems = JSON.parse(sessionStorage.getItem("cart")) || [];
	console.log("Cart items:", cartItems);

	let t=getCartTotal(cartItems);

	let total= document.createElement("span");
	total.style.fontSize="15px";
	total.textContent='Montant total du panier: '+t.toFixed(2)+'$';
	console.log( total);

	let cartTotal = document.getElementById("panier-total");
	cartTotal.innerHTML="";
	cartTotal.appendChild(total);


	let btncommande = document.getElementById("commanderbtn");

	let cartList = document.getElementById("panier-list");
	cartList.innerHTML = ""; // Vider la liste avant de la remplir à nouveau

	if (cartItems.length === 0) {
		// si le panier est vide
		cartList.innerHTML = "Votre panier est vide";
		btncommande.href="";
		return;
	}
	btncommande.href="commande.html";

	cartItems.forEach((item, index) => {
		let listItem = document.createElement("div");
		var tt = `${item.productName}`;
		var price = `${item.price}` * `${item.quantity}`;
		price=price.toFixed(2);
		if (tt === "Combi'Business") {
			listItem.innerHTML = `<strong>${item.productName}</strong> (x${item.quantity})<br>
		 veste: ${item.veste} | chemise: ${item.chemise} | pantalon:${item.pantalon} 
		 <br>
		 Accessoires: Ceinture + Cravate
		 <br>
		 ${price}$`;

		} else if (tt === "Combi'Base") {
			listItem.innerHTML = `<strong>${item.productName}</strong> (x${item.quantity})<br>
		veste: ${item.veste} | chemise: ${item.chemise} | pantalon:${item.pantalon} 
		<br>
		${price}$ `;
		}
		else if (tt === "Combi'Luxe") {
			listItem.innerHTML = `<strong>${item.productName}</strong> (x${item.quantity})<br>
		  chemise: ${item.chemise} | pantalon:${item.pantalon} 
		 <br>
		 Accessoires: Ceinture + ${item.accessoire}
		 <br>
		 ${price}$ `;

		} else {
			listItem.innerHTML = `<strong>${item.productName}</strong>(x${item.quantity})<br>
		Taille: ${item.size}<br>Couleur: ${item.color}<br>  ${price}$`;
		}


		// Créer un bouton de suppression
		let removeButton = document.createElement("i");
		removeButton.classList.add("fa", "fa-trash-o");
		removeButton.classList.add("remove-button");
		removeButton.addEventListener("click", () => removeItemFromCart(index));

		// Ajouter le bouton de suppression à l'élément de liste
		listItem.appendChild(removeButton);
		cartList.appendChild(listItem);
		// Créer les boutons "+" et "-"
		let moins = document.createElement("button");
		moins.classList.add("quantity-button", "minus");
		moins.classList.add("moins");
		moins.setAttribute("data-index", index);
		moins.innerHTML = "-";

		let plus = document.createElement("button");
		plus.classList.add("quantity-button", "plus");
		plus.classList.add("plus");
		plus.setAttribute("data-index", index);
		plus.innerHTML = "+";

		// Ajouter les écouteurs d'événements pour les boutons "+" et "-"
		moins.addEventListener("click", () => diminuer(index));
		plus.addEventListener("click", () => augmenter(index));

		// Ajouter les boutons à l'élément de liste
		listItem.appendChild(moins);
		listItem.appendChild(plus);
		listItem.appendChild(document.createElement("br"));
		listItem.appendChild(document.createElement("br"));
	});



	addHiddenFieldsToCheckoutForm(cartItems);

	$("#checkout-form").on("submit", function (event) {
		event.preventDefault();
		const formData = new FormData(this);
		const data = new URLSearchParams();

		for (const pair of formData) {
			data.append(pair[0], pair[1]);
		}

		sessionStorage.setItem("checkoutData", data.toString());

		// Générer et afficher le message de confirmation
		const confirmationMessage = generateOrderConfirmationMessage(cartItems);
		alert(confirmationMessage);

		window.location.href = "commande.html";
	});
}

// Fonction pour diminuer la quantité
function diminuer(index) {
	let cartItems = JSON.parse(sessionStorage.getItem("cart")) || [];
	if (cartItems[index].quantity > 1) {
		cartItems[index].quantity--;
		sessionStorage.setItem("cart", JSON.stringify(cartItems));
		displayCartItems(); // Mettre à jour l'affichage
	}
}

// Fonction pour augmenter la quantité
function augmenter(index) {
	let cartItems = JSON.parse(sessionStorage.getItem("cart")) || [];
	cartItems[index].quantity++;
	sessionStorage.setItem("cart", JSON.stringify(cartItems));
	displayCartItems(); // Mettre à jour l'affichage
}

// Supprimer un élément du panier
function removeItemFromCart(index) {
	let cart = JSON.parse(sessionStorage.getItem("cart")) || [];
	cart.splice(index, 1); // supprimer l'élément d'index
	sessionStorage.setItem("cart", JSON.stringify(cart));
	displayCartItems(); // rafraîchir la liste des éléments du panier
}

function getCartTotal(cart){
	var res=0;
	cart.forEach((item, index)=>{
		res+= `${item.quantity}` * `${item.price}`;
		});
		return res;
}

// Afficher les éléments du panier au chargement de la page
//document.addEventListener("DOMContentLoaded", displayCartItems);
displayCartItems();
