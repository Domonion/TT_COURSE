import java.util.HashMap;
import java.util.Scanner;

public class Main {
    private static INode term;
    private static int counter = 0;

    private static Use findLeftMostRedux(INode t) {
        if (t instanceof Expression) {
            return findLeftMostRedux(((Expression) t).e);
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

    private static void betaReduction(int m, int k) {
        System.out.println(term);
        while (counter < m) {
            Use redux = findLeftMostRedux(term);
            if(redux == null){
                return;
            }
            Expression a = (Expression) redux.left;
            INode l = a.e.copy();
            l.rename(new HashMap<>());
            INode reduced = l.substitute(a.x, redux.right);
            term = term.reduce(redux, reduced);
            counter++;
            if(counter % k == 0){
                System.out.println(term);
            }
        }

    }

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        int m = s.nextInt();
        int k = s.nextInt();
        s.nextLine();
        Parser parser = new Parser(s.nextLine());
        term = parser.Parse();
        betaReduction(m, k);
        if(counter % k != 0){
            System.out.println(term);
        }
    }
}
