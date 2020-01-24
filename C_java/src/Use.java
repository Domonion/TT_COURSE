public class Use implements INode {
    public INode left, right;
    public Use(INode left, INode right){
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "(" + left.toString() + " " + right.toString() + ")";
    }
}
