import javafx.util.Pair;

import java.util.Map;

public interface INode {
    INode SyRoC(Variable variable, INode replace, Map<String,String> renameMap, boolean config);

    Pair<INode, Boolean> PerformReduction();
}