import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.IOException;

class ReadWriteData {
    public static void main(String[] args) throws IOException {
        writeMsg("Hello, World 123\n");
        writeMsg("----------------\n");
        writeMsg("this is new line\n");
        readMsg();
    }

    public static void readMsg() throws IOException {
        FileReader fr = null;

        try {
            fr = new FileReader("./output_text.txt");

            int c;
            while((c = fr.read()) != -1) System.out.print((char)c);
        } finally {
            if (fr != null) fr.close();
        }
    }

    public static void writeMsg(String message) throws IOException {
        FileWriter fw = null;

        try {
            fw = new FileWriter("./output_text.txt", true);

            fw.append(message);
        } finally {
            if (fw != null) fw.close();
        }
    }
}
