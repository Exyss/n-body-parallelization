import csv
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

SCALE_FACTOR = 10000000

class Corpo:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return f"({self.x}, {self.y}, {self.z})"

    def __repr__(self):
        return f"({self.x}, {self.y}, {self.z})"

# Funzione per leggere i dati dal file CSV
def leggi_dati(file_path):
    dati = []
    with open(file_path, 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            dati.append([float(coord)/SCALE_FACTOR for coord in row if coord != ''])
    return dati

# Funzione chiamata ad ogni frame dell'animazione
def anima(frame):
    # Pulisce la figura precedente
    ax.cla()

    # Estrae le posizioni dei corpi per il frame corrente
    corpi = [Corpo(*coord) for coord in zip(frame[0::3], frame[1::3], frame[2::3])]

    # Estrae le coordinate x, y, z
    corpi_x = [corpo.x for corpo in corpi]
    corpi_y = [corpo.y for corpo in corpi]
    corpi_z = [corpo.z for corpo in corpi]

    # Disegna i corpi come punti nella posizione corrente
    ax.scatter(corpi_x, corpi_y, corpi_z, marker='o', s=30)

    # Imposta i limiti degli assi
    ax.set_xlim([-100000, 100000])
    ax.set_ylim([-100000, 100000])
    ax.set_zlim([-100000, 100000])

    # Aggiorna l'intestazione
    ax.set_title(f'{str(corpi)}', fontsize=6)

# Carica i dati dal file CSV
file_path = 'output.csv'
dati_corpi = leggi_dati(file_path)

# Crea la figura 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Crea l'animazione
ani = FuncAnimation(fig, anima, frames=dati_corpi, interval=50)

ani.save("render.mp4")
plt.close()