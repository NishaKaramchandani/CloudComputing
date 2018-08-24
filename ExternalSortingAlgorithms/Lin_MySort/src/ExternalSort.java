import java.io.*;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;

public class ExternalSort {

    private static int no_of_chunks = 100;
    private static int no_of_threads = 2;

    private List<String> readFileToSort(String fileName) {
        List<String> dataToSort = new ArrayList<>();
        String readLine;
        BufferedReader br;
        File fileToSort;
        try {
            fileToSort = new File(fileName);
            br = new BufferedReader(new FileReader(fileToSort));
            while ((readLine = br.readLine()) != null)
                dataToSort.add(readLine);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return dataToSort;
    }

    private int partition(List<String> dataToSort, int low, int high) {
        String pivot = dataToSort.get(high);
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (dataToSort.get(j).compareTo(pivot) <= 0) {
                i++;
                String temp = dataToSort.get(i);
                dataToSort.set(i, dataToSort.get(j));
                dataToSort.set(j, temp);
            }
        }
        String temp = dataToSort.get(i + 1);
        dataToSort.set(i + 1, dataToSort.get(high));
        dataToSort.set(high, temp);

        return i + 1;
    }

    private void sort(List<String> dataToSort, int low, int high) {
        if (low < high) {
            int pi = partition(dataToSort, low, high);
            sort(dataToSort, low, pi - 1);
            sort(dataToSort, pi + 1, high);
        }
    }

    private static void printArray(List<String> sortedList) {
        int n = sortedList.size();
        for (int i = 0; i < n; ++i)
            System.out.print(sortedList.get(i) + " ");
        System.out.println();
    }

    private void writeToFile(String fileName, List<String> dataToWrite) {
        try {
            FileWriter filewrite = new FileWriter(new File(fileName));
            BufferedWriter bufWrite = new BufferedWriter(filewrite);
            for (String str : dataToWrite) {
                bufWrite.write(str + " " + "\n");
            }
            bufWrite.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static void readWrite(RandomAccessFile raf, BufferedOutputStream bw, long numBytes) throws IOException {
        byte[] buf = new byte[(int) numBytes];
        int val = raf.read(buf);
        if (val != -1) {
            bw.write(buf);
        }
    }

    public static int kwayMergeSortedFiles(List<File> files, File outputfile, final Comparator<String> cmp) throws IOException {
        PriorityQueue<FileBuffer> pq = new PriorityQueue<FileBuffer>(11,
                new Comparator<FileBuffer>() {
                    public int compare(FileBuffer i, FileBuffer j) {
                        return cmp.compare(i.peek(), j.peek());
                    }
                }
        );
        for (File f : files) {
            FileBuffer bfb = new FileBuffer(f);
            pq.add(bfb);
        }
        BufferedWriter fbw = new BufferedWriter(new FileWriter(outputfile));
        int rowcounter = 0;
        try {
            while (pq.size() > 0) {
                FileBuffer bfb = pq.poll();
                String r = bfb.pop();
                fbw.write(r);
                fbw.newLine();
                ++rowcounter;
                if (bfb.empty()) {
                    bfb.fbr.close();
                    bfb.originalfile.delete();// we don't need you anymore
                } else {
                    pq.add(bfb); // add it back
                }
            }
        } finally {
            fbw.close();
            for (FileBuffer bfb : pq) bfb.close();
        }
        return rowcounter;
    }

    private static void callSorting() {
        Thread[] threadArray = new Thread[no_of_threads];
        for (int i = 0; i < no_of_threads; i++) {
            threadArray[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    for (int k = 1; k <= no_of_chunks / no_of_threads; k++) {
                        ExternalSort es = new ExternalSort();

                        int chunkFileNumber = ((Integer.parseInt(Thread.currentThread().getName()) * no_of_chunks) / no_of_threads) + k;
                        //System.out.println("Input filename::" + chunkFileNames[chunkFileNumber]);

                        List<String> inputData = es.readFileToSort("/tmp/chunk" + chunkFileNumber + ".txt");
                        int n = inputData.size();

                        //ExternalSort ob = new ExternalSort();
                        es.sort(inputData, 0, n - 1);

                        es.writeToFile("/tmp/chunk" + chunkFileNumber + ".txt", inputData);
                    }
                }
            });

            threadArray[i].setName(Integer.toString(i));
            threadArray[i].start();
        }
        try {

            for (int i = 0; i < no_of_threads; i++) {
                threadArray[i].join();
            }

            Comparator<String> comparator = new Comparator<String>() {
                public int compare(String r1, String r2) {
                    return r1.compareTo(r2);
                }
            };

            List<File> files = new ArrayList<>();
            for (int i = 1; i <= no_of_chunks; i++) {
                File file = new File("/tmp/chunk" + i + ".txt");
                files.add(file);
            }

            File outputFile = new File("/tmp/sortedFile.in");

            kwayMergeSortedFiles(files, outputFile, comparator);


        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void splitInputFileToChunks(String inputFilename) {
        try {
            RandomAccessFile raf = new RandomAccessFile(inputFilename, "r");
            long sourceSize = raf.length();
            long bytesPerSplit = sourceSize / no_of_chunks;
            long remainderData = sourceSize % no_of_chunks;

            int maxReadBufferSize = 8 * 1024; //8KB
            for (int i = 1; i <= no_of_chunks; i++) {
                BufferedOutputStream bw = new BufferedOutputStream(new FileOutputStream("/tmp/chunk" + i + ".txt"));
                if (bytesPerSplit > maxReadBufferSize) {
                    long numReads = bytesPerSplit / maxReadBufferSize;
                    long numRemainingRead = bytesPerSplit % maxReadBufferSize;
                    for (int k = 0; k < numReads; k++) {
                        readWrite(raf, bw, maxReadBufferSize);
                    }
                    if (numRemainingRead > 0) {
                        readWrite(raf, bw, numRemainingRead);
                    }
                } else {
                    readWrite(raf, bw, bytesPerSplit);
                }
                bw.close();
            }
            if (remainderData > 0) {
                BufferedOutputStream bw = new BufferedOutputStream(new FileOutputStream("/tmp/chunk" + (no_of_chunks + 1) + ".txt"));
                readWrite(raf, bw, remainderData);
                bw.close();
            }
            raf.close();
            callSorting();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String args[]) {
        try {
            long start = System.currentTimeMillis();
            splitInputFileToChunks(args[0]);
            long end = System.currentTimeMillis();
            NumberFormat formatter = new DecimalFormat("#0.00000");
            System.out.println("Execution time is " + formatter.format((end - start) / 1000d) + " seconds");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}