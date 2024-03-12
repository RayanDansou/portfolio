
-- Demande à l'utilisateur de saisir l'ID du groupe
\prompt 'Liste des concerts joués par un groupe où les enfants sont autorisés\n Entrez l\'ID du groupe : ' v_id_groupe

-- Requête : Liste des concerts joués par un groupe où les enfants sont autorisés à participer
SELECT *
FROM Concert
JOIN Joue ON Joue.idC = Concert.idC
WHERE Concert.isEnfantAllowed = TRUE
AND Joue.idU = :v_id_groupe;



-- Demande à l'utilisateur de saisir une durée minimale
\prompt '\nPlaylists avec un son dau moins n minutes\n Entrez la durée minimale (format : HH:MM:SS) : ' v_duree

-- Requête : Playlists avec un son d'au moins n minutes
SELECT DISTINCT P.Nom
FROM Playlist P
INNER JOIN Contient C ON P.idP = C.idPlaylist
INNER JOIN Morceau M ON C.idM = M.idM
WHERE M.DureeMorceau > :'v_duree' ;


--Demande a l'utilisateur de saisi un Genre
\prompt '\nListe des Morceaux avec un genre g\n Entrez le genre souhaité : ' v_genre
-- Requète : Morceaux de genre v_genre
SELECT M.Titre
FROM Morceau M
INNER JOIN Appartient A ON M.idM = A.idM
INNER JOIN Genre_music G ON A.idGenre = G.idGenre
WHERE G.Nom = :'v_genre' ;


--Demande a L'utilisateur de saisir un Nom de Playlist
\prompt '\ntemps moyen de la playlist p g\n Entrez la playlist souhaitée : ' v_playlist

-- Requète : temps moyen de la playlist de nom v_playlist
SELECT P.Nom, 
       AVG(EXTRACT(HOUR FROM M.DureeMorceau) * 60 + EXTRACT(MINUTE FROM M.DureeMorceau) + EXTRACT(SECOND FROM M.DureeMorceau) / 60) AS TempsMoyenEnMinutes
FROM Playlist P
INNER JOIN Contient C ON P.idP = C.idPlaylist
INNER JOIN Morceau M ON C.idM = M.idM
WHERE P.Nom= :'v_playlist'
GROUP BY P.idP, P.Nom;



-- Requête : Les utilisateurs qui suivent un groupe qui a créé une playlist
SELECT DISTINCT u.Nom
FROM Utilisateur u
JOIN Follow f ON u.idU = f.idU1
JOIN Groupes g ON f.idU2 = g.idU
JOIN Playlist p ON g.idU = p.idU;


-- Requête : Les noms des utilisateurs qui suivent au moins un autre utilisateur
SELECT Nom
FROM Utilisateur u
WHERE EXISTS (
    SELECT *
    FROM Follow f
    WHERE f.idU2 = u.idU
);



-- Requête : Tous les utilisateurs qui ont une playlist donnée
SELECT Nom
FROM Utilisateur
WHERE idU IN (
    SELECT idU
    FROM Playlist
    );

-- Requête : Playlist avec le plus de morceaux
SELECT idPlaylist, COUNT(idM) AS totalM
FROM Contient
GROUP BY idPlaylist
HAVING COUNT(idM) = (
    SELECT MAX(nbMax)
    FROM (
        SELECT COUNT(idM) AS nbMax
        FROM contient
        GROUP BY idPlaylist
    ) AS PlaylistMax
)
LIMIT 1;



-- Requête : Nombre de morceaux par playlist
SELECT p.idP, p.Nom, COUNT(c.idM) AS nombre_de_morceaux
FROM Playlist p
LEFT JOIN Contient c ON p.idP = c.idPlaylist
GROUP BY p.idP, p.Nom;


-- Requête : Groupes qui n'ont pas donné de concert en 2022 (avec des sous-requêtes corrélées)
SELECT u.Nom
FROM Utilisateur u NATURAL JOIN Groupes g
WHERE NOT EXISTS (
    SELECT *
    FROM Concert c
    NATURAL JOIN Joue o
    WHERE o.idU = g.idU
    AND c.dateC >= '2022-01-01'::DATE
    AND c.dateC < '2023-01-01'::DATE
);

-- Requête : Groupes qui n'ont pas donné de concert en 2022 (avec de l'agrégation)
SELECT u.Nom
FROM Utilisateur u
NATURAL JOIN Groupes g
LEFT JOIN Joue j ON g.idU = j.idU
LEFT JOIN Concert c ON j.idC = c.idC AND c.dateC >= '2022-01-01'::DATE AND c.dateC < '2023-01-01'::DATE
GROUP BY u.Nom
HAVING COUNT(c.idC) = 0;


--Requete: Nombre d'avis donné par chacun
SELECT U.idU, U.Nom, COUNT(*) AS NombreAvis
FROM Utilisateur U
LEFT JOIN Avis_salle ASv ON U.idU = ASv.idDonneur
LEFT JOIN Avis_groupe AG ON U.idU = AG.idDonneur
LEFT JOIN Avis_morceau AM ON U.idU = AM.idDonneur
LEFT JOIN Avis_concert AC ON U.idU = AC.idDonneur
GROUP BY U.idU, U.Nom;


--Requete: morceaux appartenant a un genre qi n'a pas de genre_parent
SELECT Titre FROM Morceau
   NATURAL JOIN Appartient
   NATURAL JOIN Genre_music
   WHERE Genre_music.Genre_parent IS NULL ;



------Requetes pour l'indice de Recommandation


-- Demande à l'utilisateur  l'id d'une personne
\prompt 'Liste des concerts joués dans le pays ou habite l\'utilisateur u\n Entrez l\'ID de l\'utilisateur : ' v_idU

--Requete: la liste des concerts dans le pays de l'utilisateur d'id v_idU
SELECT C.Nom, C.dateC
FROM Concert C
JOIN Salle_concert S ON C.idSalle = S.idSalle
JOIN Utilisateur U ON U.idU = :'v_idU'
WHERE S.Pays = U.Pays AND C.statut='Future';


\prompt 'Liste des concerts auquels participent les amis de  l\'utilisateur u\n Entrez l\'ID de l\'utilisateur : ' v_id

SELECT DISTINCT C.Nom
FROM Concert C
JOIN Prochain_concert PC ON C.idC = PC.idC
JOIN Participation P ON PC.idC = P.idC
JOIN Personnes Pe ON P.idU = Pe.idU
JOIN Follow F ON Pe.idU = F.idU2
WHERE F.idU1 = :'v_id';

