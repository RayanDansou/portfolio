import time
import math


def efficient_logarithm(p, g, y):
    t = math.floor(p ** 0.5)
    # Calcul de la liste des puissances de g modulo p jusqu'à t
    # g^0, g^t, g^(2·t), . . . , g^((t+1)·t)
    seq1 = [pow(g, i * t, p) for i in range(t + 2)]

    # Calcul de la liste des y * g^(-kt) modulo p pour k allant de 0 à t-1
    # y · g^0, y · g, · · · , y · g^(t−1)
    seq2 = [(y * pow(g, k, p)) % p for k in range(t)]

    # Recherche de l'intersection entre seq1 et seq2
    # TODO: better code
    for k in range(t):
        if seq1[k] in seq2:
            i = seq2.index(seq1[k])
            return (k * t - i)
    return None


def naive_logarithm(p, g, y):
    for i in range(1, p):
        if pow(g, i, p) == y:
            return i
    else:
        return None


def test():
    # p g et y
    p = 23
    g = 3
    y = 2

    # Algorithme naïf
    start_time = time.time()
    x = naive_logarithm(p, g, y)
    naive_time = time.time() - start_time
    print("x naif = " + str(x))

    # Algorithme efficace
    start_time = time.time()
    x = efficient_logarithm(p, g, y)
    efficient_time = time.time() - start_time
    print("x algorithmique = " + str(x))

    print("Cas basique:")
    print(f"Temps d'exécution naïf: {naive_time:.4f} secondes")
    print(f"Temps d'exécution algorithmique: {efficient_time:.4f} secondes")
    print("--------------------------------------------")

    # cas avec p grand
    p = 10 ** 7 + 19

    # Algorithme naïf
    start_time = time.time()
    x = naive_logarithm(p, g, y)
    naive_time = time.time() - start_time
    print("x naif = " + str(x))

    # Algorithme efficace
    start_time = time.time()
    x = efficient_logarithm(p, g, y)
    efficient_time = time.time() - start_time
    print("x algorithmique = " + str(x))

    print("Pire des cas (sans résultat):")
    print(f"Temps d'exécution naïf: {naive_time:.4f} secondes")
    print(f"Temps d'exécution algorithmique: {efficient_time:.4f} secondes")


def main():
    test()


if __name__ == "__main__":
    main()
