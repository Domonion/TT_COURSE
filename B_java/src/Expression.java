import javafx.util.Pair;

import java.util.Map;

public class Expression implements INode {
    public INode myNode;
    private static int renameIndex = 1;
    public Variable variable;

    public Expression(INode myNode, Variable variable) {
        this.variable = variable;
        this.myNode = myNode;
    }

    @Override
    public String toString() {
        return "(\\" + variable + '.' + myNode + ')';
    }

    @Override
    public INode SyRoC(Variable variable, INode replace, Map<String, String> renameMap, boolean config) {
        String newName = "var" + renameIndex;
        renameIndex *= 3;
        renameIndex %= 1000000007;
        INode res;
        if (!renameMap.containsKey(this.variable.name)) {
            renameMap.put(this.variable.name, newName);
            res = myNode.SyRoC(variable, replace, renameMap, config);
            renameMap.remove(this.variable.name);
        } else {
            String oldName = renameMap.get(this.variable.name);
            renameMap.put(this.variable.name, newName);
            res = myNode.SyRoC(variable, replace, renameMap, config);
            renameMap.put(this.variable.name, oldName);
        }
        Variable newVar = new Variable(newName);
        return new Expression(res, newVar);
    }

    @Override
    public Pair<INode, Boolean> PerformReduction() {
        Pair<INode, Boolean> res = myNode.PerformReduction();
        myNode = res.getKey();
        return new Pair<>(this, res.getValue());
    }
}
