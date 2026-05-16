class _4_static {
    public static int counter = 0;
    public static void counter() { counter++; }

    static {
        System.out.println("foo");
    }

    public static void main(String[] args) {
        System.out.println("Hello, World");

        while (counter < 12) { counter(); }
        System.out.println("number: " + counter);

        _4_static.outer_class display_nested_method = new _4_static.outer_class();
        display_nested_method.display();
    }

    static class outer_class {
        void display() {
            System.out.println("from nested class");
        }
    }
}

class two {
    public static void main(String[] args) {
        while (_4_static.counter < 5) { _4_static.counter(); }
        System.out.println("------------------------------");
        System.out.println("class two: " + _4_static.counter);
    }
}
