import random
import math
import time

# Range of m in tests
MIN = 2 ** 22
MAX = 2 ** 23

# return le premier nombre premier > x
def firstPrimesAfter(x):
    if x < 2: return False
    if x%2 == 0: x = x+1

    isPrime = False
    while (not isPrime):
        x += 2
        isPrime = True
        for num in range(2, x):
            if x % num == 0:
                isPrime = False
    return x


# fonction generate qui prend en entrée le nombre d’agents m et le seuil k et
# retourne un nombre premier p adapt´e et les coefficients du polynôme.
def generate(m, k):
    p = firstPrimesAfter(m)
    coeffs = []
    for i in range(k):
        coeffs.append(random.randint(0, p))
    return p, coeffs


# fonction distribute
# p(x) = Σi=0,...,k (ai * x^i)
# si = p(i) (i = 1, . . . , m)
def distribute(m, k, p, coeffs, secret):
    coeffs = [secret] + coeffs
    s = []
    for i in range(0, m):
        pi = sum(coeffs[j] * (i ** j) for j in range(k + 1))
        s.append(pi)
    return s


# s = p(0) = Σi∈I si *  ( Πj!=i (−j) / Πj!=i (i − j) )
def coalition(p, k, agents):
    agentsi = [x for x, _ in agents]
    s = [y for _, y in agents]

    result = 0
    for i in range(len(agentsi)):
        dividend = 1
        divisor = 1
        for j in range(len(agentsi)):
            if j != i:
                dividend *= (0 - agentsi[j]) % p
                divisor *= (agentsi[i] - agentsi[j]) % p
        result = (result + s[i] * dividend * pow(divisor, -1, p)) % p
    return result

# Test that tries to calculate secret with nbAgents random agents, nbTests times
def test(m, k, secret, nbTests, nbAgents):
    p, coeff = generate(m, k)
    dis = distribute(m, k, p, coeff, secret)
    agents = [(i, dis[i]) for i in range(len(dis))]

    #number of correct calculations of the secret
    score = 0

    for i in range(nbTests):
        random_agents = []
        while (len(random_agents) < nbAgents):
            randChoice = random.choice(agents)
            if randChoice not in random_agents:
                random_agents.append(randChoice)

        secret_result = coalition(p, k, random_agents)
        if secret == secret_result: score += 1

    print("\nk = " + str(k))
    print("Testing with " + str(nbAgents) + " agents: ")
    if (score == nbTests): print("\033[32m", end='')
    print("Score: " + str(score) + "/" + str(nbTests))
    if (score == nbTests): print("\033[0m", end='')


def main():

    #Avec ces paramètres: temps attendu d'execution environ 50 secondes pour chaque appel à test
    m = random.randint(MIN,MAX)
    k = 25
    secret = 245245
    nbTests = 1000

    start_time = time.time()
    test(m, k, secret, nbTests, k + 1)
    end_time = time.time()
    elapsed_time = end_time - start_time
    print("Elapsed time: " + str(round(elapsed_time,2)) + "s.")

    start_time = time.time()
    test(m, k, secret, nbTests, k)
    end_time = time.time()
    elapsed_time = end_time - start_time
    print("Elapsed time: " + str(round(elapsed_time,2)) + "s.")

if __name__ == "__main__":
    main()
