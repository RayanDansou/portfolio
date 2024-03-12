
Il est maintenant temps d’utiliser votre base de données pour faire vivre votre réseau social. Imaginez
20 questions sur la base de données que vous avez modélisée, et écrivez des requêtes SQL permettant d’y
répondre. L’originalité des questions et la difficulté des requêtes (si tant est que celle-ci soit nécessaire)
seront prises en compte dans la notation. Parmi vos requêtes, il faut au minimum :
— une requête qui porte sur au moins trois tables ;
1.Les Utilisateurs qui suivents un groupe qui a créé une Playlist
SELECT DISTINCT u.Nom
FROM Utilisateur u
JOIN Follow f ON u.idU = f.idU1
JOIN Groupes g ON f.idU2 = g.idU
JOIN Playlist p ON g.idU = p.idU;

— une ’auto jointure’ ou ’jointure réflexive’ (jointure de deux copies d’une même table)

— une sous-requête corrélée ;
3.Quels sont les noms des utilisateurs qui suivent au moins un autre utilisateur ?
SELECT Nom
FROM Utilisateur u
WHERE EXISTS (
    SELECT *
    FROM Follow f
    WHERE f.idU2 = u.idU
);

— une sous-requête dans le FROM ;
Playslist avec un son de au moins 4 minutes
SELECT DISTINCT P.Nom
FROM Playlist P
INNER JOIN Contient C ON P.idP = C.idPlaylist
INNER JOIN Morceau M ON C.idM = M.idM
WHERE M.DureeMorceau > '00:04:00';

— une sous-requête dans le WHERE ;
tous les utilisateurs qui ont une Playlist party mix
SELECT Nom
FROM Utilisateur
WHERE idU IN (
    SELECT idU
    FROM Playlist
    WHERE Nom = 'Party Mix'
);


— deux agrégats nécessitant GROUP BY et HAVING ;
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




— une requête impliquant le calcul de deux agrégats (par exemple, les moyennes d’un ensemble de
maximums) ;



— une jointure externe (LEFT JOIN, RIGHT JOIN ou FULL JOIN) ;
Nombre de Morceaux par Playlist
SELECT p.idP, p.Nom, COUNT(c.idM) AS nombre_de_morceaux
FROM Playlist p
LEFT JOIN Contient c ON p.idP = c.idPlaylist
GROUP BY p.idP, p.Nom;


— deux requêtes équivalentes exprimant une condition de totalité, l’une avec des sous requêtes corrélées et l’autre avec de l’agrégation ;

-- Requête avec des sous-requêtes corrélées
Nom des groupe qui ont pas donné de concert en 2022
SELECT u.Nom
FROM Utilisateur u NATURAL JOIN Groupes g
WHERE NOT EXISTS (
    SELECT *
    FROM Concert c
    NATURAL JOIN Joue o
    WHERE o.idU = g.idU
    AND c.DateArchive >= '2022-01-01'::DATE
    AND c.DateArchive < '2023-01-01'::DATE
)

-- Requête avec de l'agrégation
SELECT u.Nom
FROM Utilisateur u
NATURAL JOIN Groupes g
LEFT JOIN Joue j ON g.idU = j.idU
LEFT JOIN Concert c ON j.idC = c.idC AND c.DateArchive >= '2022-01-01'::DATE AND c.DateArchive < '2023-01-01'::DATE
GROUP BY u.Nom
HAVING COUNT(c.idC) = 0;

— deux requêtes qui renverraient le même résultat si vos tables ne contenaient pas de nulls, mais
qui renvoient des résultats différents ici (vos données devront donc contenir quelques nulls), vous
proposerez également de petites modifications de vos requêtes (dans l’esprit de ce qui sera présenté
dans le cours sur l’information incomplète) afin qu’elles retournent le même résultat ;

— une requête récursive (par exemple, une requête permettant de calculer quel est le prochain jour
off d’un groupe actuellement en tournée) ;
Exemple : Napalm Death est actuellement en tournée (Campagne for Musical Destruction 2023),
ils jouent sans interruption du 28/02 au 05/03, mais ils ont un jour off le 06/03 entre Utrecht
(05/03) et Bristol (07/03). En supposant qu’on est aujourd’hui le 28/02, je souhaite connaître leur
prochain jour off, qui est donc le 06/03.



— une requête utilisant du fenêtrage (par exemple, pour chaque mois de 2022, les dix groupes dont
les concerts ont eu le plus de succès ce mois-ci, en termes de nombre d’utilisateurs ayant indiqué
souhaiter y participer).

--Les concerts qui ont cumulé le plus de participations pour l'année 2022
WITH participants_cumules AS (
  SELECT g.idU, c.Nom, c.DateArchive, SUM(COUNT(DISTINCT p.idU)) OVER (PARTITION BY DATE_TRUNC('month', c.DateArchive)) AS total_participants
  FROM Concert c
  INNER JOIN Joue j ON c.idC = j.idC
  INNER JOIN Groupes g ON j.idU = g.idU
  INNER JOIN Participation p ON c.idC = p.idC
  WHERE c.DateArchive >= '2022-01-01'::DATE AND c.DateArchive < '2023-01-01'::DATE
  GROUP BY g.idU, c.Nom, c.DateArchive
)
SELECT idU, Nom, DateArchive, total_participants
FROM participants_cumules
WHERE total_participants = (
  SELECT MAX(total_participants) FROM participants_cumules
);






