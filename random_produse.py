import random
import time

id_produs_range = range(0, 20)
id_oras_range = range(0, 11)
cantitate_range = range(0, 51)

id_oras_utilizate = []

numar_randuri = 0

with open("date.csv", "w") as file:
    file.write("ID_Produs,ID_Oras,Cantitate_Produs\n")

    while numar_randuri < 220:
        id_produs = random.choice(id_produs_range)
        id_oras = random.choice(id_oras_range)
        id_oras_duplicat = False
        for produs_oras in id_oras_utilizate:
            if produs_oras[0] == id_produs and produs_oras[1] == id_oras:
                id_oras_duplicat = True
                break

        if not id_oras_duplicat:
            id_oras_utilizate.append((id_produs, id_oras))
            cantitate = random.choice(cantitate_range)
            file.write(f"{id_produs},{id_oras},{cantitate}\n")
            numar_randuri += 1

id_oras_neutilizate = 0
for i in range(1, 12):
    id_produs_găsit = False
    for produs_oras in id_oras_utilizate:
        if produs_oras[1] == i:
            id_produs_găsit = True
            break
    if not id_produs_găsit:
        id_oras_neutilizate += 1

print(f"Numărul de ID-uri de orașe neutilizate: {id_oras_neutilizate}")

print("Au fost generate 500 de rânduri în fișierul CSV.")

print(f"Timpul de executie: {time.process_time()} secunde.")
