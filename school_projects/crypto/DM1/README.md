DM1 Cryptographie

Groupe 5:
Mathieu Crocombette--Pasquet
Yao Rayan Dansou
Clement Martineau
Tedi Vyshka
=============================

## Pour exécuter :
python3 C[x].py
avec x = 4, 6, 8, 10, 11

#
### Tests C4:
- Q1: On effectue nbTests tests avec des séquences de 16 bits aléatoires.
On fait f_inverse de f avec en entrée les séquences aléatoires.
On vérifie ensuite que le texte résultant est égal a la séquence de départ.

- Q2: On effectue nbTests tests avec une clef et un R de 16 bits aléatoires, ainsi que un texte de 1024 bits aléatoires.
On fait le déchiffrement du chiffrement avec en entrée les séquences aléatoires.
On vérifie ensuite que le texte résultant est égal à la séquence de départ.
#
### Tests C6:
La fonction test(m,k,secret, nbTests, nbAgents) tire aléatoirement nbAgents agents de la sortie de distribute()
Puis, elle essaye de calculer secret.
On lance cette fonction avec k+1 agents une première fois, et puis avec k agents.
Avec k+1 agents, on est censé trouver le secret 100% de temps.
#
### Tests C8:
La fonction test() teste deux differents cas du calcul du logarithme discret avec un algorithme naif et l'algorithme efficace demandé.
Premier cas: petites valeurs, exécution instantanée.
Deuxième cas: grands valeurs (pas de réponse), environ 10 secondes pour l'algorithme naif, execution instantanée pour l'algorithme efficace.
#
### Tests C10:
La fonction test() teste nbTests fois les fonctions genB, verifA, commitA et verifB, pour que les valeurs sont correctes.
#
### Tests C11:
L'affichage produit lors de l'execution renseigne le challenge qui a été fait ainsi que la conclusion fait sur Alice.
L'execution du programme va effectuer 3 tests sur des matrice de taille 9. Le premier sur une matrice où Alice connais la solution.
Le second sur une matrice où Alice triche. Et le dernier sur une matrice où Alice ne connais pas la solution et
où on remarque qu'elle se trompe a chaque tests.
