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
    public INode reduce(Use expected) {
        if (expected == this) {
            return ((Expression)expected.left).myNode.SyRoC(((Expression)expected.left).x, expected.right, new HashMap<>(), false);
        } else{
            left = left.reduce(expected);
            right = right.reduce(expected);
            return this;
        }
    }
}
