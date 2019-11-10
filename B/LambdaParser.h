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

        virtual bool Reduct() = 0;

        virtual std::string ToString() const = 0;

        virtual ~TreeNode() = 0;
    };

    struct Variable : public TreeNode {
    public:

        Variable(std::string_view _name);

        bool Reduct();

        static Variable *CreateUnchecked(std::string_view input);

        std::string_view const name;

        std::string ToString() const;

        ~Variable();
    };

    struct Atom : public TreeNode {
    public:

        Atom(TreeNode *_value);

        TreeNode *value;

        bool Reduct();

        static Atom *Create(std::string_view input);

        bool IsVariable() const;

        std::string ToString() const;

        ~Atom();
    };

    struct Use : public TreeNode {
    public:

        std::vector<Atom *> atoms;

        Use();

        bool Reduct();

        static Use *Create(std::string_view input);

        std::string ToString() const;

        ~Use();
    };

    struct Expression : public TreeNode {
    public:

        Use *usage;
        TreeNode *variable;
        Expression *expr;

        Expression(Use *use, Variable *var, Expression *exp);

        Expression * DeepCopy();

        bool Reduct();

        static Expression *Create(std::string_view input);

        std::string ToString() const;

        bool IsUsage() const;

        bool IsClosed() const;

        ~Expression();
    };

    static Expression *Parse(std::string &input);
};


#endif //A_LAMBDAPARSER_H
