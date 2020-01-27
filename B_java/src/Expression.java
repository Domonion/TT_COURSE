import java.util.Map;

public class Expression implements INode {
    public INode e;
    private static int counter = 0;
    public Variable x;
    public Expression(INode e, Variable x) {
        this.x = x;
        this.e = e;
    }

    @Override
    public String toString() {
        return "(\\" + x.toString() + '.' + e.toString() + ')';
    }

    @Override
    public INode substitute(Variable x, INode e) {
        this.e = this.e.substitute(x, e);
        return this;
    }

    @Override
    public void rename(Map<String, String> m) {
        String name = "t" + counter++;
        if(m.containsKey(x.name)){
            String old = m.get(x.name);
            m.put(x.name, name);
            e.rename(m);
            m.put(x.name, old);
        } else{
            m.put(x.name, name);
            e.rename(m);
            m.remove(x.name);
        }
        x.name = name;
    }

    @Override
    public INode copy() {
        return new Expression(this.e.copy(), (Variable) this.x.copy());
    }

    @Override
    public INode reduce(Use expected, INode e) {
        this.e = this.e.reduce(expected, e);
        return this;
    }
}
