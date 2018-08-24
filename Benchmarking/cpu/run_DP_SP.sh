srun --output=./output/cpu_DP_1thread.dat ./MyCPUBench cpu_DP_1thread.dat &
srun --output=./output/cpu_DP_2thread.dat ./MyCPUBench cpu_DP_2thread.dat &
srun --output=./output/cpu_DP_4thread.dat ./MyCPUBench cpu_DP_4thread.dat &
srun --output=./output/cpu_SP_1thread.dat ./MyCPUBench cpu_SP_1thread.dat &
srun --output=./output/cpu_SP_2thread.dat ./MyCPUBench cpu_SP_2thread.dat &
srun --output=./output/cpu_SP_4thread.dat ./MyCPUBench cpu_SP_4thread.dat &