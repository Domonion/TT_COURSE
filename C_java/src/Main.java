import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        Parser parser = new Parser(s.nextLine());
        INode expression = parser.parse();
        System.out.println(expression.toString());
        Inference inferer = new Inference(parser.variables());
        inferer.inference(expression);
    }
}