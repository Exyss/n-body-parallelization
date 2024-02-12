import subprocess
import time
import os
import csv

def tuple_in_csv(lista_tuple, nome_file):
    with open(nome_file, 'w', newline='') as file:
        csv_writer = csv.writer(file)
        for tupla in lista_tuple:
            csv_writer.writerow(tupla)

DIR = "src"
results = []
"""
process = subprocess.Popen(["./programma_c"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
stdout, stderr = process.communicate()"""

def avg(valori):
    sum = 0
    for v in valori:
        sum += v
    return sum/len(valori)

fileinput = [
    "../../../data/input/1000_bodies.csv",
    "../../../data/input/2000_bodies.csv",
    "../../../data/input/3000_bodies.csv",
    "../../../data/input/4000_bodies.csv",
    "../../../data/input/5000_bodies.csv",
    "../../../data/input/6000_bodies.csv",
    "../../../data/input/7000_bodies.csv",
    "../../../data/input/8000_bodies.csv",
    "../../../data/input/9000_bodies.csv",
    "../../../data/input/10000_bodies.csv"
]

theta = [0.0, 0.5, 1.0]

num_step = [100]

def execProg(dire):
    process =subprocess.Popen(["make"], cwd=dire)
    stdout, stderr = process.communicate()
    if process.returncode != 0:
        print("Errore durante l'esecuzione del Make:")
        print(stderr.decode())
        return None
    for el in os.listdir(dire):
        if el != "makefile" and '.' not in el:
            if "exhaustive" in dire:
                if "sequential" in dire:
                    for inp in fileinput:
                        for nu in num_step:
                            val = []
                            for i  in range(5):
                                print(f"{el} {inp} {nu} {i}")
                                start_time = time.time()
                                process = subprocess.Popen([f"./{el}", inp, str(nu)],cwd=dire, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                                stdout, stderr = process.communicate()
                                if process.returncode != 0:
                                    print("Errore durante l'esecuzione del programma C:")
                                    print(stderr.decode())
                                    return None
                                end_time = time.time()
                                val.append((end_time - start_time)*10)
                            
                            results.append((el, inp, nu, 1, "%.9f" % avg(val)))
                elif "mpi" in dire:
                    for i in [1, 2, 4, 8]:
                        for inp in fileinput:
                            for nu in num_step:
                                val = []
                                for j  in range(5):
                                    print(f"{el} {inp} {nu} {i} {j}")
                                    start_time = time.time()
                                    process = subprocess.Popen(["mpirun", "-n", str(i), el, inp, str(nu)],cwd=dire, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                                    stdout, stderr = process.communicate()
                                    if process.returncode != 0:
                                        print("Errore durante l'esecuzione del programma C:")
                                        print(stderr.decode())
                                        return None
                                    end_time = time.time()
                                    val.append((end_time - start_time)*10)
                                results.append((el, inp, nu, i, "%.9f" % avg(val)))
                elif "omp" in dire:
                    for i in [1, 2, 4, 8]:
                        for inp in fileinput:
                            for nu in num_step:
                                val = []
                                for j  in range(5):
                                    print(f"{el} {inp} {nu} {i} {j}")
                                    start_time = time.time()
                                    process = subprocess.Popen([f"./{el}", inp, str(nu), str(i)],cwd=dire, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                                    stdout, stderr = process.communicate()
                                    if process.returncode != 0:
                                        print("Errore durante l'esecuzione del programma C:")
                                        print(stderr.decode())
                                        return None
                                    end_time = time.time()
                                    val.append((end_time - start_time)*10)
                                results.append((el, inp, nu, i, "%.9f" % avg(val)))

            if "barnes_hut" in dire:
                if "sequential" in dire:
                    for inp in fileinput:
                        for nu in num_step:
                            for t in theta:
                                val = []
                                for i  in range(5):
                                    print(f"{el} {inp} {nu} {t} {i}")
                                    start_time = time.time()
                                    process = subprocess.Popen([f"./{el}", inp, str(nu), str(t)],cwd=dire, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                                    stdout, stderr = process.communicate()
                                    end_time = time.time()
                                    if process.returncode != 0:
                                        print("Errore durante l'esecuzione del programma C:")
                                        print(stderr.decode())
                                        return None
                                    val.append((end_time - start_time)*10)
                                
                                results.append((el, inp, nu, t, 1, "%.9f" % (avg(val)*10)))
                elif "mpi" in dire:
                    for i in [1, 2, 4, 8]:
                        for inp in fileinput:
                            for nu in num_step:
                                for t in theta:
                                    val = []
                                    for j  in range(5):
                                        print(f"{el} {inp} {nu} {t} {i} {j}")
                                        start_time = time.time()
                                        process = subprocess.Popen(["mpirun", "-n", str(i), el, inp, str(nu), str(t)], cwd=dire, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                                        stdout, stderr = process.communicate()
                                        if process.returncode != 0:
                                            print("Errore durante l'esecuzione del programma C:")
                                            print(stderr.decode())
                                            return None
                                        end_time = time.time()
                                        val.append((end_time - start_time)*10)
                                    results.append((el, inp, nu, t, i, "%.9f" % avg(val)))
                elif "omp" in dire:
                    for i in [1, 2, 4, 8]:
                        for inp in fileinput:
                            for nu in num_step:
                                for t in theta:
                                    val = []
                                    for j  in range(5):
                                        print(f"{el} {inp} {nu} {t} {i} {j}")
                                        start_time = time.time()
                                        process = subprocess.Popen([f"./{el}", inp, str(nu), str(t), str(i)], cwd=dire, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                                        stdout, stderr = process.communicate()
                                        if process.returncode != 0:
                                            print("Errore durante l'esecuzione del programma C:")
                                            print(stderr.decode())
                                            return None
                                        end_time = time.time()
                                        val.append((end_time - start_time)*10)
                                    results.append((el, inp, nu, t, i, "%.9f" % avg(val)))

def explore(dire = DIR):
    elen = os.listdir(dire)
    if "makefile" in elen:
        execProg(dire)
    else:
        for item in os.listdir(dire):
            item_path = os.path.join(dire, item)
            if os.path.isdir(item_path):
                explore(item_path)
        
explore()
tuple_in_csv(results, "risultati.csv")
#valori = [0.060467,0.06135,0.062675,0.061519,0.061372,0.063248,0.064477,0.063056,0.06215,0.062947,0.061723,0.064689]
#print(avg(valori))