import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        Parser parser = new Parser(s.nextLine());
        INode root = parser.expression();
        Inference inferer = new Inference(parser.variables);
        inferer.Infer(root);
    }
}