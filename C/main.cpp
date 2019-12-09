//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include "LambdaParser.h"
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
        input += buffer + ' ';
    }
    auto res = LambdaParser::Parse(input);
    auto inferredType = res->inferenceType();
    auto equations = inferredType.fs;
    auto substitution = Substitution::Unificate(equations);
    if (substitution.isValid) {
        bool first = true;
        foraa(i, mapper) {
            if(!first){
                openVars += ", " + i.sc->ToString() + " : " + withSubstituition(i.sc, substitution)->ToString();
            } else{
                openVars = i.sc->ToString() + " : " + withSubstituition(i.sc, substitution)->ToString();
                first = false;
            }
        }
        res->Prove(substitution);
        std::reverse(all(answer));
        foraa(i, answer) {
            cout << i << '\n';
        }
    } else {
        cout << "Expression has no type";
    }
    return 0;
}