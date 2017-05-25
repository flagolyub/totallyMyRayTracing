#pragma once
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

const long double EPS = 0.001;

class Point {
public:
    long double x, y, z;

    Point(){};
    Point(long double x_, long double y_, long double z_) : x(x_), y(y_), z(z_) {};

    Point operator * (long double k) {
        return Point(k * x, k * y, k * z);
    }

    Point operator + (Point p) {
        return Point(x + p.x, y + p.y, z + p.z);
    }


    Point operator - (Point p) {
        return Point(x - p.x, y - p.y, z - p.z);
    }


    long double len() {
        return sqrt(x * x + y * y + z * z);
    }

    void normalize() {
        long double l = len();
        x /= l;
        y /= l;
        z /= l;
    }

    void show(){
        cout << "(" << x << "; " << y << "; " << z << ")" << endl;
    }

    void make_of_len(long double l){
        normalize();
        x *= l;
        y *= l;
        z *= l;
    }
};

long double v_dot_product(Point& a, Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point v_cross_product(Point& a, Point& b) {
    return Point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

long double cosa(Point& a, Point& b){
    return v_dot_product(a, b) / (a.len() * b.len());
}

