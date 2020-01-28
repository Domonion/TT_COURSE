import javafx.util.Pair;

import java.util.HashMap;
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
    public INode SyRoC(Variable variable, INode replace, Map<String, String> renameMap) {
        String curName;
        if (renameMap.get(name) != null) {
            curName = renameMap.get(name);
        } else {
            curName = name;
        }
        return curName.equals(variable.name) ? replace : new Variable(curName);
    }

    @Override
    public Pair<INode, Boolean> PerformReduction(boolean config) {
        return new Pair<>(this, config);
    }
}
