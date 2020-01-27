import java.util.Map;

public class Variable implements INode {
    public String name;

    public Variable(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return name;
    }

    @Override
    public INode substitute(Variable x, INode node) {
        return this.name.equals(x.name) ? node : this;
    }

    @Override
    public void rename(Map<String, String> m) {
        if (m.containsKey(name) && m.get(name) != null) {
            name = m.get(name);
        }
    }

    @Override
    public INode copy() {
        return new Variable(name);
    }

    @Override
    public INode reduce(Use expected, INode node) {
        return this;
    }
}
