import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class Parser {
    public interface Token {
    }

    public class Open implements Token {
    }

    public class Close implements Token {
    }

    public class Slash implements Token {
    }

    public class Dot implements Token {
    }

    public class Eof implements Token {
    }

    public class Identifier implements Token {
        public String name;

        public Identifier(String name) {
            this.name = name;
        }
    }

    private String s;
    private char ch;
    private int i = 0;
    private int l;
    private Token cur;

    private static boolean IsEof(Token t) {
        return t instanceof Eof;
    }

    public Parser(String s) {
        this.s = s;
        ch = s.charAt(0);
        l = s.length();
        this.variables = new HashSet<>();
        next();
    }

    private Identifier identifier() {
        StringBuilder res = new StringBuilder();
        while (IsValid(ch)) {
            res.append(ch);
            move();
        }
        back();
        return new Identifier(res.toString());
    }

    private Token next() {
        while (i < l && Character.isWhitespace(s.charAt(i))) {
            move();
        }
        switch (ch) {
            case '\n':
                cur = new Eof();
                break;
            case '\\':
                cur = new Slash();
                break;
            case '(':
                cur = new Open();
                break;
            case ')':
                cur = new Close();
                break;
            case '.':
                cur = new Dot();
                break;
            default:
                cur = identifier();
                break;
        }
        if (!IsEof(cur)) move();
        return cur;
    }

    private void move() {
        i++;
        if (i == l) {
            ch = '\n';
        } else {
            ch = s.charAt(i);
        }
    }

    private void back() {
        i--;
        ch = s.charAt(i);
    }

    private boolean IsValid(char c) {
        return Character.isLetterOrDigit(c) || c == '\'';
    }

    private Set<Variable> variables;

    private INode atom(Map<String, Variable> m) {
        if (cur instanceof Open) {
            next();
            INode res = expression(m);
            next();
            return res;
        } else {
            String name = ((Identifier) cur).name;
            if (m.containsKey(name)) {
                next();
                return m.get(name);
            } else {
                Variable result = new Variable(name, 0, true);
                m.put(name, result);
                variables.add(result);
                next();
                return result;
            }
        }
    }

    private INode abstraction(Map<String, Variable> m) {
        String name = ((Identifier) next()).name;
        Variable v = new Variable(name, 1, false);
        while (variables.contains(v)) {
            v.typeInt++;
        }
        variables.add(v);
        next();
        next();
        m.put(name, v);
        Expression result = new Expression(v, expression(m));
        m.remove(name);
        return result;
    }

    private INode application(INode current, Map<String, Variable> m) {
        if (cur instanceof Close || cur instanceof Slash || cur instanceof Eof) {
            return current;
        }
        return application(new Use(current, atom(m)), m);
    }

    private INode rest(INode current, Map<String, Variable> m) {
        if (cur instanceof Close || cur instanceof Eof) {
            return current;
        }
        return new Use(current, abstraction(m));
    }

    private INode expression(Map<String, Variable> m) {
        if (cur instanceof Slash) {
            return abstraction(m);
        }
        return rest(application(atom(m), m), m);
    }

    public Set<Variable> variables() {
        return variables;
    }

    public INode parse() {
        return expression(new HashMap<>());
    }
}
