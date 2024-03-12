### projet_PW réalisé par Yao Rayan DANSOU et Jade CORTIAL

#### Ce processus d'installation à faire dans le terminal nous permet d'obtenir toutes les extensions requises pour démarrer notre projet :

(-) npm intall

(-) npm intall cors

(-) npm intall express

(-) npm intall pg

(-) sudo -s -u postgres

(-) createuser -d -P web

(-) entrer le mdp '12345'

(-) createdb -O web bdd 

#### Après avoir procédé à ces installation il faut mettre à jour notre base de données :

(-) psql -U web -h localhost bdd -f init.sql (le mot de passe est celui crée plus tôt '12345')

#### Puis lancer le serveur node :

(-) node app.js (il est indiqué normalement "server is running on port 3000" dans le terminal)

#### Pour avoir accès à la page du gérant :

(-) ouvrez une nouvelle page dans le navigateur de votre choix et allez à cette adresse :
http://localhost:3000/gerant.html 

#### Vous pourrez retrouver les identifiants et login pour vous connecter à la page du gérant dans le fichier 'gerant.csv'.