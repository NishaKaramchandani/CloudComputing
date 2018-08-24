srun --output=./output/memory-RWR-10000000-1thread.dat ./MyRAMBench memory-RWR-10000000-1thread.dat &
srun --output=./output/memory-RWR-10000000-2thread.dat ./MyRAMBench memory-RWR-10000000-2thread.dat &
srun --output=./output/memory-RWR-10000000-4thread.dat ./MyRAMBench memory-RWR-10000000-4thread.dat &
srun --output=./output/memory-RWS-10000000-1thread.dat ./MyRAMBench memory-RWS-10000000-1thread.dat &
srun --output=./output/memory-RWS-10000000-2thread.dat ./MyRAMBench memory-RWS-10000000-2thread.dat &
srun --output=./output/memory-RWS-10000000-4thread.dat ./MyRAMBench memory-RWS-10000000-4thread.dat &