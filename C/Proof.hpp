//
// Created by domonion on 09.12.2019.
//

#include "Type.h"
#include "LambdaParser.h"

#ifndef C_PROOF_HPP
#define C_PROOF_HPP


extern int currentDepth;

struct Proof {
public:
    static void Prove(LambdaParser::Expression * expr, Type * type);
    static void Prove(LambdaParser::Use * usage, Type * type);
    static void Prove(LambdaParser::Atom * expr, Type * type);
    static void Prove(LambdaParser::Variable * expr, Type * type);
};


#endif //C_PROOF_HPP
