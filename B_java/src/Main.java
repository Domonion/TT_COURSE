import javafx.util.Pair;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        int m = s.nextInt();
        int k = s.nextInt();
        int iteration = 0;
        s.nextLine();
        Parser parser = new Parser(s.nextLine());
        INode root = parser.Parse();
        System.out.println(root);
        while (iteration < m) {
            Pair<INode, Boolean> res = root.PerformReduction(false);
            if (!res.getValue()) break;
            root = res.getKey();
            if (++iteration % k == 0) {
                System.out.println(root);
            }
        }
        if (iteration % k != 0) {
            System.out.println(root);
        }
    }
}
