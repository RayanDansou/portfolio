
-- renvoi la playlist avec le plus de morceau 
SELECT idPlaylist , COUNT(idM) AS totalM 
FROM Contient
GROUP BY idPlaylist 
HAVING COUNT(idM) = 
    ( SELECT MAX(nbMax) 
        FROM (SELECT COUNT(idM) AS nbMax 
              FROM contient 
              GROUP BY idPlaylist) AS PlaylistMax
    )
    LIMIT 1;

-- requete recursive : renvoi la liste de suivi a partir de l'id 13 
WITH recursive ListeSuivi AS 
    (
        SELECT idU1 FROM Amis WHERE idU1 = 13
        UNION 
        SELECT a.idU1, a.idU2 FROM Amis a, ListeSuivi l 
        WHERE a.idU1 = l.idU2 
    ) 
    SELECT idU2 FROM ListeSuivi;NON
    
-- renvoi le nombre d'avis par utilisateur en prennant les avis des tables Avis_concert, Avis_groupe, Avis_morceau et Avis_salle
SELECT idDonneur, COUNT(*) AS nombre_avis
    FROM Avis_concert c, Avis_groupe g, Avis_morceau m, Avis_salle s
    WHERE c.idDonneur = g.idDonneur AND c.idDonneur = m.idDonneur AND c.idDonneur = s.idDonneur
    GROUP BY idDonneur;

-- renvoi les genres musicaux qui ont comme genre_parent 'RAP'

SELECT

-- renvoi la liste des morceaux qui sont des morceaux de rap

SELECT Titre FROM Morceau
    NATURAL JOIN Appartient 
    NATURAL JOIN Genre_music
    WHERE Genre_music.Nom = 'Rap';

-- renvoi les morceaux possedant un genre musical qui n'a pas de genre parent

SELECT

-- renvoi la liste des concerts joué pars le groupe 14 où les enfants sont autorisés à participer 
SELECT * FROM Concert 
    JOIN Joue ON Joue.idC = Concert.idC
    WHERE Concert.isEnfantAllowed = TRUE 
    AND Joue.idU = 14;

-- renvoi le temps moyen de chaque playlist par rapport au temps des morceaux de la playlist
SELECT P.idPlaylist AS id, AVG(EXTRACT(EPOCH FROM M.DureeMorceau)) AS moyenne 
    FROM Playlist AS P
    JOIN Contient AS C ON C.idPlaylist = P.idPlaylist
    JOIN Morceau AS M ON C.idM = M.idM
    GROUP BY idPlaylist;NON
    
--