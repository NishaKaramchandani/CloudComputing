#!/bin/bash
#SBATCH --nodes=1
#SBATCH --output=spark8GB.log
#SBATCH --wait-all-nodes=1
time spark-submit --class SparkSort --master yarn --deploy-mode client --driver-memory 1g --executor-memory 1g --executor-cores 1 --num-executors 1 SparkSort.jar /input/data-8GB /user/nkaramchandani/output-spark
hadoop jar /opt/hadoop-2.9.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-2.9.0.jar teravalidate /user/nkaramchandani/output-spark /user/nkaramchandani/report-spark
hadoop fs -get /user/nkaramchandani/report-spark/part-r-00000