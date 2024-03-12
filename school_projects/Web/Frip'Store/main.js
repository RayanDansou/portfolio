
//on essaye de recuperer le panier de la session en cours ou sinon on part avec le panier vide
let cart = JSON.parse(sessionStorage.getItem("cart")) || [];

/* on defini un tableau pour les différentes couleurs*/ 
const colorNames = {
  noir: "#000000",
  blanc: "#FFFFFF",
  rouge: "#FF0000",
  vert: "#597535",
  bleu: "#2243B6",
  jaune: "#FFFF00",
  violet: "#CE9DD9",
  orange: "#FFA500",
  rose: "#FFC0CB",
  marron: "#582900",
  beige: "#C8AD7E",
  gris: "#808080",
};


// récupération des elements de la base de données
window.addEventListener('load', async () => {
  var access;
 

  try {
    const response = await fetch('http://localhost:3000/articles/combinaison');
    const articles = await response.json();
    const accessoires={ 
    "CombiLuxe": "Ceinture",
    "CombiBase": "aucun accessoire inclus!!",
    "CombiBusiness": "cravate + ceinture",
    };

    const selects={ 
      "CombiLuxe": "chemise,pantalon,accessoire",
      "CombiBase": "veste,chemise,pantalon",
      "CombiBusiness": "chemise,pantalon,veste",
      };

    displayCombi(articles,"Combinaisons",accessoires,selects);
  } catch (error) {
    console.error(error);
  }

  try {
    const response = await fetch('http://localhost:3000/articles/accessoire');
    const articles = await response.json();
    displayArticles(articles,"autres",'');
    access= articles.result;
    console.log("access:",access);
    articles.result.forEach((p) => {
      if(p.couleur_article === "noir" && p.designation==='cravate' || p.couleur_article === "noir" && p.designation==="Noeud'Pap" ){
        combiaccess("accessoire",p.designation)
      }
    })
  } catch (error) {
    console.error(error);
  }

  try {
    const response = await fetch('http://localhost:3000/articles/pantalon');
    const articles = await response.json();
    displayArticles(articles,"pantalons","Pant' ",access);
    articles.result.forEach((p) => {
      if(p.couleur_article === "noir" && p.designation==='costume'){
        combisizes("pantalon",p.tailles)
      }
    })

  } catch (error) {
    console.error(error);
  }

  try {
    const response = await fetch('http://localhost:3000/articles/chemise');
    const articles = await response.json();
    displayArticles(articles,"chemises","Chemise ",access);
    articles.result.forEach((p) => {
      if(p.couleur_article === "blanc" && p.designation==='coton'){
        combisizes("chemise",p.tailles)
      }
    })
  } catch (error) {
    console.error(error);
  }

  try {
    const response = await fetch('http://localhost:3000/articles/veste');
    const articles = await response.json();
    displayArticles(articles,"vestes","Veste ",access);
    articles.result.forEach((p) => {
      if(p.couleur_article === "noir" && p.designation==='costume'){
        combisizes("veste",p.tailles)
      }
    })
  } catch (error) {
    console.error(error);
  }

 
});


function combisizes(nom,tailles){/*fonction remplissant les tailles des combinaisons*/
  const selects = document.querySelectorAll('.Combinaisons .content #' + nom);
  const s = tailles.slice(1, -1).split(",");

  selects.forEach((select)=>{
    s.forEach((t)=>{
      const option = document.createElement("option");
      option.value=t;
      option.innerHTML = t;
      select.append(option);
    });
  });

}

function combiaccess(nom,acessoire){/*fonction remplissant les accessoires des combinaisons*/
  const select = document.querySelector('.Combinaisons .content #' + nom);
  const option = document.createElement("option");
  option.value=acessoire;
  option.innerHTML = acessoire;
  select.append(option);

}

function displayArticles(articles,nom,abbr,accesoires) {//cette fonction affcihe les articles sur la page 
    // récupérer la div où afficher les produits
  const section = document.getElementById(nom);

  const container = document.createElement("div");
  container.classList.add("box-container");
  

  // boucler sur les produits de tab1
  articles.result.forEach((product) => {
    // créer un élément de type "div" pour le produit
    const productDiv = document.createElement("div");
    productDiv.classList.add("box");
  
    const sizeSelect = document.createElement("select");

    // créer un élément "img" pour les images du produit
    const imageDiv = document.createElement("div");
    imageDiv.classList.add("image");
    const img1 = document.createElement("img");
    img1.src = product.image_article;
    img1.alt = product.designation;
    const img2 = document.createElement("img");
    img2.src = product.image_zoom_article;
    img2.alt = product.designation;
    img2.classList.add("rear-img");
    imageDiv.appendChild(img1);
    imageDiv.appendChild(img2);
    productDiv.appendChild(imageDiv);
  
    // ajouter le nom et le prix du produit
    const contentDiv = document.createElement("div");
    contentDiv.classList.add("content");
    const name = document.createElement("h3");
    name.textContent =abbr+ product.designation+' '+product.couleur_article;
    const price = document.createElement("div");
    price.classList.add("price");
    price.textContent = '$'+ product.prix_article;
    contentDiv.appendChild(name);
    contentDiv.appendChild(price);
  
    // créer les selects pour la couleur et la taille
    const colorSelect = document.createElement("select");
    colorSelect.id = "couleur";
    colorSelect.name = "couleur";
    colorSelect.value= product.couleur_article;
    console.log(colorSelect.value);
    colorSelect.style.backgroundColor=colorNames[product.couleur_article];
    if(product.couleur_article==='blanc'){
      colorSelect.style.color= colorNames['noir'];
    }
    const colorValues = new Set();
    articles.couleur.forEach((item) => {
      if (item.designation === product.designation) {
        const colors = item.couleurs.slice(1, -1).split(",");
        colors.forEach((color) => {
          colorValues.add(color);
        });
      }
    });

  // pour chaque couleur on ajoute une option
    const option = document.createElement("option");
    option.value = product.couleur_article;
    option.style.backgroundColor=colorNames[product.couleur_article];
    colorSelect.appendChild(option);
  
    colorValues.forEach((color) => {
      const option = document.createElement("option");
      option.value = color;
      option.style.backgroundColor=colorNames[color];
      if (color === product.couleur_article) {
      } else {
        colorSelect.appendChild(option);
      }
    });
   
  
    colorSelect.addEventListener('change', () => {// eventlistener pour changer aussi la photo et les infos quand on change de couleur sur l'article
      const selectedColor = colorSelect.options[colorSelect.selectedIndex].value;
      if(selectedColor==='blanc'){
        colorSelect.style.color= colorNames['noir'];
      }
      colorSelect.style.backgroundColor = colorNames[selectedColor];
      colorSelect.value= selectedColor;
    
      // on change l'image, le nom et les tailles disponibles pour le produit si la couleur change
        articles.result.forEach((p) => {
          if(p.couleur_article === selectedColor && p.designation===product.designation){

          img1.src = p.image_article;
          img2.src = p.image_zoom_article;
          price.textContent = '$'+ p.prix_article;
          name.textContent =abbr+ p.designation+' '+p.couleur_article;
            
          const s = p.tailles.slice(1, -1).split(",");
          sizeSelect.innerHTML = "";
          s.forEach((taille)=>{
          const option = document.createElement("option");
          option.value = taille;
          option.innerHTML = taille;
          sizeSelect.appendChild(option);
        });
          }
        });
    });
    contentDiv.appendChild(colorSelect);

  
    if(nom !== "autres"){// so on est entrain de chaerger des accessoires pas besoin des selects
    sizeSelect.id = "taille";
    sizeSelect.name = "taille";
    const sizeValues = new Set();
    const sizes = product.tailles.slice(1, -1).split(",");
    sizes.forEach((size) => {
      sizeValues.add(size);
    });
    sizeValues.forEach((size) => {
      const option = document.createElement("option");
      option.value = size;
      option.innerHTML = size;
      sizeSelect.appendChild(option);
    });
    contentDiv.appendChild(sizeSelect);
  }
  
    const br1 = document.createElement("br");
    const br2 = document.createElement("br");
    contentDiv.appendChild(br1);
    contentDiv.appendChild(br2);
  
    // ajouter le bouton "Ajouter au panier"
    const buyDiv = document.createElement("div");
    buyDiv.classList.add("buy");
    if(nom==="autres"){
      buyDiv.style.width='100%';
    }
    const buyButton = document.createElement("i");
    buyButton.classList.add("fas", "fa-shopping-cart");
    buyButton.addEventListener('click', () => {//lorsqu'on ajoute au panier pour certains éléments on a une fenetre popup avec des accessoires
      var name=product.designation;
      console.log(name);
      var price = product.prix_article;
      var size = sizeSelect.value;
      var color = colorSelect.value;
      console.log(color);
      addToCart(name,price,size,color,1);

      
      if(nom==="pantalons" && product.designation==='jean'){
        var n=accesoires.filter(function(e){
          return e.designation==="ceinture";
        });
        createPopup(n); 
    }
      if(nom==="pantalons" &&  product.designation==="costume"){
        var n=accesoires.filter(function(e){
          return e.designation==="ceinture" || e.designation==="Bretelles";
        });
         createPopup(n); 
        }
      if(product.designation==='coton'){ 
        var n=accesoires.filter(function(e){
          return e.designation==="cravate" || e.designation==="Noeud'Pap";
        });
        createPopup(n);}


    });
    if(nom !== "autres"){ buyDiv.appendChild(sizeSelect);}
    buyDiv.appendChild(buyButton);
    contentDiv.appendChild(buyDiv);
  
    // ajouter le contenu du produit à sa div
    productDiv.appendChild(contentDiv);
  
    // ajouter la div du produit au conteneur principal
    container.appendChild(productDiv);
    section.appendChild(container)
  });
      
}

function createPopup(accessoires){// cette fonction crée une fenetre popup juste apres l'ajout au panier
  const container = document.querySelector('body');


  console.log(accessoires);
  const popup=document.createElement("div");
  popup.classList.add("popup");
  const ov=document.createElement("div");
  ov.classList.add("overlay");
  popup.appendChild(ov);
  const co=document.createElement("div");
  co.classList.add("content");
  popup.appendChild(co);


  const titre=document.createElement("h2");
  titre.classList.add("heading");
  titre.innerHTML="Completez Votre ";
  const sp=document.createElement("span");
  sp.innerHTML="Style! ";
  titre.appendChild(sp);
  co.appendChild(titre);

  const c=document.createElement("div");
  c.classList.add("box-container");
  accessoires.forEach((product) => {
    // créer un élément de type "div" pour le produit
    const productDiv = document.createElement("div");
    productDiv.classList.add("box");
  
    // ajouter le nom et le prix du produit
    const contentDiv = document.createElement("div");
    contentDiv.classList.add("content");
    const name = document.createElement("h3");
    name.textContent =product.designation+' '+product.couleur_article;
    name.style.textAlign="center";
    const price = document.createElement("div");
    price.classList.add("price");
    price.textContent = '$'+ product.prix_article;
  
  
    const buyButton = document.createElement("i");
    buyButton.classList.add("fas", "fa-shopping-cart");
    buyButton.style.float="right";
    buyButton.addEventListener('click', () => {
      var name=product.designation;
      console.log(name);
      var price = product.prix_article;
      var color= product.couleur_article;
      var size='L';
      addToCart(name,price,size,color,1);
    });
    name.appendChild(buyButton);
    name.appendChild(price);
  
    // ajouter le contenu du produit à sa div
    productDiv.appendChild(name);
  
    // ajouter la div du produit au conteneur principal
    c.appendChild(productDiv);
    const br= document.createElement("br");
    c.appendChild(br);
    c.appendChild(br);

  });

  co.appendChild(c);
  const d=document.createElement("div");
  d.classList.add("buttons");
  co.appendChild(d);

  const n= document.createElement("button");
  n.classList.add("close-btn");
  n.innerHTML="Non merci";
  n.style.float="right";
  const n2= document.createElement("button");
  n2.classList.add("finish-btn");
  n2.innerHTML="Terminé";
  n2.style.float="left";
  n.addEventListener('click', () => {
    container.removeChild(popup);
  });
  n2.addEventListener('click', () => {
    container.removeChild(popup);
  });
  d.appendChild(n);
  d.appendChild(n2);
  container.appendChild(popup);

}
  

//fonction d'affichage de combinaisons
function displayCombi(articles,nomdiv,accesoires,selects){
  const section = document.querySelector('.' + nomdiv);

    const container = document.createElement("div");
    container.classList.add("box-container");
    section.appendChild(container);

    articles.forEach((product)=>{
      const productDiv = document.createElement("div");
      productDiv.classList.add("box");
    
      const imageDiv = document.createElement("div");
      imageDiv.classList.add("image");
      const img1 = document.createElement("img");
      img1.src = product.image_article;
      img1.alt = product.designation;
      imageDiv.appendChild(img1);
      productDiv.appendChild(imageDiv);

      const contentDiv = document.createElement("div");
      contentDiv.classList.add("content");
      const name = document.createElement("h3");
      name.textContent =product.designation;
      const price = document.createElement("div");
      price.classList.add("price");
      price.textContent = '$'+ product.prix_article;
      contentDiv.appendChild(name);
      contentDiv.appendChild(price);
      const br1 = document.createElement("br");
      contentDiv.appendChild(br1);
      const buyDiv = document.createElement("div");
      buyDiv.classList.add("buy");
      const br2 = document.createElement("br");
      buyDiv.appendChild(br2);
      console.log(selects[product.designation.replace("'","")].split(","));
      selects[product.designation.replace("'","")].split(",").forEach((p)=>{
        const label= document.createElement("label");
        label.className=p;
        label.innerHTML=p;
        const br = document.createElement("br");
        buyDiv.appendChild(label);
        buyDiv.appendChild(br);
        const sizeSelect = document.createElement("select");
        sizeSelect.id = p;
        sizeSelect.name = p;
        buyDiv.appendChild(sizeSelect);
        const br2 = document.createElement("br");
        buyDiv.appendChild(br2);
      })

      const br = document.createElement("br");
      buyDiv.appendChild(br);
      const br3 = document.createElement("br");
      buyDiv.appendChild(br3);
      const s = document.createElement("span");
      s.innerHTML="Acessoire inclus : <br>"+ accesoires[product.designation.replace("'","")];
      buyDiv.appendChild(s);
      const a = document.createElement("br");
      buyDiv.appendChild(a);
      const b = document.createElement("br");
      buyDiv.appendChild(b);
      const c = document.createElement("br");
      buyDiv.appendChild(c);
      const d = document.createElement("br");
      buyDiv.appendChild(d);


      const buyButton = document.createElement("i");
      buyButton.classList.add("fas", "fa-shopping-cart");
      buyButton.addEventListener('click', () => {
        var name = product.designation;
        console.log(name);
        var p = price.textContent.replace("$",'');
        var sizePantalon =  $(event.target).closest('.content').find('#pantalon').val();
        console.log(p);
        console.log(sizePantalon);

        var sizeChemise = $(event.target).closest('.content').find('#chemise').val();
        var sizeVeste =  $(event.target).closest('.content').find('#veste').val();
        var choixAccessoire;
        if (name==="Combi'Luxe"){
           choixAccessoire =  $(event.target).closest('.content').find('#accessoire').val();
        }
        addCombiToCart(name,p,sizePantalon,sizeChemise,sizeVeste,choixAccessoire,1);
      })
      buyDiv.appendChild(buyButton);
      contentDiv.appendChild(buyDiv);
      productDiv.appendChild(contentDiv);
      container.appendChild(productDiv);
    });
    console.log(section);

  }


//fonction de chagement du panier
function goToCart() {
    sessionStorage.setItem("cart", JSON.stringify(cart));
    // se rend sur la page du panier
    window.location.href = `Panier.html`;
}


// Fonction pour ajouter un produit au panier
function addToCart(productName, price,s,c, quantity) {
    console.log("ajout ici");
  // Recherchez si le produit existe déjà dans le panier
  let productIndex = -1;
  for (let i = 0; i < cart.length; i++) {
    if (cart[i].productName === productName && cart[i].price === price && cart[i].color === c && cart[i].size === s ) {
      productIndex = i;
      break;
    }
  }
  // Si le produit existe, mettez à jour la quantité
  if (productIndex !== -1) {
    cart[productIndex].quantity += quantity;
  }
  // Sinon, ajoutez le produit au panier
  else {
    cart.push({
      productName: productName,
      price: price,
      size: s,
      color: c,
      quantity: quantity
    });
  }
  displayCart();
}

// Fonction pour ajouter une combinaison au panier
function addCombiToCart(productName, price,sizePantalon,sizeChemise,sizeVeste,choixAcessoire, quantity) {
  console.log("ajout ici");
// Recherchez si le produit existe déjà dans le panier
let productIndex = -1;
for (let i = 0; i < cart.length; i++) {
  if (cart[i].productName === productName && cart[i].price === price && cart[i].pantalon === sizePantalon && cart[i].chemise === sizeChemise
     && cart[i].sizeVeste === sizeVeste && cart[i].accessoire === choixAcessoire) {
    productIndex = i;
    break;
  }
}
// Si le produit existe, mettez à jour la quantité
if (productIndex !== -1) {
  cart[productIndex].quantity += quantity;
}
// Sinon, ajoute le produit au panier
else {
  if( typeof choixAcessoire !== 'undefined') var access= choixAcessoire;
  cart.push({
    productName: productName,
    price: price,
    pantalon: sizePantalon,
    chemise: sizeChemise,
    veste: sizeVeste,
    accessoire: access,
    quantity: quantity
  });
}
displayCart();
}

// Fonction pour afficher le nombre de produits et le total du panier sur la page HTML
function displayCart() {
  let total = 0;
  let itemCount = 0;
  for (let i = 0; i < cart.length; i++) {
    total += cart[i].price * cart[i].quantity;
    console.log(cart[i].price);
    itemCount += cart[i].quantity;
  }
  console.log(itemCount);
  console.log(total);
  document.getElementById("cart-count").textContent = itemCount;
  document.getElementById("cart-total").innerHTML = total.toFixed(2)+ "$";
}


$(document).ready(function() {
    displayCart();
    let navbar = document.querySelector(".navbar");

    document.querySelector('#menu-bar').onclick = () =>{
        navbar.classList.toggle('active');
    }
    
    window.onscroll = () =>{
        navbar.classList.remove('active');
    }

    $('.header .icons .fa-shopping-cart').click(function(){
      goToCart();
    });
    
});

