public class Variable implements INode {
    public int typeInt;
    public boolean can;
    public String name;
    public Variable(int typeInt, String name, boolean can){
        this.name = name;
        this.typeInt = typeInt;
        this.can = can;
    }

    @Override
    public String toString() {
        return name;
    }
}
