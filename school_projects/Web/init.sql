DROP TABLE IF EXISTS commande_panier_detail CASCADE;
DROP TABLE IF EXISTS commande_panier CASCADE;
DROP TABLE IF EXISTS commande CASCADE;
DROP TABLE IF EXISTS panier_detail CASCADE;
DROP TABLE IF EXISTS panier CASCADE;
DROP TABLE IF EXISTS articles CASCADE;
DROP TABLE IF EXISTS type_articles CASCADE;
DROP TABLE IF EXISTS gerant CASCADE;
DROP TABLE IF EXISTS client CASCADE;
DROP TABLE IF EXISTS statut_commande CASCADE;
DROP TYPE IF EXISTS type_article;
DROP TYPE IF EXISTS taille;
DROP TYPE IF EXISTS couleur;
DROP TYPE IF EXISTS statut;

CREATE TABLE client (
  id_client INT PRIMARY KEY,
  nom_client VARCHAR(255),
  prenom_client VARCHAR(255),
  adresse_client VARCHAR(255),
  tel_client VARCHAR(20),
  mail_client VARCHAR(255)
);

CREATE TABLE gerant (
  id_gerant text PRIMARY KEY,
  mot_de_passe VARCHAR(255)
);

CREATE TYPE type_article AS ENUM (
    'pantalon', 'ceinture', 'bretelle', 'cravate', 'noeud papillon', 'pochette de costume', 'veste', 'pull', 'short', 'paire de chaussette', 'jupe', 'chemise', 'combinaison'
);

CREATE TABLE type_articles (
  id_type_article INT PRIMARY KEY,
  enum_article type_article NOT NULL
);

CREATE TYPE taille AS ENUM (
    'XS', 'S', 'M', 'L', 'XL'
);

CREATE TYPE couleur AS ENUM (
    'blanc', 'noir', 'gris', 'bleu', 'rouge', 'vert', 'jaune', 'orange', 'rose', 'violet', 'marron', 'beige'
);

CREATE TABLE articles (
  id_article INT PRIMARY KEY,
  id_type_article INT,
  designation VARCHAR(255),
  image_article VARCHAR(255), -- lien vers l'image
  image_zoom_article VARCHAR(255), -- lien vers l'image2 (style)  
  taille_article taille,
  couleur_article couleur,
  prix_article DECIMAL(10,2),
  stock_article INT,
  FOREIGN KEY (id_type_article) REFERENCES type_articles(id_type_article)
);

CREATE TABLE panier (
  numero_ligne INT,
  id_client INT,
  id_article INT,
  prix_article DECIMAL(10,2),
  quantite_panier INT,
  PRIMARY KEY (numero_ligne, id_client, id_article),
  FOREIGN KEY (id_client) REFERENCES client(id_client),
  FOREIGN KEY (id_article) REFERENCES articles(id_article)
);

CREATE TABLE panier_detail (
  numero_ligne INT,
  id_client INT,
  id_article INT,
  PRIMARY KEY (numero_ligne, id_client, id_article),
  FOREIGN KEY (id_client) REFERENCES client(id_client),
  FOREIGN KEY (id_article) REFERENCES articles(id_article)
);

CREATE TYPE statut AS ENUM (
    'cree', 'payee', 'preparee', 'expediee', 'recue', 'supprimee'
);

CREATE TABLE statut_commande (
  id_statut_commande INT PRIMARY KEY,
  enum_statut statut NOT NULL,
  enum_ordre INT
);

CREATE TABLE commande (
  id_commande SERIAL PRIMARY KEY ,
  nom_livraison VARCHAR(255),
  prenom_livraison VARCHAR(255),
  adresse_livraison VARCHAR(255),
  tel_livraison VARCHAR(20),
  mail_livraison VARCHAR(255),
  heure_livraison TIMESTAMP,
  id_statut_commande INT,
  montant_total DECIMAL(10,2),
  FOREIGN KEY (id_statut_commande) REFERENCES statut_commande(id_statut_commande)
);

CREATE TABLE commande_panier (
  id_commande INT,
  numero_ligne INT,
  id_article INT,
  prix_article_commande DECIMAL(10,2),
  quantite_panier_commande INT,
  PRIMARY KEY (id_commande, numero_ligne, id_article),
  FOREIGN KEY (id_commande) REFERENCES commande(id_commande),
  FOREIGN KEY (id_article) REFERENCES articles(id_article)
);

CREATE TABLE commande_panier_detail (
  id_commande INT,
  numero_ligne INT,
  id_article INT,
  PRIMARY KEY (id_commande, numero_ligne, id_article),
  FOREIGN KEY (id_commande) REFERENCES commande(id_commande),
  FOREIGN KEY (id_article) REFERENCES articles(id_article)
);

ALTER SEQUENCE commande_id_commande_seq RESTART WITH 1;

\copy client FROM 'csv/client.csv' WITH (FORMAT csv, DELIMITER ';');
\copy gerant FROM 'csv/gerant.csv' WITH (FORMAT csv, DELIMITER ';');
\copy type_articles FROM 'csv/type_articles.csv' WITH (FORMAT csv, DELIMITER ';');
\copy articles FROM 'csv/articles.csv' WITH (FORMAT csv, DELIMITER ';');
\copy statut_commande FROM 'csv/statut_commande.csv' WITH (FORMAT csv, DELIMITER ';');
\copy commande (nom_livraison, prenom_livraison, adresse_livraison, tel_livraison, mail_livraison, heure_livraison, id_statut_commande, montant_total)FROM 'csv/commande.csv' WITH (FORMAT csv, DELIMITER ';');