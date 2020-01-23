public class Variable implements INode {
    public String name;
    public int typeInt;
    public boolean can;
    public Variable(String name, int typeInt, boolean can){
        this.name = name;
        this.typeInt = typeInt;
        this.can = can;
    }

    @Override
    public String toString() {
        return name;
    }
}
