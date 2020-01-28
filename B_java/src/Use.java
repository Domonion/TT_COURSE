import javafx.util.Pair;

import java.util.HashMap;
import java.util.Map;

public class Use implements INode {
    public INode left, right;
    public static INode currentRedux;
    public static INode currentSyroc;

    public Use(INode left, INode right) {
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "(" + left + " " + right + ")";
    }

    @Override
    public INode SyRoC(Variable variable, INode replace, Map<String, String> renameMap) {
        return new Use(left.SyRoC(variable, replace, renameMap), right.SyRoC(variable, replace, renameMap));
    }

    @Override
    public Pair<INode, Boolean> PerformReduction(boolean config) {
        //TODO - bug: i found redux, reduced it, but changed only in my parent, others stop
        if (!config && left instanceof Expression) {
            currentSyroc = ((Expression) left).myNode.SyRoC(((Expression) left).variable, right, new HashMap<>());
            currentRedux = this;
            return new Pair<>(currentSyroc, true);
        } else if (config && this == currentRedux) {
            return new Pair<>(currentSyroc, true);
        } else {
            Pair<INode, Boolean> leftReduce = left.PerformReduction(config);
            left = leftReduce.getKey();
            config |= leftReduce.getValue();
            Pair<INode, Boolean> rightReduce = right.PerformReduction(config);
            right = rightReduce.getKey();
            config |= rightReduce.getValue();
            return new Pair<>(this, config);
        }
    }
}
