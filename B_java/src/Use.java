import javafx.util.Pair;

import java.util.HashMap;
import java.util.Map;

public class Use implements INode {
    public INode left, right;

    public Use(INode left, INode right) {
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "(" + left + " " + right + ")";
    }

    @Override
    public INode SyRoC(Variable variable, INode replace, Map<String, String> renameMap, boolean config) {
        return new Use(left.SyRoC(variable, replace, renameMap, config), right.SyRoC(variable, replace, renameMap, config));
    }

    @Override
    public Pair<INode, Boolean> PerformReduction() {
        if (left instanceof Expression) {
            INode syRoC = ((Expression) left).myNode.SyRoC(((Expression) left).variable, right, new HashMap<>(), false);
            return new Pair<>(syRoC, true);
        } else {
            Pair<INode, Boolean> leftReduce = left.PerformReduction();
            left = leftReduce.getKey();
            if (leftReduce.getValue()) {
                return new Pair<>(this, true);
            }
            Pair<INode, Boolean> rightReduce = right.PerformReduction();
            right = rightReduce.getKey();
            return new Pair<>(this, rightReduce.getValue());
        }
    }
}
