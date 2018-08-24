srun --output=./output/memory-RWR-1000000-1thread.dat ./MyRAMBench memory-RWR-1000000-1thread.dat &
srun --output=./output/memory-RWR-1000000-2thread.dat ./MyRAMBench memory-RWR-1000000-2thread.dat &
srun --output=./output/memory-RWR-1000000-4thread.dat ./MyRAMBench memory-RWR-1000000-4thread.dat &
srun --output=./output/memory-RWS-1000000-1thread.dat ./MyRAMBench memory-RWS-1000000-1thread.dat &
srun --output=./output/memory-RWS-1000000-2thread.dat ./MyRAMBench memory-RWS-1000000-2thread.dat &
srun --output=./output/memory-RWS-1000000-4thread.dat ./MyRAMBench memory-RWS-1000000-4thread.dat &