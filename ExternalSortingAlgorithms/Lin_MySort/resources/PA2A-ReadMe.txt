The source code for ExternalSort is written in java and can be found in the ExternalSort.java file on GIT repository. To compile the file:


javac ExternalSort.java.


To run the file we need to provide the input file name as a command line argument. The slurm job file had the java command and valsort command for 2 different input files to ExternalSort. Also, similar two slurm jobs are written for sort command in Linux.


Following are the list of commands to run ExternalSort and linsort:
sbatch mysort2GB.slurm
sbatch mysort20GB.slurm
sbatch linsort2GB.slurm
sbatch linsort20GB.slurm


The log files are:
mysort2GB.log
mysort20GB.log
linsort2GB.log
linsort20GB.log