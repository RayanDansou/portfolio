from calculator import *  
import re

# D expressions régulières pour chaque opération 
plus = r"^-?\d*\s*\+\s*-?\d+$"
moins = r"^-?\d*\s*-\s*-?\d+$"
fois = r"^-?\d*\s*\*\s*-?\d+$"
div = r"^-?\d*\s*\/\s*-?\d+$"
mod = r"^-?\d*\s*%\s*-?\d+$"
Cos = r"^cos\s*-?\d+$"
Sin = r"^sin\s*-?\d+$"
Tan = r"^tan\s*-?\d+$"


# Fonction pour décomposer la commande 
def decompose_command(command):
    command = command.replace(" ", "")  # Supprime les espaces de la commande
    if re.match(plus, command):  # addition
        return addition, [int(x) for x in command.split('+') if x!= '']  # Sépare les nombres et les convertit en entiers
    elif re.match(div, command):  #  division
        return division, [int(x) for x in command.split('/') if x!= '']  
    elif re.match(moins, command):  #  soustraction
        return soustraction, [int(x) for x in command.split('-') if x!= '']  
    elif re.match(fois, command):  #  multiplication
        return multiplication, [int(x) for x in command.split('*') if x!= '']  
    elif re.match(mod, command):  #  modulo
        return modulo, [int(x) for x in command.split('%') if x!= '']  
    elif re.match(Cos, command):  #  fonction cosinus
        return cos, [int(x) for x in command.split('cos') if x!= '']  
    elif re.match(Tan, command):  #  fonction tangente
        return tan, [int(x) for x in command.split('tan') if x!= '']  
    elif re.match(Sin, command):  #  fonction sinus
        return sin, [int(x) for x in command.split('sin') if x!= '']  
    else:
        return None, None  # Retourne None si la commande ne correspond à aucune opération ou fonction


# Boucle principale pour la calculatrice
while True:
    data = input("Entrez votre Calcul: \n")  
    if data == "exit":  # si l'utilisateur souhaite quitter le programme
        break 
    operation, values = decompose_command(data)  # Décompose la commande et récupère l'opération et les valeurs
    if operation is not None and values is not None: 
        result = operation(values)  
        print("Résultat:", result)  # Affiche le résultat de l'opération
    else:
        print('Veuillez reformuler votre calcul\n')  # Affiche un message si la commande est incorrecte ou non prise en charge
