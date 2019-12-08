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
    //TODO here is problem with naming lambda parameters, i got same type for different lambda parameters with same name
    auto inferredType = res->inferenceType();
    auto type = inferredType.sc;
    auto equations = inferredType.fs;
    auto substitution = Substitution::Unificate(equations);
    if (substitution.isValid) {
        //TODO may be i should deepcopy type before going anywhere
        substitution.Substitute(type);
        cout << type->ToString();
//        auto proof = Proof::Prove(res, type);
//        proof.Write();
    } else {
        cout << "Expression has no type";
    }
    return 0;
}