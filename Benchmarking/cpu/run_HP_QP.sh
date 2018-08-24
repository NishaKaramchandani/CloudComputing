srun --output=./output/cpu_HP_1thread.dat ./MyCPUBench cpu_HP_1thread.dat &
srun --output=./output/cpu_HP_2thread.dat ./MyCPUBench cpu_HP_2thread.dat &
srun --output=./output/cpu_HP_4thread.dat ./MyCPUBench cpu_HP_4thread.dat &
srun --output=./output/cpu_QP_1thread.dat ./MyCPUBench cpu_QP_1thread.dat &
srun --output=./output/cpu_QP_2thread.dat ./MyCPUBench cpu_QP_2thread.dat &
srun --output=./output/cpu_QP_4thread.dat ./MyCPUBench cpu_QP_4thread.dat &