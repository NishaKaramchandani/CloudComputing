To run hadoop sort:


Step 1: Compile java classes and create jar file with .class files.


$ javac -classpath $(hadoop classpath) HadoopSort.java
$ jar cvf HadoopSort.jar *.class


Step 2: Use slurm scripts available in the folder to schedule sorting jobs.


sbatch hadoop8GB.slurm
sbatch hadoop20GB.slurm
sbatch hadoop80GB.slurm


To run spark sort:


Step 1: Compile java classes and create jar file with .class files.


$ javac -classpath /opt/spark-2.3.0-bin-hadoop2.7/jars/spark-core_2.11-2.3.0.jar:/opt/spark-2.3.0-bin-hadoop2.7/jars/spark-sql_2.11-2.3.0.jar:/opt/spark-2.3.0-bin-hadoop2.7/jars/scala-compiler-2.11.8.jar:/opt/spark-2.3.0-bin-hadoop2.7/jars/scala-library-2.11.8.jar SparkSort.java
$ jar cvf SparkSort.jar SparkSort.class 


Step 2: Use slurm scripts available in the folder to schedule sorting jobs.


sbatch spark8GB.slurm
sbatch spark20GB.slurm
sbatch spark80GB.slurm