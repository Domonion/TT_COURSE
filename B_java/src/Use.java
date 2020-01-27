import java.util.Map;

public class Use implements INode {
    public INode left, right;

    public Use(INode left, INode right) {
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "(" + left + " " + right + ")";
    }

    @Override
    public INode substitute(Variable x, INode node) {
        this.left = this.left.substitute(x, node);
        this.right = this.right.substitute(x, node);
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
    public INode reduce(Use expected, INode node) {
        if (expected == this) {
            return node;
        } else{
            left = left.reduce(expected, node);
            right = right.reduce(expected, node);
            return this;
        }
    }
}
