import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;

class ReadWriteData {
    public static void main(String[] args) throws IOException {
        writeMsg("Hello, World 123");
        readMsg();
    }

    public static void readMsg() throws IOException {
        BufferedReader br = null;

        try {
            br = new BufferedReader(new FileReader("./output_text.txt"));

            String i;
            while((i = br.readLine()) != null) System.out.println(i);
        } finally {
            if (br != null) br.close();
        }
    }

    public static void writeMsg(String message) throws IOException {
        FileWriter fw = null;

        try {
            fw = new FileWriter("./output_text.txt");

            fw.write(message);
        } finally {
            if (fw != null) fw.close();
        }
    }
}
