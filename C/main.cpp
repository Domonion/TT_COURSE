//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include "LambdaParser.h"
#include "main.h"

int main() {
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    iostream::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string input;
    string buffer;
    while(cin >> buffer) {
        input += buffer + ' ';
    }
    auto res = LambdaParser::Parse(input);
    auto equations = res->GetEquation();
    return 0;
}