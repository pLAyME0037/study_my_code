import java.util.LinkedList;

class LinkedListFinal {
    public static void main(String[] args) {
        LinkedList<Object> ll = new LinkedList<>();

        ll.add("Dog");
        ll.add(5);
        ll.add("Home");
        ll.add("Cat");
        ll.add(3.14);

        System.out.println(ll);
        System.out.println(ll.getFirst());
        System.out.println(ll.getLast());
    }
}
