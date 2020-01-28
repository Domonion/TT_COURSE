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
    public INode SyRoC(Variable variable, INode substitution, Map<String, String> renameMap, boolean config) {
        String name = "t" + counter++;
        INode res;
        if (renameMap.containsKey(x.name)) {
            String old = renameMap.get(x.name);
            renameMap.put(x.name, name);
            res = myNode.SyRoC(variable, substitution, renameMap, config);
            renameMap.put(x.name, old);
        } else {
            renameMap.put(x.name, name);
            res = myNode.SyRoC(variable, substitution, renameMap, config);
            renameMap.remove(x.name);
        }
        Variable newVar = new Variable(name);
        return new Expression(res, newVar);
    }

    @Override
    public INode reduce(Use expected, INode node) {
        myNode = myNode.reduce(expected, node);
        return this;
    }
}
