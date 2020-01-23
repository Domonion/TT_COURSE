import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static util.Util.*;

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
            case EOF:
                cur = new Eof();
                break;
            case SLASH:
                cur = new Slash();
                break;
            case OPEN:
                cur = new Open();
                break;
            case CLOSE:
                cur = new Close();
                break;
            case DOT:
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
            ch = EOF;
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

    private Term atom(Map<String, Variable> m) {
        if (cur instanceof Open) {
            next();
            Term res = expression(m);
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

    private Term abstraction(Map<String, Variable> m) {
        String name = ((Identifier) next()).name;
        Variable v = new Variable(name, 1, false);
        while (variables.contains(v)) {
            v.alpha++;
        }
        variables.add(v);
        next();
        next();
        m.put(name, v);
        Abstraction result = new Abstraction(v, expression(m));
        m.remove(name);
        return result;
    }

    private Term application(Term current, Map<String, Variable> m) {
        if (cur instanceof Close || cur instanceof Slash || cur instanceof Eof) {
            return current;
        }
        return application(new Application(current, atom(m)), m);
    }

    private Term rest(Term current, Map<String, Variable> m) {
        if (cur instanceof Close || cur instanceof Eof) {
            return current;
        }
        return new Application(current, abstraction(m));
    }

    private Term expression(Map<String, Variable> m) {
        if (cur instanceof Slash) {
            return abstraction(m);
        }
        return rest(application(atom(m), m), m);
    }

    public Set<Variable> variables() {
        return variables;
    }

    public Term parse() {
        return expression(new HashMap<>());
    }
}
