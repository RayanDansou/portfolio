import random
from time import time


###########################################################################################################
# Question 1

def substitution(a):
    # dictionnary of substitutions
    subs = {"0": "e", "1": "4", "2": "d", "3": "1", "4": "2", "5": "F", "6": "B", "7": "8", "8": "3",
            "9": "a", "a": "6", "b": "c", "c": "5", "d": "9", "e": "0", "f": "7"}
    # turn the input into a character, for example 1001 -> d
    char = str(hex(int(a, 2)))[2::]

    # get the result from the dictionnary and turn it from hex to int
    result = int(subs[char], 16)

    # return as string representing its binary value
    return format(result, '#06b')[2::]


def substitution_inverse(a):
    # dictionnary of substitutions
    subs = {"0": "e", "1": "3", "2": "4", "3": "8", "4": "1", "5": "C", "6": "A", "7": "F", "8": "7",
            "9": "D", "a": "9", "b": "6", "c": "B", "d": "2", "e": "0", "f": "5"}
    # turn the input into a character, for example 1001 -> d
    char = str(hex(int(a, 2)))[2::]

    # get the result from the dictionnary and turn it from hex to int
    result = int(subs[char], 16)

    # return as string representing its binary value
    return format(result, '#06b')[2::]


def rotate_clef(clef):
    # turn clef (int) into a binary string
    clef_to_str = format(int(bin(clef), 2), '#018b')[2::]
    # rotate the string and return as int
    rotated_str = clef_to_str[-4::] + (clef_to_str[:-4:])
    return int(rotated_str, 2)


def clefi(clef, i):
    for j in range(0, i - 1):
        clef = rotate_clef(clef)
    return clef


def rotate_clef_inverse(clef):
    # turn clef (int) into a binary string
    clef_to_str = format(int(bin(clef), 2), '#018b')[2::]
    # rotate the string and return as int
    rotated_str = (clef_to_str[4::]) + clef_to_str[:4:]
    return int(rotated_str, 2)


def clefi_inverse(clef, i):
    for j in range(0, i - 1):
        clef = rotate_clef_inverse(clef)
    return clef


def perms(n):
    # fill the beginning with 0
    s = format(int(n, 2), '#018b')

    # create new permutated list and join it to return as string
    res = [int(x) for x in s[2::]]
    l = [res[0], res[4], res[8], res[12], res[1], res[5], res[9], res[13], res[2], res[6], res[10], res[14], res[3],
         res[7], res[11], res[15]]

    return ''.join(str(x) for x in l)

# l'inverse d'une permutation est la permutation elle même
perms_inverse = perms

def f(text, clef):
    new_text = text
    for i in range(1, 5):  # todo remettre 5
        # xor
        new_text = new_text ^ clefi(clef, i)

        # create y1 y2 y3 y4
        new_text = format(new_text, '#018b')
        y1 = new_text[2:6]
        y2 = new_text[6:10]
        y3 = new_text[10:14]
        y4 = new_text[14:18]

        # substitutions
        sub_y1 = substitution(y1)
        sub_y2 = substitution(y2)
        sub_y3 = substitution(y3)
        sub_y4 = substitution(y4)
        new_text = sub_y1 + sub_y2 + sub_y3 + sub_y4

        # permutation
        new_text = int(perms(new_text), 2)

    return new_text


def f_inverse(text, clef):
    new_text = text
    for i in range(4, 0, -1):  # todo remettre 5
        # permutation
        new_text = int(perms_inverse(bin(new_text)), 2)

        # create y1 y2 y3 y4
        new_text = format(new_text, '#018b')
        y1 = new_text[2:6]
        y2 = new_text[6:10]
        y3 = new_text[10:14]
        y4 = new_text[14:18]

        # substitutions
        sub_y1 = substitution_inverse(y1)
        sub_y2 = substitution_inverse(y2)
        sub_y3 = substitution_inverse(y3)
        sub_y4 = substitution_inverse(y4)
        new_text = sub_y1 + sub_y2 + sub_y3 + sub_y4

        # xor
        new_text = int(new_text, 2) ^ clefi(clef, i)

    return new_text


###########################################################################################################

# Question 2

# ATTENTION: return un tableau d'entier, utiliser bin dessus lors de l'utilisation
def decoupe_bin_chiffr(binaire, length):
    binaire_str = bin(binaire)[2:]
    len_bourrage = length - (len(binaire_str) % length)
    binaire_str = binaire_str.zfill(len_bourrage + len(binaire_str))
    binaire_str = binaire_str[:len_bourrage - 1] + "1" + binaire_str[len_bourrage:]
    # comme ça on bourre avec le mot 000000001 (par exemple)
    # on peut retrouver le mot original en supprimant cette partie
    return [int("0b" + (binaire_str[i:i + length]), 2) for i in range(0, len(binaire_str), length)]


def decoupe_bin_dechiffr(binaire, length):
    binaire_str = bin(binaire)[2:]
    len_bourrage = length - (len(binaire_str) % length)
    if length != len_bourrage:
        binaire_str = binaire_str.zfill(len_bourrage + len(binaire_str))
    return [int("0b" + (binaire_str[i:i + length]), 2) for i in range(0, len(binaire_str), length)]


# F est une PRF (pseudo-random function generator),
# p un texte clair,
# r est un compteur (16 bits),
# k est une clef (16 bits)
def chiffrement_mode_ctr(F, p, r, k):
    # Découpe en blocs de 16
    blocs = decoupe_bin_chiffr(p, 16)
    index = 0
    res_concat = ""
    while index < len(blocs):
        res_f_xor = F(r, k) ^ blocs[index]
        res_f_xor = format(res_f_xor, '#018b')
        res_concat = res_concat + res_f_xor[2:]

        index += 1
        if r >= 2 ** 16:
            r = 0
        else:
            r += 1
    return "0b" + res_concat


# F est une PRF (pseudo-random function generator),
# c un texte chiffré,
# r est un compteur (16 bits),
# k est une clef (16 bits)
def dechiffrement_mode_ctr(F, c, r, k):
    # Découpe en blocs de 16
    blocs = decoupe_bin_dechiffr(c, 16)
    index = 0
    res_concat = ""
    while index < len(blocs):
        res_f_xor = F(r, k) ^ blocs[index]
        res_f_xor = format(res_f_xor, '#018b')
        res_concat = res_concat + res_f_xor[2:]

        index += 1
        if r >= 2 ** 16:
            r = 0
        else:
            r += 1

    # on supprime le bourrage
    debourrage_len = 0
    while res_concat[debourrage_len] != '1' and debourrage_len <= 16:
        debourrage_len += 1
    res_concat = res_concat[debourrage_len + 1:]

    return "0b" + res_concat


#####################################################################################################
# Question 3:
"""
Un caractère en Unicode est codé en 16 bits et chaque message est constitué de 100 caractères donc 1600 bits.
L'addition sur r est modulo 2**16.
Pour chaque message, on incrémente R de 100 (+1 par 16 bits)
Ce qui veut dire que si l'on envoie un nombre de messages l > (65536 / 100),
alors il y aura une intersection entre le premier message et le 655ème.

Attention, R étant tiré aléatoirement, pour que cette hypothèse fonctionne
les 655 messages doivent être envoyés en un seul "texte" de 655 * 100 * 16 (=1 048 000) bits.
"""
#####################################################################################################
# Main + Tests

def test_q1(nbTests):
    # un texte de 16 bits aléatoire et une clef aléatoire
    random.seed(time())
    cpt_points = 0
    tour = 0

    while tour < nbTests:
        tour += 1
        text = random.randint(0, 2 ** 16)
        clef = random.randint(0, 2 ** 16)
        if text == f_inverse(f(text, clef), clef):
            cpt_points += 1

    if (cpt_points == nbTests): print("\033[32m", end='')
    print("tests f et f_inverse : " + str(cpt_points) + "/" + str(nbTests))
    if (cpt_points == nbTests): print("\033[0m", end='')


def test_q2(nbTests):
    # un texte de 1024 bits aléatoires ainsi qu'une clef et un entier r de 16 bits aléatoires
    random.seed(time())
    cpt_points = 0
    tour = 0

    while tour < nbTests:
        tour += 1
        text = random.randint(0, 2 ** 1024)
        clef = random.randint(0, 2 ** 16)
        r = random.randint(0, 2 ** 16)
        c = chiffrement_mode_ctr(f, text, r, clef)
        p = dechiffrement_mode_ctr(f, int(c, 2), r, clef)
        if bin(text) == p:
            cpt_points += 1

    if (cpt_points == nbTests): print("\033[32m", end='')
    print("tests chiffrement et déchiffrement : " + str(cpt_points) + "/" + str(nbTests))
    if (cpt_points == nbTests): print("\033[0m", end='')


def main():
    test_q1(1000)
    test_q2(1000)


if __name__ == "__main__":
    main()
