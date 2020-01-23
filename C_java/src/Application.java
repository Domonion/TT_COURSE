import static util.Util.*;

public class Application implements Term {
    public Term l, r;
    public Application(Term l, Term r){
        this.l = l;
        this.r = r;
    }

    @Override
    public String toString() {
        return OPEN + l.toString() + WHITESPACE + r.toString() + CLOSE;
    }
}
