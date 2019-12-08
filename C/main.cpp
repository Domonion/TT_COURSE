//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include "LambdaParser.h"
//#include "Proof.h"
#include "main.h"
#include "Substitution.h"

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    iostream::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string input;
    string buffer;
    while (cin >> buffer) {
        //TODO FUCKINLONG
        input += buffer + ' ';
    }
    auto res = LambdaParser::Parse(input);
    auto inferredType = res->inferenceType();
    auto type = inferredType.sc;
    auto equations = inferredType.fs;
    auto substitution = Substitution::Unificate(equations);
    if (substitution.isValid) {
        substitution.Substitute(type);
        cout << type->ToString();
//        auto proof = Proof::Prove(res, type);
//        proof.Write();
    } else {
        cout << "Expression has no type";
    }
    return 0;
}