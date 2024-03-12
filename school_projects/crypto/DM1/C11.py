import random
import C10
import math


# permutation aleatoire sur 1-n
def gen_permut(n):
    p = list(range(n))
    random.shuffle(p)
    return p


def commit_all(p, g, h, perm, m, n):
    tab_x = [[0 for _ in range(0, n)] for _ in range(0, n)]
    tab_y = [[0 for _ in range(0, n)] for _ in range(0, n)]
    tab_c = [[0 for _ in range(0, n)] for _ in range(0, n)]
    for i in range(0, n):
        for j in range(0, n):
            tab_c[i][j], tab_y[i][j] = C10.commitA(p, g, h, perm[m[i][j] - 1])
            tab_x[i][j] = perm[m[i][j] - 1]
    return tab_c, tab_y, tab_x


# renvoie le challenge aleatoire choisi par B
def choix_chal(n):
    random.seed()
    chal = random.randint(0, 3 * n)
    return chal


def reveal_all(tab_x, tab_y, chal, n, pre_deter):
    rep = []
    if chal > 2 * n:  # On reveal une ligne
        for j in range(0, n):
            rep.append((tab_x[chal - (2 * n) - 1][j], tab_y[chal - (2 * n) - 1][j]))

    elif chal > n:  # On reveal une colone
        for i in range(0, n):
            rep.append((tab_x[i][chal - n - 1], tab_y[i][chal - n - 1]))

    elif chal > 0:  # On reveal une sous gille
        rac = int(math.sqrt(n))
        i = 0
        while i + rac <= chal - 1:  # On cherche la ligne de la sous grille
            i = i + rac
        j = rac * (chal - i - 1)  # On calcule la colone de la sous grille
        for k in range(0, rac):
            for l in range(0, rac):
                rep.append((tab_x[i + k][j + l], tab_y[i + k][j + l]))
    else:
        for e in pre_deter:
            rep.append((tab_x[e[0]][e[1]], tab_y[e[0]][e[1]]))
    return rep


def exist_permu(A, B, perm):
    sA = set(A)
    sB = set(B)
    if (len(sA) != len(sB)) or (len(A) != len(B)):
        return False

    if len(sA) == 0:
        return True

    e = A[0]  # On cherche une permutation ppour un element
    for p in perm:  # si on a deja trouver une permutation on l'applique
        if p[0] == e:
            A.remove(p[0])
            B.remove(p[1])
            exist_permu(A, B, perm)
            return False  # si la permutation s'avere etre fausse on s'arrete

    card_e = A.count(e)  # On calcule la cardinalité de l'element
    s_perm = []
    for i in sB:
        if B.count(i) == card_e:  # On trouve les elements de B qui ont le meme cardinale
            s_perm.append(i)

    for p in s_perm:  # On teste les permutations (Back Tracking)
        A.remove(e)
        B.remove(p)
        perm.append((e, p))
        if exist_permu(A, B, perm):
            return True
        perm.remove((e, p))
        A.append(e)
        B.append(p)
    return False  # ie un element n'a aucune permutation possible


# verifie la solution de A
def verif_all(p, g, h, m, tab_c, rev, chal, n, pre_deter):
    if chal > 0:
        In = list(range(0, n))

        if chal > 2 * n:  # On verifie la ligne
            for j in range(0, n):
                if tab_c[chal - (2 * n) - 1][j] == \
                        (pow(g, 2 * rev[j][1], p) * pow(h, rev[j][0], p)) and rev[j][0] in In:
                    In.remove(rev[j][0])
                else:
                    return False

        elif chal > n:  # On reveal une colone
            for i in range(0, n):
                if tab_c[i][chal - n - 1] == (pow(g, 2 * rev[i][1], p) * pow(h, rev[i][0], p)) and rev[i][0] in In:
                    In.remove(rev[i][0])
                else:
                    return False

        elif chal > 0:  # On reveal une sous gille
            rac = int(math.sqrt(n))
            i = 0
            while i + rac <= chal - 1:  # On cherche la ligne de la sous grille
                i = i + rac
            j = rac * (chal - i - 1)  # On cherche la colone de la sous grille
            for k in range(0, int(rac)):
                for l in range(0, int(rac)):
                    if tab_c[i + k][j + l] == (
                            pow(g, 2 * rev[(k * rac) + l][1], p) * pow(h, rev[(k * rac) + l][0], p)) and \
                            rev[(k * rac) + l][0] in In:
                        In.remove(rev[(k * rac) + l][0])
                    else:
                        return False
        return True

    else:
        i = 0
        permu_deter = []
        for e in pre_deter:
            if tab_c[e[0]][e[1]] == (pow(g, 2 * rev[i][1], p) * pow(h, rev[i][0], p)):
                permu_deter.append(rev[i][0])
                i = i + 1
            else:
                return False

        val_pre_deter = []
        for e in pre_deter:
            val_pre_deter.append(m[e[0]][e[1]] - 1)

        if exist_permu(val_pre_deter, permu_deter, []):
            return True
        return False


def do_test(n, m, pre_deter, p, g, h):
    perm = gen_permut(n)
    tab_c, tab_y, tab_x = commit_all(p, g, h, perm, m, n)
    chal = choix_chal(n)
    print("challenge " + str(chal))
    rev = reveal_all(tab_x, tab_y, chal, n, pre_deter)
    v = verif_all(p, g, h, m, tab_c, rev, chal, n, pre_deter)
    if v:
        print("tests reussi")
    else:
        print("Alice triche")


def test_rate():
    n = 9  # taille de la matrice de jeu
    m = [[1 for _ in range(0, n)] for _ in range(0, n)]  # matrice_de_jeu fausse
    pre_deter = [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2), (2, 0), (2, 1), (2, 2)]  # cellule predeterminer
    p, g, h = C10.genB()
    if C10.verifA(p, g, h):
        do_test(n, m, pre_deter, p, g, h)
    else:
        print("p, g et h ne passent pas les teste d'Alice")

def test_triche():
    n = 9  # taille de la matrice de jeu
    m = [[1, 1, 1, 1, 1, 1, 1, 1, 1],
         [2, 2, 2, 2, 2, 2, 2, 2, 2],
         [3, 3, 3, 3, 3, 3, 3, 3, 3],

         [4, 4, 4, 4, 4, 4, 4, 4, 4],
         [5, 5, 5, 5, 5, 5, 5, 5, 5],  # matrice_de_jeu triche
         [6, 6, 6, 6, 6, 6, 6, 6, 6],

         [7, 7, 7, 7, 7, 7, 7, 7, 7],
         [8, 8, 8, 8, 8, 8, 8, 8, 8],
         [9, 9, 9, 9, 9, 9, 9, 9, 9]]
    pre_deter = [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2), (2, 0), (2, 1), (2, 2)]  # cellule predeterminer
    p, g, h = C10.genB()
    if C10.verifA(p, g, h):
        do_test(n, m, pre_deter, p, g, h)
    else:
        print("p, g et h ne passent pas les teste d'Alice")

def test_reussi():
    n = 9  # taille de la matrice de jeu
    m = [[7, 6, 1, 5, 8, 3, 4, 9, 2],
         [9, 4, 3, 2, 7, 6, 8, 5, 1],
         [8, 5, 2, 4, 1, 9, 3, 6, 7],

         [1, 7, 4, 3, 2, 5, 9, 8, 6],
         [6, 8, 9, 1, 4, 7, 2, 3, 5],  # matrice_de_jeu correcte
         [2, 3, 5, 9, 6, 8, 1, 7, 4],

         [3, 2, 6, 7, 9, 4, 5, 1, 8],
         [5, 1, 8, 6, 3, 2, 7, 4, 9],
         [4, 9, 7, 8, 5, 1, 6, 2, 3]]
    pre_deter = [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2), (2, 0), (2, 1), (2, 2)]  # cellule predeterminer
    p, g, h = C10.genB()
    if C10.verifA(p, g, h):
        do_test(n, m, pre_deter, p, g, h)
    else:
        print("p, g et h ne passent pas les teste d'Alice")


# surement mieux de demander a l'execution du programme les parametre plutot que de les mettre en dure dans le code
def main():
    test_reussi()
    test_triche()
    test_rate()


"""
Question 2
Les parametre de sécurité choisi tel que p=2q+1 avec p et q de "grand" nombre premier (compris entre 10**15 et 10**16)
et h**q = 1 mod(p) avec h dans [2,p-1].
L'encodage d'une valeur x se fait de tel sort que c = [y ← Z_q : (g**(2y) · h**(x) ) mod p]
La fiabilité de cette encodage repose sur le fait qu'il est difficile de calculer le logarithme discret de grand nombre premier
et que Z_q soit suffisament grand pour qu'il soit difficile de trouver des colision lors du calcule de c.
"""

"""
Question 3
Chaque challenge a n! bonne réponse et n**n réponses au total. Il y a 3n+1 challenge.
Apres un échange qui a réussi la probabilité que Alice triche correspond donc :
a la probabilité que Alice ai donnée une bonne réponse pour chaque challenge
C'est a dire :au quotient du nombre de bonne réponse par le produit du nombre de réponse possible et du nombre de challenge.
C'est a dire : (n!) / ((n**n)*(3n+1))
Pour n=9 : (9!)÷((9^9)×(3×9+1)) = 0,000033452 < 10**3
pour n=9, il suffit donc d'une répétition pour etre certain que Alice ne triche pas.
Notre mise en oeuvre est asser efficace pour traiter un tel nombre de répétition.
"""

if __name__ == "__main__":
    main()