import java.util.Map;

public class Use implements INode {
    public INode left, right;

    public Use(INode left, INode right) {
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "(" + left.toString() + " " + right.toString() + ")";
    }

    @Override
    public INode substitute(Variable x, INode e) {
        this.left = this.left.substitute(x, e);
        this.right = this.right.substitute(x, e);
        return this;
    }

    @Override
    public void rename(Map<String, String> m) {
        left.rename(m);
        right.rename(m);
    }

    @Override
    public INode copy() {
        return new Use(this.left.copy(), this.right.copy());
    }

    @Override
    public INode reduce(Use expected, INode e) {
        if (expected == this) {
            return e;
        } else{
            left = left.reduce(expected, e);
            right = right.reduce(expected, e);
            return this;
        }
    }
}
