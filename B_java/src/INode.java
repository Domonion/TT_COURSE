import java.util.Map;

public interface INode {
    INode substitute(Variable x, INode e);
    void rename(Map<String, String> m);
    INode copy();
    INode reduce(Use expected, INode e);
}