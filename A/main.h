//
// Created by Artemiy.Kononov on 11/5/2019.
//

#ifndef A_MAIN_H
#define A_MAIN_H

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <random>
#include <vector>
#include <stack>
#include <deque>
#include <exception>
#include <queue>
#include <array>
#include <list>
#include <bitset>
#include <set>
#include <map>
#include <unordered_map>
#include <cctype>
#include <search.h>
#include <unordered_set>
#include <search.h>
#include <climits>
#include <functional>
#include <utility>
#include <string>
#include <cstring>
#include <cassert>
#include <fstream>
#include <regex>


#define TASK ""
#define forn(i, s, n) for(auto (i) = (s); (i) < (n); (i)++)
#define forb(i, s, n) for(auto (i) = (s); (i) >= (n); (i)--)
#define fora(i, a) for(auto (i) : (a))
#define foraa(i, a) for(auto & (i) : (a))
#define size(a) (int((a).size()))
#define sqr(i) ((i) * (i))
#define all(a) (a).begin(), (a).end()
#define rall(a) (a).rbegin(), (a).rbegin()
#define vec vector
#define pr pair
#define sc second
#define fs first
#define umap unordered_map
#define uset unordered_set
#define pq priority_queue
#define eb emplace_back
#define lb lower_bound
#define ub upper_bound
#define mp make_pair
#define pb pop_back
#define is(fromVariable, toType, name) toType name = dynamic_cast<toType>(fromVariable)

using namespace std;

typedef unsigned int uint;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;

thread_local mt19937 randomer;

void Initialize(){
    iostream::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    randomer.seed();
}
#endif //A_MAIN_H
