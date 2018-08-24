!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=./linsort20GB.log
time LC_ALL=C sort -o /tmp/output20GB.txt /input/data-20GB.in

valsort /tmp/output20GB.txt