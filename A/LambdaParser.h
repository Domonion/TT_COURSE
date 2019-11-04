//
// Created by Artemiy.Kononov on 11/5/2019.
//

#ifndef A_LAMBDAPARSER_H
#define A_LAMBDAPARSER_H


struct LambdaParser {
public:
    struct TreeNode;
    struct Variable;
    struct Use;
    struct Atom;
    struct Expression;
private:

    static void whitespaceToSpace(string &out) {
        foraa(i, out) {
            if (isspace(i)) {
                i = ' ';
            }
        }
    }

    static void trimSpaces(string &out) {
        out.resize(unique(all(out)) - out.begin());
    }

    struct ParserException : public exception {
    private:
        string const info;
        TreeNode const *node;
    public:
        ParserException(string const &_info, TreeNode const *_node) : exception(), info(_info), node(_node) {
        }
    };

public:

    struct TreeNode {
    public:
        virtual string const &ToString() const = 0;

        virtual ~TreeNode() = 0;
    };

    struct Variable : public TreeNode {
    private:

        string const name;

        Variable(string const &_name) : name(_name) {}

    public:

        static Variable CreateUnchecked(string const &input) {
            return Variable(input);
        }

        static Variable Create(string const &input) {
            if (!regex_match(input, Util::GetInstance().VARIABLE_REGEX)) {
                throw new ParserException(input + " is not variable", nullptr);
            }
            return Variable(input);
        }

        static bool TryCreate(string const &input, TreeNode *&var) {//TODO is it true that plain Variable * will pass?
            if (regex_match(input, Util::GetInstance().VARIABLE_REGEX)) {
                var = new Variable(input);
                return true;
            }
            return false;
        }

        string const &GetName() const {
            return name;
        }

        string const &ToString() const {
            return GetName();
        }

        ~Variable() {}
    };

    struct Atom : public TreeNode {
    private:
        TreeNode *const value;

        Atom(TreeNode *const _value) : value(_value) {}

    public:
        static bool TryCreate(string const &input, TreeNode *&var) {//TODO is it true that plain Variable * will pass?

        }

        string const &ToString() const {
            if (is(value, Expression*const, expr)) {
                return Util::GetInstance().OPEN + expr->ToString() + Util::GetInstance().CLOSE;
            } else if (is(value, Variable*const, var)) {
                return var->ToString();
            } else {
                throw new ParserException("Atom::ToString - Impossible cast", this);
            }
        }

        ~Atom() {}
    };

    //todo 2. for use stop sumbols are: \ and eof

    struct Use : public TreeNode {
    private:
        LambdaParser::Atom *const value;
        Use *const next;
    public:

    };

    struct Expression : public TreeNode {

    };
};



#endif //A_LAMBDAPARSER_H
