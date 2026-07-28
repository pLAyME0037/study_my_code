import java.util.LinkedList;

public class _10_simple_ll {
    public static void main(String[] args) {
        LinkedList<Object> ll = new LinkedList<>();

        boolean is = 1 > 2;

        ll.add(1);
        ll.add("element number 2");
        ll.add(3.14);
        ll.add(is);
        ll.add('d');

        System.out.println("Elements in LinkedList:");
        System.out.println(ll);
        System.out.println();
        System.out.println("First Element: " + ll.getFirst());
        System.out.println("Last Element: " + ll.getLast());

        System.out.println(check(ll));
    }

    public static String check(LinkedList<Object> ob) {
        if (ob.getLast() instanceof Character) return "true";
        return "false";
    }
}
