public class Expression implements INode {
    public INode e;
    public Variable x;
    public Expression(INode e, Variable x) {
        this.x = x;
        this.e = e;
    }

    @Override
    public String toString() {
        return "(\\" + x.toString() + '.' + e.toString() + ')';
    }
}
