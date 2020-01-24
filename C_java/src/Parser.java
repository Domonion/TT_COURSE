import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class Parser {

    public interface Token {
    }

    public static class Open implements Token {
    }

    public static class Close implements Token {
    }

    public static class Slash implements Token {
    }

    public static class Dot implements Token {
    }

    public static class Eof implements Token {
    }

    public static class Identifier implements Token {
    }

    private String input;
    private char currentChar;
    private int index = 0;
    private int length;
    private Token currentToken;
    private String varName;
    private Map<String, Variable> variableMap;

    private static boolean IsEof(Token token) {
        return token instanceof Eof;
    }

    public Parser(String input) {
        this.input = input;
        variableMap = new HashMap<>();
        currentChar = input.charAt(0);
        length = input.length();
        this.variables = new HashSet<>();
        nextToken();
    }

    private Identifier newIdentifier() {
        StringBuilder builder = new StringBuilder();
        while (Character.isLetterOrDigit(currentChar) || currentChar == '\'') {
            builder.append(currentChar);
            nextChar();
        }
        index--;
        currentChar = input.charAt(index);
        varName = builder.toString();
        return new Identifier();
    }

    private Token nextToken() {
        while (index < length && Character.isWhitespace(input.charAt(index))) {
            nextChar();
        }
        switch (currentChar) {
            case '\n':
                currentToken = new Eof();
                break;
            case '\\':
                currentToken = new Slash();
                break;
            case '(':
                currentToken = new Open();
                break;
            case ')':
                currentToken = new Close();
                break;
            case '.':
                currentToken = new Dot();
                break;
            default:
                currentToken = newIdentifier();
                break;
        }
        if (!IsEof(currentToken)) nextChar();
        return currentToken;
    }

    private void nextChar() {
        index++;
        if (index == length) {
            currentChar = '\n';
        } else {
            currentChar = input.charAt(index);
        }
    }

    public Set<Variable> variables;

    private INode atom() {
        if (currentToken instanceof Open) {
            nextToken();
            INode res = expression();
            nextToken();
            return res;
        } else {
            String variableName = this.varName;
            if (variableMap.containsKey(variableName)) {
                nextToken();
                return variableMap.get(variableName);
            } else {
                Variable res = new Variable(variableName, 0, true);
                variableMap.put(variableName, res);
                variables.add(res);
                nextToken();
                return res;
            }
        }
    }

    private INode abstraction() {
        nextToken();
        String variableName = this.varName;
        Variable node = new Variable(variableName, 1, false);
        while (variables.contains(node)) {
            node.typeInt++;
        }
        variables.add(node);
        nextToken();
        nextToken();
        variableMap.put(variableName, node);
        Expression res = new Expression(node, expression());
        variableMap.remove(variableName);
        return res;
    }

    private INode application(INode node) {
        if (currentToken instanceof Close || currentToken instanceof Slash || currentToken instanceof Eof) {
            return node;
        }
        return application(new Use(node, atom()));
    }

    private INode rest(INode node) {
        if (currentToken instanceof Close || currentToken instanceof Eof) {
            return node;
        }
        return new Use(node, abstraction());
    }

    public INode expression() {
        if (currentToken instanceof Slash) {
            return abstraction();
        }
        return rest(application(atom()));
    }
}
