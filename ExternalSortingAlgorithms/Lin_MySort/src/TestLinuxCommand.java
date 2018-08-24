import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class TestLinuxCommand {
    public static void main(String[] argv) throws Exception {
        String command = "valsort /input/data-2GB.in";
        Process child = Runtime.getRuntime().exec(command);

        BufferedReader reader =
                new BufferedReader(new InputStreamReader(child.getInputStream()));
        String c;
        while ((c = reader.readLine()) != null) {
            System.out.println(c);
        }
        reader.close();
    }
}