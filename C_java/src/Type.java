public class Type implements Tau {
    public Variable v;
    public Type(Variable v){
        this.v = v;
    }

    @Override
    public String toString() {
        return v.toString();
    }
}
