srun --output=./output/disk-WR-1-1thread.dat ./MyDiskBench disk-WR-1-1thread.dat &
srun --output=./output/disk-WR-1-2thread.dat ./MyDiskBench disk-WR-1-2thread.dat &
srun --output=./output/disk-WR-1-4thread.dat ./MyDiskBench disk-WR-1-4thread.dat &
srun --output=./output/disk-WR-1-8thread.dat ./MyDiskBench disk-WR-1-8thread.dat &
srun --output=./output/disk-WR-1-16thread.dat ./MyDiskBench disk-WR-1-16thread.dat &
srun --output=./output/disk-WR-1-32thread.dat ./MyDiskBench disk-WR-1-32thread.dat &
srun --output=./output/disk-WR-1-64thread.dat ./MyDiskBench disk-WR-1-64thread.dat &
srun --output=./output/disk-WR-1-128thread.dat ./MyDiskBench disk-WR-1-128thread.dat &