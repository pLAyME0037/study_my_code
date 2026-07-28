import java.util.ArrayList;

class _9_simple_arr_list {
    public static void main(String[] args) {
        ArrayList<Integer> arr = new ArrayList<>();
        arr.add(10);
        arr.add(20);
        arr.add(30);

        for (Integer arrs : arr) {
            System.out.println(arrs);
        }
    }
}
