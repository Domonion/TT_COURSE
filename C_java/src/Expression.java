public class Expression implements INode {
    public Variable x;
    public INode e;
    public Expression(Variable x, INode e) {
        this.x = x;
        this.e = e;
    }

    @Override
    public String toString() {
        return "(\\" + x.toString() + '.' + e.toString() + ')';
    }
}
