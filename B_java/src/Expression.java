import java.util.Map;

public class Expression implements INode {
    public INode myNode;
    private static int counter = 0;
    public Variable x;

    public Expression(INode myNode, Variable x) {
        this.x = x;
        this.myNode = myNode;
    }

    @Override
    public String toString() {
        return "(\\" + x + '.' + myNode + ')';
    }

    @Override
    public INode substitute(Variable x, INode node) {
        myNode = myNode.substitute(x, node);
        return this;
    }

    @Override
    public void rename(Map<String, String> m) {
        String name = "t" + counter++;
        if (m.containsKey(x.name)) {
            String old = m.get(x.name);
            m.put(x.name, name);
            myNode.rename(m);
            m.put(x.name, old);
        } else {
            m.put(x.name, name);
            myNode.rename(m);
            m.remove(x.name);
        }
        x.name = name;
    }

    @Override
    public INode copy() {
        return new Expression(myNode.copy(), (Variable) x.copy());
    }

    @Override
    public INode reduce(Use expected, INode node) {
        myNode = myNode.reduce(expected, node);
        return this;
    }
}
