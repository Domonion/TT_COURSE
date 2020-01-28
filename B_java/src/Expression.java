import javafx.util.Pair;

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
//        counter*= 13;
//        counter %= 1000000007;
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
    public Pair<INode, Boolean> reduce() {
        Pair<INode, Boolean> res = myNode.reduce();
        myNode = res.getKey();
        return new Pair<>(this, res.getValue());
    }
}
