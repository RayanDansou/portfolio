


Choix techniques du Projet:

Nous avons fait le choix technique d'utiliser Java swing qui s'imposait comme plus facile pour nous
vu que nous n'avions jamais fait de JavaFx


Voici un diagramme de Classe de ce projet:


                        Main
                         |
                         |
                       Start
                         |
                         |
                     GameFrame
                         |
                         |
      --------------------------------------------
      |            |             |               |           
      |            |             |               |          
   Player       GameSolo        Text         StatDisplayer  
      |            |             |               |
      |            |             |               |
 ---------        Game        ----------        Stat
 |       |          |         |        |
 |       |          |         |        |
Life   Level      Mode       Word    TextUtils




Points non implémentés du projet:
   - mode multiJoueur
   - mots spéciaux du mode Normal

 
 
 
 Pour compiler le Projet:
    - Télécharger l'archive contenant le projet
    - l'extraire
    - Se rendre dans le dossier 
    - typintest/ 
    - Executer les commandes : 
            -mvn package
            -java -cp target/typintest-Y.jar com.example.control.Main