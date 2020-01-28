import java.util.Map;

public interface INode {
    INode SyRoC(Variable variable, INode substitution, Map<String,String> renameMap, boolean config);

    INode reduce(Use expected, INode node);
}