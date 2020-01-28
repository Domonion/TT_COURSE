import javafx.util.Pair;

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
    public INode SyRoC(Variable variable, INode substitution, Map<String, String> renameMap, boolean config) {
        String curName;
        if (renameMap.containsKey(name) && renameMap.get(name) != null) {
            curName = renameMap.get(name);
        } else {
            curName = name;
        }
        return curName.equals(variable.name) ? substitution : new Variable(curName);
    }

    @Override
    public Pair<INode, Boolean> reduce() {
        return new Pair<>(this, false);
    }
}
