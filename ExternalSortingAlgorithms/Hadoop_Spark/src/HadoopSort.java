import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

import java.io.IOException;

class MapperClass extends Mapper<LongWritable, Text, Text, Text> {

    @Override
    protected void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        Text key_for_write = new Text(value.toString().substring(0, 10));
        Text value_for_write = new Text(value.toString().substring(key_for_write.getLength()));
        context.write(key_for_write, value_for_write);
    }
}

class PartitionerClass extends Partitioner<Text, Text> {

    @Override
    public int getPartition(Text key, Text value, int noOfPartitions) {
        if (noOfPartitions == 0 || noOfPartitions == 1 || key.charAt(0) < 32)
            return 0;
        else if (key.charAt(0) > 126) {
            return noOfPartitions - 1;
        } else {
            return ((int) Math.floor((key.charAt(0) - 32) / (((int) Math.ceil(95 / noOfPartitions)) + 1)));
        }
    }

}

class ReducerClass extends Reducer<Text, Text, Text, Text> {

    @Override
    protected void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {

        while(values.iterator().hasNext()) {
            Text text = values.iterator().next();
            context.write(key, new Text(text.toString().substring(1) + "\r"));
        }
    }

}

public class HadoopSort extends Configured implements Tool {

    @Override
    public int run(String[] args) throws Exception {
        int r = 0;
        if (args != null && args.length == 2) {
            Configuration hadoop_configuration = getConf();
            hadoop_configuration.set("mapred.textoutputformat.separator", " ");

            FileSystem file_system_hadoop = FileSystem.get(hadoop_configuration);
            if (file_system_hadoop.exists(new Path(args[1]))) {
                file_system_hadoop.delete(new Path(args[1]), true);
            }

            Job hadoopJob = Job.getInstance(hadoop_configuration);
            hadoopJob.setJobName("Hadoop Sort");
            hadoopJob.setJarByClass(HadoopSort.class);
            hadoopJob.setPartitionerClass(PartitionerClass.class);
            hadoopJob.setMapOutputKeyClass(Text.class);
            hadoopJob.setMapOutputValueClass(Text.class);
            hadoopJob.setMapperClass(MapperClass.class);
            hadoopJob.setReducerClass(ReducerClass.class);

            FileInputFormat.addInputPath(hadoopJob, new Path(args[0]));
            FileOutputFormat.setOutputPath(hadoopJob, new Path(args[1]));
            r = hadoopJob.waitForCompletion(true) ? 0 : 1;
        } else {
            System.out.println(" hadoop jar <JAR_FILE> <HDFS INPUT FILE PATH> <OUTPUT DIRECTORY PATH> ");
        }
        return r;
    }

    public static void main(String[] args) throws Exception {
        int r = ToolRunner.run(new Configuration(), new HadoopSort(), args);
        System.exit(r);
    }

}
