import javafx.util.Pair;

import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

public class Inference {
    private Set<Variable> variables;
    private Map<INode, IType> types;
    private Map<IType, IType> typesFromSystem;
    private List<Pair<IType, IType>> system;
    private int counter = 0;

    public Inference(Set<Variable> variables) {
        this.variables = variables;
        types = new HashMap<>();
        typesFromSystem = new HashMap<>();
        //TODO maybe better array list
        //TODO also i am not sure if there is no struct-like shit
        system = new LinkedList<>();
    }

    private String freeName() {
        return "t" + counter++;
    }

    private IType infer(INode expression) {
        if (expression instanceof Variable) {
            IType result;
            if (types.containsKey(expression)) {
                result = types.get(expression);
            } else {
                result = new Type(new Variable(freeName(), ((Variable) expression).typeInt, ((Variable) expression).can));
            }
            types.put(expression, result);
            return result;
        } else if (expression instanceof Expression) {
            Expression abstraction = (Expression) expression;
            Type t = new Type(new Variable(freeName(), abstraction.x.typeInt, abstraction.x.can));
            types.put(abstraction.x, t);
            TypeNode typeNode = new TypeNode(t, infer(abstraction.e));
            types.putIfAbsent(expression, typeNode);
            return typeNode;
        } else {
            Use use = (Use) expression;
            Type pi = new Type(new Variable(freeName(), 0, true));
            IType p = infer(use.l);
            IType r = infer(use.r);
            TypeNode typeNode = new TypeNode(r, pi);
            system.add(new Pair<>(p, typeNode));
            types.put(use, pi);
            types.putIfAbsent(use.l, p);
            types.putIfAbsent(use.r, r);
            return pi;
        }
    }

    private boolean contains(IType t, Type v) {
        if (t instanceof Type) {
            //TODO is it ok, may be equals?
            return t == v;
        } else {
            return contains(((TypeNode) t).l, v) || contains(((TypeNode) t).r, v);
        }
    }

    private boolean bad() {
        //TODO stream ok?
        return system.stream().anyMatch(e -> e.getKey() instanceof Type && e.getValue() instanceof TypeNode && contains(e.getValue(), (Type) e.getKey()));
    }

    private boolean substitute(TypeNode typeNode, IType k, IType v) {
        boolean res;
        if (typeNode.l instanceof Type) {
            //TODO what is ==?
            if (typeNode.l == k) {
                typeNode.l = v;
                res = true;
            } else {
                res = false;
            }
        } else {
            res = substitute((TypeNode) typeNode.l, k, v);
        }
        if (!res) {
            if (typeNode.r instanceof Type) {
                //TODO what is ==?
                if (typeNode.r == k) {
                    typeNode.r = v;
                    res = true;
                }
            } else {
                res = substitute((TypeNode) typeNode.r, k, v);
            }
        }
        return res;
    }

    private boolean substitute(Type k, IType v) {
        AtomicBoolean result = new AtomicBoolean(false);
        //TODO linked list?
        List<Pair<IType, IType>> toAdd = new LinkedList<>();
        List<Pair<IType, IType>> toRemove = new LinkedList<>();
        //TODO foreach ok?
        system.forEach(it -> {
            //TODO what is ==?
            if (!(it.getKey() == k && it.getValue() == v)) {
                if (it.getKey() instanceof Type) {
                    //TODO what is ==?
                    if (it.getKey() == k) {
                        toRemove.add(it);
                        toAdd.add(new Pair<>(v, it.getValue()));
                        result.set(true);
                    }
                } else if (it.getKey() instanceof TypeNode) {
                    if (substitute((TypeNode) it.getKey(), k, v)) {
                        result.set(true);
                    }
                }
                if (it.getValue() instanceof Type) {
                    //TODO what is ==?
                    if (it.getValue() == k) {
                        toRemove.add(it);
                        toAdd.add(new Pair<>(it.getKey(), v));
                        result.set(true);
                    }
                } else if (it.getValue() instanceof TypeNode) {
                    if (substitute((TypeNode) it.getValue(), k, v)) {
                        result.set(true);
                    }
                }
            }
        });
        system.addAll(toAdd);
        system.removeAll(toRemove);
        return result.get();
    }

    private void s(TypeNode typeNode, IType k, IType v) {
        if (typeNode.l instanceof Type) {
            //TODO what is ==?
            if (typeNode.l == k) {
                typeNode.l = v;
            }
        } else if (typeNode.l instanceof TypeNode) {
            substitute((TypeNode) typeNode.l, k, v);
        }
        if (typeNode.r instanceof Type) {
            //TODO what is ==?
            if (typeNode.r == k) {
                typeNode.r = v;
            }
        } else if (typeNode.r instanceof TypeNode) {
            substitute((TypeNode) typeNode.r, k, v);
        }
    }

    private boolean solve() {
        List<Pair<IType, IType>> toAdd = new LinkedList<>();
        while (true) {
            int cnt = 0;
            AtomicInteger typeCnt = new AtomicInteger();
            if (bad()) return false;
            system.addAll(toAdd);
            toAdd.clear();
            List<Pair<IType, IType>> entries = new ArrayList<>(system);
            for (Pair<IType, IType> it : entries) {
                //TODO what is !=?
                if (it.getKey() != it.getValue()) {
                    if (it.getKey() instanceof Type) {
                        typeCnt.getAndIncrement();
                        boolean success = substitute((Type) it.getKey(), it.getValue());
                        if (success) {
                            cnt++;
                            break;
                        }
                    } else if (it.getKey() instanceof TypeNode) {
                        if (it.getValue() instanceof Type) {
                            system.remove(it);
                            toAdd.add(new Pair<>(it.getValue(), it.getKey()));
                        } else if (it.getValue() instanceof TypeNode) {
                            IType l = ((TypeNode) it.getKey()).l;
                            IType r = ((TypeNode) it.getKey()).r;
                            IType a = ((TypeNode) it.getValue()).l;
                            IType b = ((TypeNode) it.getValue()).r;
                            toAdd.add(new Pair<>(l, a));
                            toAdd.add(new Pair<>(r, b));
                            system.remove(it);
                        }
                    }
                } else {
                    system.remove(it);
                }
            }
            //TODO what is ==?
            if (cnt == 0 && typeCnt.get() == system.size() + toAdd.size()) {
                return true;
            }
        }
    }

    private int getRuleNumber(INode expression) {
        if (expression instanceof Variable) {
            return 1;
        }
        if (expression instanceof Use) {
            return 2;
        }
        return 3;
    }

    private void printAnswer(INode expression, Set<Variable> context, int depth) {
        int ruleNumber = getRuleNumber(expression);
        String e = expression.toString();
        String t = types.get(expression).toString();
        StringBuilder typesStr = new StringBuilder();
        typesStr.append("*   ".repeat(depth));
        int index = 0;
        for (Variable it : context) {
            typesStr.append(it).append(" : ").append(types.get(it));
            if(index != context.size() - 1){
                typesStr.append(", ");
            }
            index++;
        }
        typesStr.append(context.isEmpty() ? "" : " ");
        System.out.println(typesStr + "|- " + e + " " + t + " [rule #" + ruleNumber + "]");
        if (expression instanceof Expression) {
            HashSet<Variable> kek = new HashSet<>(context);
            kek.add(((Expression) expression).x);
            printAnswer(((Expression) expression).e, kek, depth + 1);
        } else if (expression instanceof Use) {
            printAnswer(((Use) expression).l, context, depth + 1);
            printAnswer(((Use) expression).r, context, depth + 1);
        }
    }

    public void inference(INode expression) {
        infer(expression);
        boolean f = solve();
        if (f) {
            system.forEach(p -> typesFromSystem.put(p.getKey(), p.getValue()));
            types.keySet().forEach(k -> {
                IType t = types.get(k);
                if (typesFromSystem.containsKey(t)) {
                    types.put(k, typesFromSystem.get(t));
                }
            });
            system.forEach(k -> types.forEach((key, value) -> {
                if (value instanceof Type) {
                    //TODO what is ==?
                    if (value == k.getKey()) {
                        types.put(key, k.getValue());
                    }
                } else if (value instanceof TypeNode) {
                    s((TypeNode) value, k.getKey(), k.getValue());
                }
            }));
            printAnswer(expression, variables.stream().filter(v -> v.can).collect(Collectors.toSet()), 0);
        } else {
            System.out.println("Expression has no type");
        }
    }
}
