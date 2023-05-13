import random
import time

# Definim intervalul de valori pentru fiecare câmp
id_produs_range = range(1, 210)
id_oras_range = range(1, 11)
cantitate_range = range(0, 50)

# Definim o listă goală pentru a ține evidența id-urilor de orașe utilizate
id_oras_utilizate = []

# Definim variabila pentru a numara randurile generate
numar_randuri = 0

# Deschidem fișierul CSV pentru scriere
with open("date.csv", "w") as file:
    # Scriem antetul coloanelor
    file.write("ID_Produs,ID_Oras,Cantitate_Produs\n")

    # Generăm date aleatoare până când s-au generat 1000 de randuri
    while numar_randuri < 500:
        id_produs = random.choice(id_produs_range)
        id_oras = random.choice(id_oras_range)
        # Verificăm dacă id-ul orașului a fost deja utilizat
        if id_oras not in id_oras_utilizate:
            # Adăugăm id-ul orașului în lista de id-uri utilizate
            id_oras_utilizate.append(id_oras)
        cantitate = random.choice(cantitate_range)
        file.write(
            f"{id_produs},{id_oras},{cantitate}\n")
        numar_randuri += 1

gasit = 0
contor = 0

for i in range(1, 11):
    gasit = 0
    for element in id_oras_utilizate:
        if element == i:
            gasit = 1
            break
    if gasit == 0:
        contor += 1

print(contor)

# Afisam un mesaj de final
print("Au fost generate 500 de randuri in fisierul CSV.")

# Afisam timpul de executie
print(f"Timpul de executie: {time.process_time()} secunde.")
