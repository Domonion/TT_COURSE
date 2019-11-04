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
        virtual std::string const &ToString() const = 0;

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

        std::string const &ToString() const;

        ~Variable();
    };

    struct Atom : public TreeNode {
    private:
        TreeNode *const value;

        Atom(TreeNode *const _value);

    public:
        static Atom *Create(std::string const &input);

        static bool TryCreate(std::string const &input, TreeNode *&var);

        std::string const &ToString() const;

        TreeNode *GetValue() const;

        ~Atom();
    };

    struct Use : public TreeNode {
    private:
        Atom *const value;
        Use *const next;

        Use(Atom *const _value, Use *const next);

    public:
        static Atom *Create(std::string const &input);

        static bool TryCreate(std::string const &input, TreeNode *&var);

        std::string const &ToString() const;

        Atom *GetValue() const;

        Use *GetNext() const;

        ~Use();
    };

    struct Expression : public TreeNode {
    private:
        
    public:
        static Expression *Create(std::string const &input);
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
