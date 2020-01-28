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
    public INode SyRoC(Variable variable, INode substitution, Map<String, String> renameMap, boolean config) {
        return new Use(left.SyRoC(variable, substitution, renameMap, config), right.SyRoC(variable, substitution, renameMap, config));
    }

    @Override
    public Pair<INode, Boolean> reduce() {
        if (left instanceof Expression) {
            INode syrocNode = ((Expression) left).myNode.SyRoC(((Expression) left).x, right, new HashMap<>(), false);
            return new Pair<>(syrocNode, true);
        } else {
            Pair<INode, Boolean> leftReduce = left.reduce();
            left = leftReduce.getKey();
            if (leftReduce.getValue()) {
                return new Pair<>(this, true);
            }
            Pair<INode, Boolean> rightReduce = right.reduce();
            right = rightReduce.getKey();
            return new Pair<>(this, rightReduce.getValue());
        }
    }
}
