//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include <string>
#include <exception>
#include <string_view>

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
        std::string_view const name;

        Variable(std::string_view _name);

    public:
        static Variable *CreateUnchecked(std::string_view input);

        static Variable* CreateChecked(std::string_view input);

        std::string GetName() const;

        std::string ToString() const;

        ~Variable();
    };

    struct Atom : public TreeNode {
    private:
        TreeNode *const value;

        Atom(TreeNode *const _value);

    public:

        static Atom* Create(std::string_view input);

        bool IsVariable() const;

        std::string ToString() const;

        TreeNode *GetValue() const;

        ~Atom();
    };

    struct Use : public TreeNode {
    private:
        Atom *const value;
        Use *const next;

        Use(Atom *const _value, Use *const _next);

    public:

        static Use* Create(std::string_view input);

        std::string ToString() const;

        Atom *GetValue() const;

        bool IsLast() const;

        Use *GetNext() const;

        ~Use();
    };

    struct Expression : public TreeNode {
    private:
        Use *const usage;
        Variable *const variable;
        Expression *const expr;

        Expression(Use *use, Variable *var, Expression *exp);

    public:

        static Expression * Create(std::string_view input);

        std::string ToString() const;

        Use *GetUsage() const;

        Variable *GetVariable() const;

        Expression *GetExpression() const;

        bool IsUsage() const;

        bool IsClosed() const;

        ~Expression();
    };

    static Expression *Parse(std::string& input);

private:

    static void whitespaceToSpace(std::string &out);

    static std::string_view trimSpaces(std::string const &out);

    static std::string_view trimSpaces(std::string_view const & out);

    static void uniqueSpaces(std::string &out);
};


#endif //A_LAMBDAPARSER_H
