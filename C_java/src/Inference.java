import javafx.util.Pair;

import java.util.*;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

public class Inference {
    private Set<Variable> variables;
    private Map<Term, Tau> types;
    private Map<Tau, Tau> typesFromSystem;
    private List<Pair<Tau, Tau>> system;
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

    private Tau infer(Term expression) {
        if (expression instanceof Variable) {
            Tau result;
            if (types.containsKey(expression)) {
                result = types.get(expression);
            } else {
                result = new Type(new Variable(freeName(), ((Variable) expression).alpha, ((Variable) expression).free));
            }
            types.put(expression, result);
            return result;
        } else if (expression instanceof Abstraction) {
            Abstraction abstraction = (Abstraction) expression;
            Type t = new Type(new Variable(freeName(), abstraction.x.alpha, abstraction.x.free));
            types.put(abstraction.x, t);
            Impl impl = new Impl(t, infer(abstraction.e));
            types.putIfAbsent(expression, impl);
            return impl;
        } else {
            Application application = (Application) expression;
            Type pi = new Type(new Variable(freeName(), 0, true));
            Tau p = infer(application.l);
            Tau r = infer(application.r);
            Impl impl = new Impl(r, pi);
            system.add(new Pair<>(p, impl));
            types.put(application, pi);
            types.putIfAbsent(application.l, p);
            types.putIfAbsent(application.r, r);
            return pi;
        }
    }

    private boolean contains(Tau t, Type v) {
        if (t instanceof Type) {
            //TODO is it ok, may be equals?
            return t == v;
        } else {
            return contains(((Impl) t).l, v) || contains(((Impl) t).r, v);
        }
    }

    private boolean bad() {
        //TODO stream ok?
        return system.stream().anyMatch(e -> e.getKey() instanceof Type && e.getValue() instanceof Impl && contains(e.getValue(), (Type) e.getKey()));
    }

    private boolean substitute(Impl impl, Tau k, Tau v) {
        boolean res;
        if (impl.l instanceof Type) {
            //TODO what is ==?
            if (impl.l == k) {
                impl.l = v;
                res = true;
            } else {
                res = false;
            }
        } else {
            res = substitute((Impl) impl.l, k, v);
        }
        if (!res) {
            if (impl.r instanceof Type) {
                //TODO what is ==?
                if (impl.r == k) {
                    impl.r = v;
                    res = true;
                }
            } else {
                res = substitute((Impl) impl.r, k, v);
            }
        }
        return res;
    }

    private boolean substitute(Type k, Tau v) {
        AtomicBoolean result = new AtomicBoolean(false);
        //TODO linked list?
        List<Pair<Tau, Tau>> toAdd = new LinkedList<>();
        List<Pair<Tau, Tau>> toRemove = new LinkedList<>();
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
                } else if (it.getKey() instanceof Impl) {
                    if (substitute((Impl) it.getKey(), k, v)) {
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
                } else if (it.getValue() instanceof Impl) {
                    if (substitute((Impl) it.getValue(), k, v)) {
                        result.set(true);
                    }
                }
            }
        });
        system.addAll(toAdd);
        system.removeAll(toRemove);
        return result.get();
    }

    private void s(Impl impl, Tau k, Tau v) {
        if (impl.l instanceof Type) {
            if (impl.l == k) {
                impl.l = v;
            }
        } else if (impl.l instanceof Impl) {
            substitute((Impl) impl.l, k, v);
        }
        if (impl.r instanceof Type) {
            if (impl.r == k) {
                impl.r = v;
            }
        } else if (impl.r instanceof Impl) {
            substitute((Impl) impl.r, k, v);
        }
    }

    private boolean solve() {
        List<Pair<Tau, Tau>> toAdd = new LinkedList<>();
        while (true) {
            int cnt = 0;
            AtomicInteger typeCnt = new AtomicInteger();
            if (bad()) return false;
            system.addAll(toAdd);
            toAdd.clear();
            List<Pair<Tau, Tau>> entries = new ArrayList<>(system);
            for (Pair<Tau, Tau> it : entries) {
                if (it.getKey() != it.getValue()) {
                    if (it.getKey() instanceof Type) {
                        typeCnt.getAndIncrement();
                        boolean success = substitute((Type) it.getKey(), it.getValue());
                        if (success) {
                            cnt++;
                            break;
                        }
                    } else if (it.getKey() instanceof Impl) {
                        if (it.getValue() instanceof Type) {
                            system.remove(it);
                            toAdd.add(new Pair<>(it.getValue(), it.getKey()));
                        } else if (it.getValue() instanceof Impl) {
                            Tau l = ((Impl) it.getKey()).l;
                            Tau r = ((Impl) it.getKey()).r;
                            Tau a = ((Impl) it.getValue()).l;
                            Tau b = ((Impl) it.getValue()).r;
                            toAdd.add(new Pair<>(l, a));
                            toAdd.add(new Pair<>(r, b));
                            system.remove(it);
                        }
                    }
                } else {
                    system.remove(it);
                }
            }
            if (cnt == 0 && typeCnt.get() == system.size() + toAdd.size()) {
                return true;
            }
        }
    }

    private int getRuleNumber(Term expression) {
        if (expression instanceof Variable) {
            return 1;
        }
        if (expression instanceof Application) {
            return 2;
        }
        return 3;
    }

    private void printAnswer(Term expression, Set<Variable> context, int depth) {
        int ruleNumber = getRuleNumber(expression);
        String e = expression.toString();
        String t = types.get(expression).toString();
        StringBuilder typesStr = new StringBuilder();
        typesStr.append("*   ".repeat(depth));
        //TODO zapataya
        context.forEach(it -> typesStr.append(it).append(" : ").append(types.get(it)).append(", "));
        typesStr.append(context.isEmpty() ? "" : " ");
        System.out.println(typesStr + "|- " + e + " " + t + " [rule #" + ruleNumber + "]");
        if (expression instanceof Abstraction) {
            HashSet<Variable> kek = new HashSet<>(context);
            kek.add(((Abstraction) expression).x);
            printAnswer(((Abstraction) expression).e, kek, depth + 1);
        } else if (expression instanceof Application) {
            printAnswer(((Application) expression).l, context, depth + 1);
            printAnswer(((Application) expression).r, context, depth + 1);
        }
    }

    public void inference(Term expression) {
        infer(expression);
        boolean f = solve();
        if (f) {
            system.forEach(p -> typesFromSystem.put(p.getKey(), p.getValue()));
            types.keySet().forEach(k -> {
                Tau t = types.get(k);
                if (typesFromSystem.containsKey(t)) {
                    types.put(k, typesFromSystem.get(t));
                }
            });
            system.forEach(k -> types.forEach((key, value) -> {
                if (value instanceof Type) {
                    if (value == k.getKey()) {
                        types.put(key, k.getValue());
                    }
                } else if (value instanceof Impl) {
                    s((Impl) value, k.getKey(), k.getValue());
                }
            }));
            printAnswer(expression, variables.stream().filter(v -> v.free).collect(Collectors.toSet()), 0);
        } else {
            System.out.println("Expression has no type");
        }
    }
}
