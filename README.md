# N-Body Problem Parallelization
This project contains several C implementations capable of parallelizing the [n-body problem](https://en.wikipedia.org/wiki/N-body_problem). The project was made by students for a Multicore Programming course within a BSc degree program in Computer Science.

## ♟️ Algorithms
Two algorithms were used to experiment:
- A trivial direct sum algorithm with time complexity: $O(n^3)$
- The [Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation) with time complexity: $O(n \log n)$

Both algorithms have been parallelized and compared.

## 👾 Implementations
A single process version and two multicore versions are provided for both algorithms:
- One with distributed memory, implemented using the [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface) library.
- One with shared memory, implemented using the [OpenMP](https://en.wikipedia.org/wiki/OpenMP) library.

## 🗂️ In this repository 
In this repository you will find:
- Various versions of optimization procedures done on the initial code.
- A detailed report explaining the various optimizations and the reason behind the choices made.
- Some utility Python scripts that are able to:
    - Carry out performance tests
    - Generate input files with random bodies
    - Render the output file and have a graph of the bodies
    - Test the similarity between the outputs of the different implementations
- The data used for performance analysis, including pre-populated input csv files end some expected results. 

## 📚 Libs
To run the codes correctly you need to install the libraries:
- open-mpi (C)
- omp (C)
- mathplot (Python, only to render the results)

## 💻 Usage
First, clone [this repository](https://github.com/Exyss/n-body-parallelization.git). In each subfolder there is a makefile that automatically compiles the associated version. Set the current working directory to the desired folder and run `make`.

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
