cd ../src/barnes_hut/omp


perf stat -d ./omp_bh ../../../data/input/5000_bodies.csv 100 0.25 8
mv output.csv 5000_bodies_100_steps_0_25_theta.csv

perf stat -d ./omp_bh ../../../data/input/5000_bodies.csv 100 0.5 8
mv output.csv 5000_bodies_100_steps_0_5_theta.csv

perf stat -d ./omp_bh ../../../data/input/5000_bodies.csv 100 1.0 8
mv output.csv 5000_bodies_100_steps_1_0_theta.csv

perf stat -d ./omp_bh ../../../data/input/10000_bodies.csv 100 0.25 8
mv output.csv 10000_bodies_100_steps_0_25_theta.csv

perf stat -d ./omp_bh ../../../data/input/10000_bodies.csv 100 0.5 8
mv output.csv 10000_bodies_100_steps_0_5_theta.csv

perf stat -d ./omp_bh ../../../data/input/10000_bodies.csv 100 1.0 8
mv output.csv 10000_bodies_100_steps_1_0_theta.csv
