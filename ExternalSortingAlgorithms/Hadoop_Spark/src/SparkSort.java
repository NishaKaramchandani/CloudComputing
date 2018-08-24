import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.PairFunction;

import scala.Tuple2;


public class SparkSort {

    private static long execution_start_time;
    private static long sort_start_time;
    private static long file_read_time;
    private static long sort_time;

    private static class GetKeyPair implements PairFunction<String, String, String> {

        public Tuple2<String, String> call(String s) {
            String key = s.substring(0, 10);
            String value = s.substring(10);
            return new Tuple2<String, String>(key, value);
        }
    }

    public static void main(String[] args) {

        JavaSparkContext java_spark_context = new JavaSparkContext("local[*]", "Spark Sort");
        execution_start_time = System.currentTimeMillis();
        JavaRDD<String> lines = java_spark_context.textFile(args[0]);
        file_read_time = System.currentTimeMillis() - execution_start_time;
        System.out.println("Time taken to read input file::" + file_read_time);
        sort_start_time = System.currentTimeMillis();
        JavaPairRDD<String, String> sortedOutput = lines.mapToPair(new GetKeyPair()).sortByKey(true);
        sortedOutput.flatMap(new FlatMapFunction<Tuple2<String, String>, String>() {
            public Iterator<String> call(Tuple2<String, String> t) throws Exception {
                List<String> returnValues = new ArrayList<String>();
                returnValues.add(t._1() + "  " + t._2().trim() + "\t");
                return returnValues.iterator();
            }
        }).saveAsTextFile(args[1]);
        sort_time = System.currentTimeMillis() - sort_start_time;
        System.out.println("Time taken to sort file::" + sort_time);
    }
}