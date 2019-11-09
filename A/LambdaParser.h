//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include <string>
#include <exception>
#include <string_view>
#include <vector>

#ifndef A_LAMBDAPARSER_H
#define A_LAMBDAPARSER_H

struct LambdaParser {
public:
    struct TreeNode {
    public:

        virtual std::string ToString() const = 0;

        virtual ~TreeNode() = 0;
    };

    struct Variable : public TreeNode {
    private:

        Variable(std::string_view _name);

    public:

        static Variable *CreateUnchecked(std::string_view input);

        std::string_view const name;

        std::string ToString() const;

        ~Variable();
    };

    struct Atom : public TreeNode {
    private:

        Atom(TreeNode *_value);

        TreeNode *value;

    public:

        static Atom *Create(std::string_view input);

        TreeNode *Get();

        void Set(TreeNode *_value);

        bool IsVariable() const;

        std::string ToString() const;

        ~Atom();
    };

    struct Use : public TreeNode {
    private:

        std::vector<Atom *> atoms;

        Use();

    public:

        static Use *Create(std::string_view input);

        std::string ToString() const;

        std::vector<Atom *> &GetAtoms();

        ~Use();
    };

    struct Expression : public TreeNode {
    private:

        Use *usage;
        Variable *variable;
        Expression *expr;

        Expression(Use *use, Variable *var, Expression *exp);

    public:

        static Expression *Create(std::string_view input);

        std::string ToString() const;

        Use *GetUsage() const;

        void SetUsage(Use *_usage);

        Variable *GetVariable() const;

        void SetVariable(Variable *_variable);

        Expression *GetExpression() const;

        void SetExpression(Expression *_expr);

        bool IsUsage() const;

        bool IsClosed() const;

        ~Expression();
    };

    static Expression *Parse(std::string &input);
};


#endif //A_LAMBDAPARSER_H
