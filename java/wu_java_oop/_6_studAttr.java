class _6_studAttr {
    public String name = null;
    public int    age  = 0;

    public static void main(String[] args) {
        _6_studAttr stud = new _6_studAttr();
        stud.display("Mean", 22);
        stud.display("Jea", 20);
        stud.display("bo", 40);
        stud.display("neary", 10);
        stud.display("Tola", 40);
    }

    public void display(String name, int age) {
        this.name = name;
        this.age  = age;

        System.out.format("Student name %s. Student is %d years old. %n",
                          name, age);
    }
}
