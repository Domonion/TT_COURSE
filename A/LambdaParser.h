//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include <string>
#include <exception>

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
        std::string const name;

        Variable(std::string const &_name);

    public:
        static Variable *CreateUnchecked(std::string const &input);

        static Variable *Create(std::string const &input);

        static bool TryCreate(std::string const &input, TreeNode *&var);

        std::string const &GetName() const;

        std::string ToString() const;

        ~Variable();
    };

    struct Atom : public TreeNode {
    private:
        TreeNode *const value;

        Atom(TreeNode *const _value);

    public:
        static Atom *Create(std::string const &input);

        static bool TryCreate(std::string const &input, TreeNode *&var);

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
        static Use *Create(std::string const &input);

        static bool TryCreate(std::string const &input, TreeNode *&var);

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
        Expression(Use * use, Variable * var, Expression * exp);
    public:
        static Expression *Create(std::string const &input);

        static bool TryCreate(std::string const &input, TreeNode *&var);

        std::string ToString() const;

        Use *GetUsage() const;

        Variable *GetVariable() const;

        Expression *GetExpression() const;

        bool IsUsage() const;

        bool IsClosed() const;

        ~Expression();
    };

    static Expression *Parse(std::string input);

private:
    static void whitespaceToSpace(std::string &out);

    static void trimSpaces(std::string &out);

    struct ParserException : public std::exception {
    private:
        std::string const info;
        TreeNode const *node;
    public:
        ParserException(std::string const &_info, TreeNode const *_node);
    };
};


#endif //A_LAMBDAPARSER_H
