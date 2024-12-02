import java.util.*;
import java.util.stream.Collectors;

class Entry {
    int id;
    boolean reversed;

    public Entry(int id, boolean reversed) {
        this.id = id;
        this.reversed = reversed;
    }
}

class SuperString {
    List<String> list;
    Stack<Entry> order;
    Boolean reversed;
    Boolean canPrint;

    public SuperString() {
        list = new ArrayList<String>();
        order = new Stack<>();
        reversed = false;
        canPrint = false;
    }

    public void append(String s) {
        canPrint = true;
        list.add(s);
        Entry current = new Entry(list.size() - 1, reversed);
        order.add(current);
    }

    public void insert(String s) {
        canPrint = true;
        list.add(0, s);
        Entry current = new Entry(0, reversed);
        order.add(current);

    }


    public Boolean contains(String s) {
        canPrint = true;
        String value = this.toString();
        int i = value.indexOf(s);
        return i != -1;
    }

    private String inverseString(String s) {
        return new StringBuilder(s).reverse().toString();
    }


    public void reverse() {
        reversed = !reversed;
        Collections.reverse(list);
        list = list.stream().map(this::inverseString).collect(Collectors.toList());
    }


    public void removeLast(int k) {
        for (int i = 0; i < k; i++) {
            int index;
            Entry current = order.pop();
            boolean order = (reversed == current.reversed);
            if (!order) {
                index = list.size() - 1 - current.id;
            } else {
                index = current.id;
            }
            list.remove(index);
        }
    }

    @Override
    public String toString() {
        if (!canPrint) return "";
        return String.join("", list) + "\n";
    }

}

public class SuperStringTest {

    public static void main(String[] args) {
        Scanner jin = new Scanner(System.in);
        int k = jin.nextInt();
        if (k == 0) {
            SuperString s = new SuperString();
            while (true) {
                int command = jin.nextInt();
                if (command == 0) {//append(String s)
                    s.append(jin.next());
                }
                if (command == 1) {//insert(String s)
                    s.insert(jin.next());
                }
                if (command == 2) {//contains(String s)
                    System.out.println(s.contains(jin.next()));
                }
                if (command == 3) {//reverse()
                    s.reverse();
                }
                if (command == 4) {//toString()
                    System.out.print(s);
                }
                if (command == 5) {//removeLast(int k)
                    s.removeLast(jin.nextInt());
                }
                if (command == 6) {//end
                    break;
                }
            }
        }
    }


}
