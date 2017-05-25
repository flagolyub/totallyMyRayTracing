#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Color {
public:
    unsigned int r, g, b;

    Color(){};
    Color(unsigned int r_, unsigned int g_, unsigned int b_): r(r_), g(g_), b(b_){};


    void operator +(Color c){
        r = min(r + c.r, (unsigned int)255);
        g = min(g + c.g, (unsigned int)255);
        b = min(b + c.b, (unsigned int)255);
    }

    void add(long double a){
        r = min((unsigned int)a + r, (unsigned int)255);
        g = min((unsigned int)a + g, (unsigned int)255);
        b = min((unsigned int)a + b, (unsigned int)255);
    }

    void show(){
        cout << r << " " << g << " " << b << endl;
    }

    Color(Color c, double k){
        r = int((double)c.r * k);
        g = int((double)c.g * k);
        b = int((double)c.b * k);
    }
};
