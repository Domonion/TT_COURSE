import javafx.util.Pair;

import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

public class Inference {
    private Set<Variable> vars;
    private Map<INode, IType> types;
    private Map<IType, IType> newTypes;
    private List<Pair<IType, IType>> typesList;
    private int typesCnt = 0;

    public Inference(Set<Variable> vars) {
        this.vars = vars;
        types = new HashMap<>();
        newTypes = new HashMap<>();
        //TODO maybe better array list
        //TODO also i am not sure if there is no struct-like shit
        typesList = new LinkedList<>();
    }

    private IType DoInference(INode node) {
        if (node instanceof Variable) {
            IType res;
            if (types.containsKey(node)) {
                res = types.get(node);
            } else {
                res = new Type(new Variable(((Variable) node).typeInt, "t" + typesCnt++, ((Variable) node).can));
            }
            types.put(node, res);
            return res;
        } else if (node instanceof Expression) {
            Expression now = (Expression) node;
            Type type = new Type(new Variable(now.x.typeInt, "t" + typesCnt++, now.x.can));
            types.put(now.x, type);
            TypeNode typeNode = new TypeNode(type, DoInference(now.e));
            types.putIfAbsent(node, typeNode);
            return typeNode;
        } else {
            Use use = (Use) node;
            Type inferredType = new Type(new Variable(0, "t" + typesCnt++, true));
            IType leftType = DoInference(use.left);
            IType rightType = DoInference(use.right);
            TypeNode typeNode = new TypeNode(rightType, inferredType);
            typesList.add(new Pair<>(leftType, typeNode));
            types.put(use, inferredType);
            types.putIfAbsent(use.left, leftType);
            types.putIfAbsent(use.right, rightType);
            return inferredType;
        }
    }

    private boolean Has(IType type, Type whatToFind) {
        if (type instanceof Type) {
            //TODO is it ok, may be equals?
            return type == whatToFind;
        } else {
            return Has(((TypeNode) type).left, whatToFind) || Has(((TypeNode) type).right, whatToFind);
        }
    }

    private boolean Check() {
        return typesList.stream().anyMatch(e -> e.getKey() instanceof Type && e.getValue() instanceof TypeNode && Has(e.getValue(), (Type) e.getKey()));
    }

    private boolean Replace(TypeNode typeNode, IType toFind, IType toReplace) {
        boolean res;
        if (typeNode.left instanceof Type) {
            //TODO what is ==?
            if (typeNode.left == toFind) {
                typeNode.left = toReplace;
                res = true;
            } else {
                res = false;
            }
        } else {
            res = Replace((TypeNode) typeNode.left, toFind, toReplace);
        }
        if (!res) {
            if (typeNode.right instanceof Type) {
                //TODO what is ==?
                if (typeNode.right == toFind) {
                    typeNode.right = toReplace;
                    res = true;
                }
            } else {
                res = Replace((TypeNode) typeNode.right, toFind, toReplace);
            }
        }
        return res;
    }

    private boolean Replace(Type toFind, IType toReplace) {
        //TODO comon atomic?
        AtomicBoolean result = new AtomicBoolean(false);
        //TODO linked list?
        List<Pair<IType, IType>> addition = new LinkedList<>();
        List<Pair<IType, IType>> removement = new LinkedList<>();
        //TODO foreach ok?
        typesList.forEach(current -> {
            //TODO what is ==?
            if (!(current.getKey() == toFind && current.getValue() == toReplace)) {
                if (current.getKey() instanceof Type) {
                    //TODO what is ==?
                    if (current.getKey() == toFind) {
                        removement.add(current);
                        addition.add(new Pair<>(toReplace, current.getValue()));
                        result.set(true);
                    }
                } else if (current.getKey() instanceof TypeNode) {
                    if (Replace((TypeNode) current.getKey(), toFind, toReplace)) {
                        result.set(true);
                    }
                }
                if (current.getValue() instanceof Type) {
                    //TODO what is ==?
                    if (current.getValue() == toFind) {
                        removement.add(current);
                        addition.add(new Pair<>(current.getKey(), toReplace));
                        result.set(true);
                    }
                } else if (current.getValue() instanceof TypeNode) {
                    if (Replace((TypeNode) current.getValue(), toFind, toReplace)) {
                        result.set(true);
                    }
                }
            }
        });
        typesList.addAll(addition);
        typesList.removeAll(removement);
        return result.get();
    }

    private void Start(TypeNode typeNode, IType toFind, IType toReplace) {
        if (typeNode.left instanceof Type) {
            //TODO what is ==?
            if (typeNode.left == toFind) {
                typeNode.left = toReplace;
            }
        } else if (typeNode.left instanceof TypeNode) {
            Replace((TypeNode) typeNode.left, toFind, toReplace);
        }
        if (typeNode.right instanceof Type) {
            //TODO what is ==?
            if (typeNode.right == toFind) {
                typeNode.right = toReplace;
            }
        } else if (typeNode.right instanceof TypeNode) {
            Replace((TypeNode) typeNode.right, toFind, toReplace);
        }
    }

    private boolean Unificate() {
        List<Pair<IType, IType>> addition = new LinkedList<>();
        while (true) {
            int counter = 0;
            //TODO common atomic?
            AtomicInteger typeCount = new AtomicInteger();
            if (Check()) return false;
            typesList.addAll(addition);
            addition.clear();
            List<Pair<IType, IType>> entries = new ArrayList<>(typesList);
            for (Pair<IType, IType> entry : entries) {
                //TODO what is !=?
                if (entry.getKey() != entry.getValue()) {
                    if (entry.getKey() instanceof Type) {
                        typeCount.getAndIncrement();
                        if (Replace((Type) entry.getKey(), entry.getValue())) {
                            counter++;
                            break;
                        }
                    } else if (entry.getKey() instanceof TypeNode) {
                        if (entry.getValue() instanceof Type) {
                            typesList.remove(entry);
                            addition.add(new Pair<>(entry.getValue(), entry.getKey()));
                        } else if (entry.getValue() instanceof TypeNode) {
                            IType l = ((TypeNode) entry.getKey()).left;
                            IType r = ((TypeNode) entry.getKey()).right;
                            IType a = ((TypeNode) entry.getValue()).left;
                            IType b = ((TypeNode) entry.getValue()).right;
                            addition.add(new Pair<>(l, a));
                            addition.add(new Pair<>(r, b));
                            typesList.remove(entry);
                        }
                    }
                } else {
                    typesList.remove(entry);
                }
            }
            //TODO what is ==?
            if (counter == 0 && typeCount.get() == typesList.size() + addition.size()) {
                return true;
            }
        }
    }

    //kek lol russkie
    private int GetRuleNumber(INode нода) {
        if (нода instanceof Variable) {
            return 1;
        }
        if (нода instanceof Use) {
            return 2;
        }
        return 3;
    }

    private void PrintAnswer(INode expression, Set<Variable> context, int depth) {
        int ruleNumber = GetRuleNumber(expression);
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
            PrintAnswer(((Expression) expression).e, kek, depth + 1);
        } else if (expression instanceof Use) {
            PrintAnswer(((Use) expression).left, context, depth + 1);
            PrintAnswer(((Use) expression).right, context, depth + 1);
        }
    }

    public void Infer(INode expression) {
        DoInference(expression);
        if (Unificate()) {
            typesList.forEach(pair -> newTypes.put(pair.getKey(), pair.getValue()));
            types.keySet().forEach(current -> {
                IType type = types.get(current);
                if (newTypes.containsKey(type)) {
                    types.put(current, newTypes.get(type));
                }
            });
            typesList.forEach(pair -> types.forEach((key, value) -> {
                if (value instanceof Type) {
                    //TODO what is ==?
                    if (value == pair.getKey()) {
                        types.put(key, pair.getValue());
                    }
                } else if (value instanceof TypeNode) {
                    Start((TypeNode) value, pair.getKey(), pair.getValue());
                }
            }));
            PrintAnswer(expression, vars.stream().filter(cur -> cur.can).collect(Collectors.toSet()), 0);
        } else {
            System.out.println("Expression has no type");
        }
    }
}
