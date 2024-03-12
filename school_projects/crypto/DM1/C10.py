import random
import hashlib
import time

# Range of q
MIN = 2 ** 22
MAX = 2 ** 23

def est_premier(n):  # Pour vérifier qu'un nombre est premier
    if n < 2:
        return False
    for i in range(2, int(n ** 0.5) + 1):
        if n % i == 0:
            return False
    return True


# def est_premier(n):
#    return n > 1 and all(n % i != 0 for i in range(2, int(n**0.5)+1))

def genB():
    while True:  # on génére 2 nombres aléatoires et on s'arrete des qu'ils sont tous les 2 premiers
        q = random.randint(MIN, MAX)
        p = 2 * q + 1
        if est_premier(q) and est_premier(p):
            break

    # Génère un générateur g de Zp*
    g = 2
    while pow(g, q, p) == 1:  # pour choisir le générateur
        g += 1

    # Génère un élément h de Zp* \ {1} tel que h^q ≡ 1 (mod p)
    while True:
        h = random.randint(2, p - 1)
        if pow(h, q, p) == 1:
            break
    return (p, g, h)


# Fonction 2 : verifA
def verifA(p, g, h):
    if not est_premier(p) or (p - 1) % 2 != 0:
        return False
    q = (p - 1) // 2
    if not est_premier(q) or pow(g, q, p) == 1:
        return False
    return pow(h, q, p) == 1


# Fonction 3 : commitA
def commitA(p,g,h,x):
    if verifA(p,g,h):
        q= (p-1)//2
        y=random.randint(1,q)
        c= pow(g,2*y,p)*pow(h,x,p)
        return (c,y)
    else:
        return (0, 0)


# Fonction 4 : revealA
def revealA(x, y):
    return (x, y)


# Fonction 5 : verifB
def verifB(p, g, h, c, x, y):
    return (c == pow(g, 2 * y, p) * pow(h, x, p))



def test(nbTests):
    score = 0
    print("Testing genB(), verifA(), commitA(), verifB() ...")
    for i in range(nbTests):
        # Test genB() function
        p, g, h = genB()
        assert est_premier(p) == True
        assert pow(g, (p-1)//2, p) != 1
        assert pow(h, (p-1)//2, p) == 1

        # Test verifA() function
        assert verifA(p, g, h) == True
        assert verifA(p+1, g, h) == False
        assert verifA(p, g+1, h) == False

        # Test commitA() and revealA() functions
        x = random.randint(1, (p-1)//2)
        c, y = commitA(p, g, h, x)
        assert c == pow(g, 2*y, p) * pow(h, x, p)
        assert revealA(x, y) == (x, y)

        # Test verifB() function
        assert verifB(p, g, h, c, x, y) == True
        assert verifB(p+1, g, h, c, x, y) == False

        score += 1

    if(score == nbTests):
            print("\033[32mAll tests passed!")
    print("Score: " + str(score) + "/" + str(nbTests))
    if(score == nbTests): print("\033[0m",end='')


def main():
    start_time = time.time()
    test(1000)
    end_time = time.time()
    elapsed_time = end_time - start_time
    print("Elapsed time: " + str(round(elapsed_time,2)) + "s.")



if __name__ == "__main__":
    main()
