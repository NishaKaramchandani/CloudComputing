#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=mysort2GB.log
#SBATCH --wait-all-nodes=1
echo $SLURM_JOB_NODELIST
java ExternalSort /input/data-2GB.in
valsort /tmp/sortedFile.in