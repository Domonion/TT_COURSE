import java.util.Scanner;

public class Main {

    private static Use findLeftMostRedux(INode t) {
        if (t instanceof Expression) {
            return findLeftMostRedux(((Expression) t).myNode);
        } else if (t instanceof Variable) {
            return null;
        } else {
            Use use = (Use) t;
            if (use.left instanceof Expression) {
                return use;
            } else {
                Use res = findLeftMostRedux(use.left);
                if (res == null) {
                    res = findLeftMostRedux(use.right);
                }
                return res;
            }
        }
    }

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        int m = s.nextInt();
        int k = s.nextInt();
        int counter = 0;
        s.nextLine();
        Parser parser = new Parser(s.nextLine());
        INode term = parser.Parse();
        System.out.println(term);
        while (counter < m) {
            Use redux = findLeftMostRedux(term);
            if (redux == null) {
                break;
            }
            term = term.reduce(redux);
            if (++counter % k == 0) {
                System.out.println(term);
            }
        }
        if (counter % k != 0) {
            System.out.println(term);
        }
    }
}
