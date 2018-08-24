#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=hadoop20GB.log
#SBATCH --wait-all-nodes=1
time hadoop jar HadoopSort.jar HadoopSort -D mapred.reduce.tasks=1 /input/data-20GB /user/nkaramchandani/output-hadoop
hadoop jar /opt/hadoop-2.9.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.0.jar teravalidate /user/nkaramchandani/output-hadoop /user/nkaramchandani/report-hadoop
hadoop fs -get /user/nkaramchandani/report-hadoop/part-r-00000