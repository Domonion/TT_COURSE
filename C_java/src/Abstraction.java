import static util.Util.*;

public class Abstraction implements Term {
    public Variable x;
    public Term e;
    public Abstraction(Variable x, Term e) {
        this.x = x;
        this.e = e;
    }

    @Override
    public String toString() {
        return OPEN + (SLASH + x.toString()) + DOT + e.toString() + CLOSE;
    }
}
