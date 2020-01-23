public class Use implements INode {
    public INode l, r;
    public Use(INode l, INode r){
        this.l = l;
        this.r = r;
    }

    @Override
    public String toString() {
        return "(" + l.toString() + " " + r.toString() + ")";
    }
}
