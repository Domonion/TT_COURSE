import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        Parser parser = new Parser(s.nextLine());
        Term expression = parser.parse();
        System.out.println(expression.toString());
        new Inference(parser.variables()).inference(expression);
    }
}