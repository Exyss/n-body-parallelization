import csv
import random

# Numero di pianeti
num_pianeti = 100

# Apri un file CSV in modalità scrittura
with open(f'{num_pianeti}_bodies.csv', 'w', newline='') as file:
    writer = csv.writer(file)

    # Genera dati casuali per ogni pianeta
    for _ in range(num_pianeti):
        massa = random.uniform(1e20, 1e30)
        posizione_x = random.uniform(-1e15, 1e15)
        posizione_y = random.uniform(-1e15, 1e15)
        posizione_z = random.uniform(-1e15, 1e15)
        velocità_x = random.uniform(-1e5, 1e5)
        velocità_y = random.uniform(-1e5, 1e5)
        velocità_z = random.uniform(-1e5, 1e5)

        # Scrivi i dati nel file CSV
        writer.writerow([massa, posizione_x, posizione_y, posizione_z, velocità_x, velocità_y, velocità_z])

print(f"Dati generati e salvati in {num_pianeti}_bodies.csv")