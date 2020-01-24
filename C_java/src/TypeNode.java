public class TypeNode implements IType {
    public IType left, right;
    public TypeNode(IType left, IType right){
        this.left = left;
        this.right = right;
    }

    @Override
    public String toString() {
        return "(" + left.toString() + " -> " + right.toString() + ")";
    }
}
