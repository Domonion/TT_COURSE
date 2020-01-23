import static util.Util.*;

public class Impl implements Tau {
    public Tau l, r;
    public Impl(Tau l, Tau r){
        this.l = l;
        this.r = r;
    }

    @Override
    public String toString() {
        return OPEN + l.toString() + WHITESPACE + IMPLIES + WHITESPACE + r.toString() + CLOSE;
    }
}
