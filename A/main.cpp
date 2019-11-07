//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include "LambdaParser.h"
#include "main.h"

int main() {
    //freopen("input.txt","r",stdin);
    //freopen("output.txt","w",stdout);
    iostream::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    string input;
    getline(cin, input, '{');
    cout << LambdaParser::Parse(input)->ToString();
    return 0;
}