//
// Created by Artemiy.Kononov on 11/5/2019.
//
#include "LambdaParser.h"
#include "main.h"

int main() {
//    freopen("input.txt","r",stdin);
//    freopen("output.txt","w",stdout);
    iostream::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
//    int m, k;
//    cin >> m >> k;
    string input;
    string buffer;
    while (cin >> buffer) {
        input += buffer;
        //TODO that is very very long(
        input += ' ';
    }
    auto res = LambdaParser::Parse(input);
//    int now = 0;
    cout << res->ToString() << '\n';
//    while (now < m && res->Reduct()) {
//        now++;
//        if (now % k == 0) {
//            cout << res->ToString() << '\n';
//        }
//    }
//    //TODO there may be questions with output
//    if (now % k) {
//        string last = res->ToString();
//        if (!res->Reduct()) {
//            cout << last << '\n';
//        }
//    }
    return 0;
}