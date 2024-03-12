DROP TABLE IF EXISTS Utilisateur CASCADE;
DROP TABLE IF EXISTS Morceau CASCADE;
DROP TABLE IF EXISTS Genre_music CASCADE;
DROP TABLE IF EXISTS MotCle CASCADE;
DROP TABLE IF EXISTS Contient CASCADE;
DROP TABLE IF EXISTS Commentaire CASCADE;
DROP TABLE IF EXISTS Tag CASCADE;
DROP TABLE IF EXISTS Playlist CASCADE;
DROP TABLE IF EXISTS Amis CASCADE;
DROP TABLE IF EXISTS Follow CASCADE;
DROP TABLE IF EXISTS Salle_concert CASCADE;
DROP TABLE IF EXISTS Participation CASCADE;
DROP TABLE IF EXISTS Organise CASCADE;
DROP TABLE IF EXISTS Joue CASCADE;
DROP TABLE IF EXISTS Archive_concert CASCADE;
DROP TABLE IF EXISTS Prochain_concert CASCADE;
DROP TABLE IF EXISTS Avis_concert CASCADE;
DROP TABLE IF EXISTS Avis_groupe CASCADE;
DROP TABLE IF EXISTS Avis_morceau CASCADE;
DROP TABLE IF EXISTS Avis_salle CASCADE;
DROP TABLE IF EXISTS Groupes CASCADE;
DROP TABLE IF EXISTS Personnes CASCADE;
DROP TABLE IF EXISTS Associations CASCADE;
DROP TABLE IF EXISTS Artistes CASCADE;
DROP TABLE IF EXISTS Concert CASCADE;
DROP TABLE IF EXISTS Appartient CASCADE;

CREATE TABLE Utilisateur(
    idU INT PRIMARY KEY NOT NULL,
    Nom VARCHAR(200) NOT NULL,
    Ville VARCHAR(35)NOT NULL,
    Pays VARCHAR(35)NOT NULL,
    typeUtilisateur VARCHAR(35) CHECK (typeUtilisateur IN ('Personne', 'Groupe', 'Association')) NOT NULL,
    CONSTRAINT unique_id UNIQUE (idU)
);

CREATE TABLE Artistes(
    idU INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU),
    CONSTRAINT unique_id_art UNIQUE (idU)
);

CREATE TABLE Groupes(
    idU INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU)
);

CREATE TABLE Associations(
    idU INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU),
    CONSTRAINT unique_id_assoc UNIQUE (idU)
);

CREATE TABLE Personnes(
    idU INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU),
    CONSTRAINT unique_id_person UNIQUE (idU)
);


CREATE TABLE Amis(
    idU1 INT NOT NULL,
    idU2 INT NOT NULL,
    FOREIGN KEY (idU1) REFERENCES Utilisateur(idU),
    FOREIGN KEY (idU2) REFERENCES Utilisateur(idU),
    CONSTRAINT unique_amis UNIQUE (idU1, idU2)
);

CREATE TABLE Follow(
    idU1 INT NOT NULL,
    idU2 INT NOT NULL,
    FOREIGN KEY (idU1) REFERENCES Utilisateur(idU),
    FOREIGN KEY (idU2) REFERENCES Utilisateur(idU),
    CONSTRAINT unique_follow UNIQUE (idU1, idU2)
);

CREATE TABLE Playlist(
    idP INT PRIMARY KEY NOT NULL,
    idU INT NOT NULL,
    Nom VARCHAR(35) NOT NULL,
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU)
);

CREATE TABLE Morceau(
    idM INT PRIMARY KEY NOT NULL,
    idU INT NOT NULL,
    Titre VARCHAR(35) NOT NULL,
    DureeMorceau TIME NOT NULL,
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU)
);

CREATE TABLE Genre_music(
    idGenre INT PRIMARY KEY NOT NULL,
    Nom VARCHAR(35) NOT NULL,
    Genre_parent INT 
);

CREATE TABLE MotCle(
    idMotcle INT PRIMARY KEY NOT NULL,
    Nom VARCHAR(35) NOT NULL,
    isGenre BOOLEAN NOT NULL
);

CREATE TABLE Contient(
    idPlaylist INT NOT NULL,
    idM INT NOT NULL,
    FOREIGN KEY (idPlaylist) REFERENCES Playlist(idP),
    FOREIGN KEY (idM) REFERENCES Morceau(idM)
);

CREATE TABLE Commentaire(
    idCom INT PRIMARY KEY NOT NULL,
    Contenu VARCHAR(350) NOT NULL,
    typeCom VARCHAR(35) CHECK (typeCom IN ('Salle', 'Groupe', 'Morceau', 'Concert')) NOT NULL
);

CREATE TABLE Tag(
    idCom INT NOT NULL,
    idMotcle INT NOT NULL,
    FOREIGN KEY (idCom) REFERENCES Commentaire(idCom),
    FOREIGN KEY (idMotcle) REFERENCES MotCle(idMotcle)
);

CREATE TABLE Salle_concert(
    idSalle INT PRIMARY KEY NOT NULL,
    Nom VARCHAR(100) NOT NULL,
    Adresse VARCHAR(100) NOT NULL,
    Ville VARCHAR(35) NOT NULL,
    Pays VARCHAR(35) NOT NULL
);

CREATE TABLE Concert(
    idC INT PRIMARY KEY NOT NULL,
    idSalle INT NOT NULL,
    Nom VARCHAR(35 ) NOT NULL,
    dateC DATE NOT NULL,
    nb_participant INT NOT NULL,
    isEnfantAllowed BOOLEAN NOT NULL,
    statut VARCHAR(35) CHECK (statut IN ('Future', 'Passé'))NOT NULL,
    FOREIGN KEY (idSalle) REFERENCES Salle_concert(idSalle)
);

CREATE TABLE Archive_concert(
    idC INT NOT NULL,
    FOREIGN KEY (idC) REFERENCES Concert(idC),
    CONSTRAINT unique_archive UNIQUE (idC)
);

CREATE TABLE Prochain_concert(
    idC INT NOT NULL,
    FOREIGN KEY (idC) REFERENCES Concert(idC),
    CONSTRAINT unique_pro UNIQUE (idC)

);

CREATE TABLE Avis_salle(
    idAvS INT PRIMARY KEY NOT NULL,
    idCom INT NOT NULL,
    idSalle INT NOT NULL,
    idDonneur INT NOT NULL,
    FOREIGN KEY (idCom) REFERENCES Commentaire(idCom),
    FOREIGN KEY (idSalle) REFERENCES Salle_concert(idSalle),
    FOREIGN KEY (idDonneur) REFERENCES Utilisateur(idU)
);

CREATE TABLE Avis_morceau(
    idAvM INT PRIMARY KEY NOT NULL,
    idCom INT NOT NULL,
    idM INT NOT NULL,
    idDonneur INT NOT NULL,
    FOREIGN KEY (idCom) REFERENCES Commentaire(idCom),
    FOREIGN KEY (idM) REFERENCES Morceau(idM),
    FOREIGN KEY (idDonneur) REFERENCES Utilisateur(idU)
);


CREATE TABLE Avis_groupe(
    idAvG INT PRIMARY KEY NOT NULL,
    idCom INT NOT NULL,
    idU INT NOT NULL,
    idDonneur INT NOT NULL,
    FOREIGN KEY (idCom) REFERENCES Commentaire(idCom),
    FOREIGN KEY (idU) REFERENCES Utilisateur(idU),
    FOREIGN KEY (idDonneur) REFERENCES Utilisateur(idU)
);

CREATE TABLE Avis_concert(
    idAvC INT PRIMARY KEY NOT NULL,
    idCom INT NOT NULL,
    idC INT NOT NULL,
    idDonneur INT NOT NULL,
    FOREIGN KEY (idCom) REFERENCES Commentaire(idCom),
    FOREIGN KEY (idC) REFERENCES Archive_concert(idC),
    FOREIGN KEY (idDonneur) REFERENCES Utilisateur(idU)
);

CREATE TABLE Participation(
    idU INT NOT NULL,
    idC INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Personnes(idU),
    FOREIGN KEY (idC) REFERENCES Prochain_concert(idC)
);

CREATE TABLE Organise(
    idU INT NOT NULL,
    idC INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Associations(idU),
    FOREIGN KEY (idC) REFERENCES Prochain_concert(idC)
);

CREATE TABLE Joue(
    idU INT NOT NULL,
    idC INT NOT NULL,
    FOREIGN KEY (idU) REFERENCES Artistes(idU),
    FOREIGN KEY (idC) REFERENCES Prochain_concert(idC)
);

CREATE TABLE Appartient(
    idM INT NOT NULL,
    idGenre INT NOT NULL,
    FOREIGN KEY (idM) REFERENCES Morceau(idM),
    FOREIGN KEY (idGenre) REFERENCES Genre_music(idGenre)
);

\copy Utilisateur FROM 'CSV/Utilisateur.csv' WITH (FORMAT csv, DELIMITER ',');

INSERT INTO Groupes (idU)
SELECT idU
FROM Utilisateur
WHERE typeUtilisateur = 'Groupe';

INSERT INTO Personnes (idU)
SELECT idU
FROM Utilisateur
WHERE typeUtilisateur = 'Personne';


INSERT INTO Associations (idU)
SELECT idU
FROM Utilisateur
WHERE typeUtilisateur = 'Association';

INSERT INTO Artistes (idU)
SELECT idU
FROM Utilisateur
WHERE typeUtilisateur = 'Groupe'
OR typeUtilisateur = 'Personne';

\copy Amis FROM 'CSV/Amis.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Follow FROM 'CSV/Follow.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Playlist FROM 'CSV/Playlist.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Morceau FROM 'CSV/Morceau.csv' WITH (FORMAT csv, DELIMITER ',');
\copy MotCle FROM 'CSV/Motcle.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Contient FROM 'CSV/Contient.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Commentaire FROM 'CSV/Commentaire.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Tag FROM 'CSV/Tag.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Salle_concert FROM 'CSV/Salle_concert.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Avis_salle FROM 'CSV/avis_salle.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Avis_groupe FROM 'CSV/avis_groupe.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Avis_morceau FROM 'CSV/avis_morceau.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Concert FROM 'CSV/Concert.csv' WITH (FORMAT csv, DELIMITER ',');

INSERT INTO Archive_concert (idC)
SELECT idC
FROM Concert
WHERE statut = 'Passé';

INSERT INTO Prochain_concert (idC)
SELECT idC
FROM Concert
WHERE statut = 'Future';

\copy Avis_concert FROM 'CSV/avis_concert.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Joue FROM 'CSV/joue.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Organise FROM 'CSV/organise.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Participation FROM 'CSV/participation.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Genre_music FROM 'CSV/Genre_music.csv' WITH (FORMAT csv, DELIMITER ',');
\copy Appartient FROM 'CSV/Appartient.csv' WITH (FORMAT csv, DELIMITER ',');


