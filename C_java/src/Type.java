public class Type implements IType {
    public Variable v;
    public Type(Variable v){
        this.v = v;
    }

    @Override
    public String toString() {
        return v.toString();
    }
}
