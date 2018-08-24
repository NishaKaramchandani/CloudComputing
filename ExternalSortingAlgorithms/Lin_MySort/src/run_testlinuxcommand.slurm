#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=testlinuxcommand.log
#SBATCH --wait-all-nodes=1
echo $SLURM_JOB_NODELIST
java TestLinuxCommand