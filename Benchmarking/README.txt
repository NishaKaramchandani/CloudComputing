CPU:


nkaramchandani/cs553-pa1/cpu/
1. Make all to compile .c file
2. ./run_DP_SP.sh To run slurm jobs for Double Precision and Single Precision in background
3. ./run_HP_QP.sh To run slurm jobs for Half Precision and Quarter Precision in background


Memory:


nkaramchandani/cs553-pa1/memory/
Throughput:
1. Make all to compile .c file
2. ./run_1KB.sh To run slurm jobs for 1KB to measure throughput in background
3. ./run_1MB.sh To run slurm jobs for 1MB to measure throughput in background
4. ./run_10MB.sh To run slurm jobs for 10MB to measure throughput in background


Latency:
1. Make all to compile .c file
2. ./run_1B.sh To run slurm jobs for 1B to measure latency in background


Disk:


nkaramchandani/cs553-pa1/disk/
Throughput:
1. Make all to compile .c file
2. ./run_RR.sh To run slurm jobs for Read Random to measure throughput in background
3. ./run_RS.sh To run slurm jobs for Read Sequential to measure throughput in background
4. ./run_WR.sh To run slurm jobs for Write Random to measure throughput in background
5. ./run_WS.sh To run slurm jobs for Write Sequential to measure throughput in background


Latency:
1. Make all to compile .c file
2. ./run_RR_1KB.sh To run slurm jobs for Read Random 1 KB to measure latency in background
3. ./run_WR_1KB.sh To run slurm jobs for Write Random 1 KB to measure latency in background