const express = require('express');
const cors = require('cors');
const { Pool } = require('pg');
const bodyParser = require('body-parser');
const app = express();
const port = process.env.PORT || 3000;
const path = require('path');
app.use(express.static(path.join(__dirname, "Frip'Store")));

app.use(cors());


// Configuration de la connexion à la base de données
const pool = new Pool({
  user: 'web',
  host: 'localhost',
  database: 'bdd',
  password: '12345',
  port: 5432,
});

// Middleware
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// Routes
app.get('/gerant.html', (req, res) => {
    res.sendFile(path.join(__dirname, "Frip'Store", 'gerant.html'));
});

app.post('/login_gerant', async (req, res) => {
    const { id_gerant, mot_de_passe } = req.body;

    try {
        const result = await pool.query(`
            SELECT * FROM gerant
            WHERE id_gerant = $1 AND mot_de_passe = $2
        `, [id_gerant, mot_de_passe]);

        if (result.rowCount === 0) {
            res.status(401).send('Identifiant ou mot de passe incorrect');
        } else {
            res.status(200).send('Connexion réussie');
        }
    } catch (error) {
        console.error(error);
        res.status(500).send('Erreur lors de la connexion');
    }
});

app.post('/update_stock', async (req, res) => {
    const { article_id, stock_change } = req.body;
  
    try {
      const result = await pool.query(`
        UPDATE articles
        SET stock_article = stock_article + $1
        WHERE id_article = $2 AND (stock_article + $1) >= 0
      `, [stock_change, article_id]);
  
      if (result.rowCount === 0) {
        res.status(400).send("L'opération n'a pas été effectuée car le stock ne peut pas être inférieur à 0 ou l'article est introuvable.");
      } else {
        res.status(200).send('Stock mis à jour');
      }
    } catch (error) {
      console.error(error);
      res.status(500).send('Erreur lors de la mise à jour du stock');
    }
  });

  app.get('/get_orders', async (req, res) => {
    try {
        const result = await pool.query(`
          SELECT commande.id_commande, heure_livraison, statut_commande.enum_statut
          FROM commande 
          INNER JOIN statut_commande ON commande.id_statut_commande = statut_commande.id_statut_commande
          ORDER BY heure_livraison DESC
        `);

        res.status(200).json(result.rows);
    } catch (error) {
        console.error(error);
        res.status(500).send("Erreur lors de la récupération des commandes");
    }
});

app.post('/delete_order', async (req, res) => {
  const { order_id } = req.body;
  try {
    // Obtenir l'ID du statut de commande "supprimée"
    const deletedStatusResult = await pool.query(`
        SELECT id_statut_commande FROM statut_commande
        WHERE enum_statut = 'supprimee'
    `);

    if (deletedStatusResult.rowCount === 0) {
        return res.status(400).json({ success: false, message: 'Le statut "supprimee" n\'existe pas' });
    }

    const deletedStatusId = deletedStatusResult.rows[0].id_statut_commande;

    // Mettre à jour la commande avec le statut "supprimee"
    const result = await pool.query(`
        UPDATE commande
        SET id_statut_commande = $1
        WHERE id_commande = $2 AND id_statut_commande != $1
    `, [deletedStatusId, order_id]);

    if (result.rowCount === 0) {
        res.status(404).json({ success: false, message: 'Commande introuvable ou déjà supprimee' });
    } else {
        res.status(200).json({ success: true, message: 'Commande marquee comme supprimee' });
    }
    } catch (error) {
    console.error(error);
    res.status(500).json({ success: false, message: 'Erreur lors de la mise a jour de la commande' });
    }
});

app.get('/article_info/:article_id', async (req, res) => {
    const { article_id } = req.params;
  
    try {
      const result = await pool.query(`
        SELECT * FROM articles
        WHERE id_article = $1
      `, [article_id]);
  
      if (result.rowCount === 0) {
        res.status(404).send('Article introuvable');
      } else {
        res.status(200).json(result.rows[0]);
      }
    } catch (error) {
      console.error(error);
      res.status(500).send('Erreur lors de la récupération des informations de l\'article');
    }
  });

// route pour ajouter une commande dans la bdd
  app.post('/ajouter_commande', (req, res) => {
    const commande = req.body;

    const val = [
      commande.Nom,
      commande.Prenom,
      commande.Adresse,
      commande.Telephone,
      commande.Adresse_email,
      commande.datelivraison,
      commande.statut,
      commande.total
    ];

    const sql = 'INSERT INTO commande (nom_livraison, prenom_livraison, adresse_livraison, tel_livraison, mail_livraison, heure_livraison, id_statut_commande, montant_total) VALUES ($1, $2, $3, $4, $5, $6, $7, $8)';

    pool.query(sql, val, (error) => {
      if (error) {
        console.error(error);
        res.sendStatus(500);
      } else {
        res.sendStatus(200);
      }
    });
  });



app.get('/articles/pantalon', async (req, res) => {
    try {
      const result= await pool.query(` SELECT designation, couleur_article, image_article, image_zoom_article,prix_article, ARRAY_AGG(taille_article) AS tailles 
      FROM articles 
      WHERE id_type_article = (SELECT id_type_article FROM type_articles WHERE enum_article = 'pantalon')
      GROUP BY designation, couleur_article, image_article, image_zoom_article, prix_article `);

      const couleur=await pool.query(` SELECT designation, ARRAY_AGG(DISTINCT couleur_article) AS couleurs
      FROM articles
      WHERE id_type_article=100
      GROUP BY designation `);

  
     res.status(200).json({result: result.rows, couleur: couleur.rows});

    } catch (error) {
      console.error(error);
      res.status(500).send('Erreur lors de la récupération des articles de type "pantalon"');
    }
});

app.get('/articles/chemise', async (req, res) => {
  try {
    const result= await pool.query(` SELECT designation, couleur_article, image_article, image_zoom_article,prix_article, ARRAY_AGG(taille_article) AS tailles 
    FROM articles 
    WHERE id_type_article = (SELECT id_type_article FROM type_articles WHERE enum_article = 'chemise')
    GROUP BY designation, couleur_article, image_article, image_zoom_article, prix_article `);

    const couleur=await pool.query(` SELECT designation, ARRAY_AGG(DISTINCT couleur_article) AS couleurs
    FROM articles
    WHERE id_type_article=210
    GROUP BY designation `);


   res.status(200).json({result: result.rows, couleur: couleur.rows});

  } catch (error) {
    console.error(error);
    res.status(500).send('Erreur lors de la récupération des articles de type "pantalon"');
  }
});


app.get('/articles/veste', async (req, res) => {
  try {
    const result= await pool.query(` SELECT designation, couleur_article, image_article, image_zoom_article,prix_article, ARRAY_AGG(taille_article) AS tailles 
    FROM articles 
    WHERE id_type_article = (SELECT id_type_article FROM type_articles WHERE enum_article = 'veste')
    GROUP BY designation, couleur_article, image_article, image_zoom_article, prix_article `);

    const couleur=await pool.query(` SELECT designation, ARRAY_AGG(DISTINCT couleur_article) AS couleurs
    FROM articles
    WHERE id_type_article=160
    GROUP BY designation `);


   res.status(200).json({result: result.rows, couleur: couleur.rows});

  } catch (error) {
    console.error(error);
    res.status(500).send('Erreur lors de la récupération des articles de type "pantalon"');
  }
});


app.get('/articles/accessoire', async (req, res) => {
  try {
    const result = await pool.query(`SELECT designation, couleur_article, image_article, image_zoom_article, prix_article, ARRAY_AGG(taille_article) AS tailles 
                                      FROM articles 
                                      WHERE id_type_article IN (SELECT id_type_article FROM type_articles WHERE enum_article IN ('ceinture', 'bretelle', 'cravate', 'noeud papillon', 'paire de chaussette'))
                                      GROUP BY designation, couleur_article, image_article, image_zoom_article, prix_article`);

    const couleur = await pool.query(`SELECT designation, ARRAY_AGG(DISTINCT couleur_article) AS couleurs
                                       FROM articles
                                       GROUP BY designation`);

    res.status(200).json({ result: result.rows, couleur: couleur.rows });
  } catch (error) {
    console.error(error);
    res.status(500).send('Erreur lors de la récupération des articles de type "accessoires"');
  }
});

app.get('/articles/combinaison', async (req, res) => {
  try {
    const result= await pool.query(` SELECT designation, couleur_article, image_article, image_zoom_article,prix_article
    FROM articles 
    WHERE id_type_article = (SELECT id_type_article FROM type_articles WHERE enum_article = 'combinaison')
    GROUP BY designation, couleur_article, image_article, image_zoom_article, prix_article `);

   res.status(200).json(result.rows);

  } catch (error) {
    console.error(error);
    res.status(500).send('Erreur lors de la récupération des articles de type "combinaison"');
  }
});


  
// Démarrage du serveur
app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
