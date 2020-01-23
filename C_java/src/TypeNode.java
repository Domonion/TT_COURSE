public class TypeNode implements IType {
    public IType l, r;
    public TypeNode(IType l, IType r){
        this.l = l;
        this.r = r;
    }

    @Override
    public String toString() {
        return "(" + l.toString() + " -> " + r.toString() + ")";
    }
}
