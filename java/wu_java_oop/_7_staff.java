import java.lang.reflect.Constructor;

class _7_staff {
    public String name;
    public double salary;

    public static void main(String[] args) {
        _7_staff staff = new _7_staff();
        staff.display("Mean", 2000);
        staff.display("jinna", 1000);
        staff.display("palla", 2000);
        staff.display("tola", 2000);
    }

    public void display(String name, double salary) {
        this.name = name;
        this.salary = salary;

        System.out.format("Staff name %s with salary %.2f. %n",
                          name, salary);
    }
}
