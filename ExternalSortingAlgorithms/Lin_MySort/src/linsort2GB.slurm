!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=./linsort2GB.log
time LC_ALL=C sort -o /tmp/output2GB.txt /input/data-2GB.in

valsort /tmp/output2GB.txt