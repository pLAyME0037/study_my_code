import java.util.ArrayList;

public class _8_array_list {
    public static void main(String[] args) {
        ArrayList<String> stu = new ArrayList<>();

        stu.add("Alice");
        stu.add("Bob");
        stu.add("Charile");
        stu.add("David");
        stu.addFirst("Eve");
        System.out.println(stu.indexOf("Eve"));

        stu.set(1, "Chunk");
        stu.remove(2);

        if (stu.contains("Alice")) {
            System.out.println("Alice is in array");
        } else {
            System.out.println("LOL");
        }

        System.out.println();

        for (String stus : stu) {
            System.out.println(stus); 
        }

        stu.clear();
        System.out.println();

        System.out.println(stu);
    }
}
