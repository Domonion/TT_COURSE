public class Variable implements Term {
    public String name;
    public int alpha;
    public boolean free;
    public Variable(String name, int alpha, boolean free){
        this.name = name;
        this.alpha = alpha;
        this.free = free;
    }

    @Override
    public String toString() {
        return name;
    }
}
