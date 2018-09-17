#ifndef UTILS_H
#define	UTILS_H

#include<bits/stdc++.h>

using namespace std;

int getNearestInteger(double a);

pair<int,int> cartesianToHex(int x, int y, int rows);

pair<int,int> hexToCartesian(int a,int b,int rows);

vector<string> split(string str, char delimiter);

#endif
