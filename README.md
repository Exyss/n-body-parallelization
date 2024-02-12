# n-body-parallelization
In this project, several C implementations are provided to parallelize the [n-body problem](https://en.wikipedia.org/wiki/N-body_problem). 

## ♟️ Algorithms
Two approaches were followed to work on the project:
- direct sum algorithm (_exhaustive_)
- [Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation).

## 👾 Implementations
A single process version and two multicore versions are provided for both algorithms:
- one with distributed memory (implemented using the [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface) library)
- one with shared memory (implemented using the [OMP](https://en.wikipedia.org/wiki/OpenMP) library).

## 🗂️ In this repository 
In this repository you will find:
- Various versions of the code are provided that analyze the optimization process.
- A detailed report is provided explaining the various codes and the reasons for the choices made.
- Python scripts are present in the **utility** folder for:
    - carry out performance tests
    - generate input files with random bodies
    - render the output file and have a graph of the bodies
    - test the similarity between the outputs of the different implementations
- a data folder with some pre-populated input csv files end some expected output 

## 📚 Libs
To run the codes correctly you need to install the libraries:
- open-mpi 
- omp 
- mathplot (for render)

## 💻 Usage
To download the sources clone [this repository](https://github.com/Exyss/n-body-parallelization.git).
there is a makefile for each subfolder that compiles the various versions. Just set the current working directory to the folder of the file you want to run and run `make`.

### To run the code:
- **exhaustive sequential** version

```bash
 .\<name_of_executable> <input_file> <simulation_steps>
```
- **Barnes Hut sequential** version 
```bash
.\<name_of_executable> <input_file> <simulation_steps> <approx_threshold>
```
- **exhaustive mpi** version
```bash
mpirun -n <number_of_cores> <name_of_executable> <input_file> <simulation_steps>
```
- **Barnes Hut mpi** version
```bash
mpirun -n <number_of_cores> <name_of_executable> <input_file> <simulation_steps> <approx_threshold>
```
- **exhaustive omp** version
```bash
.\<name_of_executable> <input_file> <simulation_steps> <thread_count>
```
- **Barnes Hut omp** 
```bash
.\<name_of_executable> <input_file> <simulation_steps> <approx_threshold> <thread_count>
```

The output file will be written to the current directory with the name _output.csv_
