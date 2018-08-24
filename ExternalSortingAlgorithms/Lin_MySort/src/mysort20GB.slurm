#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=mysort20GB.log
#SBATCH --wait-all-nodes=1
echo $SLURM_JOB_NODELIST
java ExternalSort /input/data-20GB.in
valsort /tmp/sortedFile.in