#pragma once
#include <vector>
#include "utils.h"
#include <algorithm>
#include <iostream>

using namespace std;

const long double EPS = 0.001;



class Color {
public:
    Color(){};
    Color(unsigned int r_, unsigned int g_, unsigned int b_): r(r_), g(g_), b(b_){};
    Color(Color c, double coeff){
        r = int((double)c.r * coeff);
        g = int((double)c.g * coeff);
        b = int((double)c.b * coeff);
    }

    void add(long double a){
        r = (r + (unsigned int)a) < 255 ? (r + (unsigned int)a) : 255;
        g = (g + (unsigned int)a) < 255 ? (g + (unsigned int)a) : 255;
        b = (b + (unsigned int)a) < 255 ? (b + (unsigned int)a) : 255;
    }

    void operator +(Color c){
        r = min(r + c.r, (unsigned int)255);
        g = min(g + c.g, (unsigned int)255);
        b = min(b + c.b, (unsigned int)255);
    }

	unsigned int r, g, b;
};

class Point {
public:
    Point(long double x_, long double y_, long double z_) : x(x_), y(y_), z(z_) {};
    Point(){};
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
        std::cout << "(" << x << "; " << y << "; " << z << ")" << endl;
    }

    void make_of_len(long double l){
        normalize();
        x *= l;
        y *= l;
        z *= l;
    }

    Point operator * (long double a) {
        return Point(a * x, a * y, a * z);
    }

    Point operator + (Point other) {
        return Point(x + other.x, y + other.y, z + other.z);
    }


    Point operator - (Point other) {
        return Point(x - other.x, y - other.y, z - other.z);
    }

    long double x, y, z;
};

Point v_cross_product(Point& a, Point& b) {
    return Point(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

Point v_min(Point& a, Point& b) {
    return Point(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

long double v_dot_product(Point& a, Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

long double cosa(Point& a, Point& b){
    return v_dot_product(a, b) / (a.len() * b.len());
}

struct Ray{
    Ray(Point start_, Point dir_) : start(start_), dir(dir_) {};
    Point start;
    Point dir;
};

class GeomObj {
public:
    GeomObj(Color color_) : color(color_){}
    GeomObj(){};
    virtual pair<bool, Point> ray_intersect(Ray r) = 0;
    virtual Point normal(Point pnt) = 0;
    Color color;
};


class Sphere : public GeomObj {
public:
    Sphere(Point center_, long double r_, Color color_) : GeomObj(color_), center(center_), r(r_) {};

    Point normal(Point pnt){
        return pnt - center;
    }

    pair<bool, Point> ray_intersect(Ray ray) {
        long double t0, t1; // solutions for t if the ray intersects
        Point L = ray.start - center;
        long double a = v_dot_product(ray.dir, ray.dir);
        long double b = 2 * v_dot_product(ray.dir, L);
        long double c = v_dot_product(L, L) - r * r;

		if (!solveQuadratic(a, b, c, t0, t1)) 
			return{ false, center };

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) 
				return{ false, center }; // both t0 and t1 are negative 
		}

		return{ true, ray.start + ray.dir * t0 };
	
	}

    Point center;
    long double r;
};

class Triangle : public GeomObj {
public:
    Triangle(Point a_, Point b_, Point c_) : GeomObj(Color(0, 0, 0)), a(a_), b(b_), c(c_) {};
    Triangle(Point a_, Point b_, Point c_, Color color_) : GeomObj(color_), a(a_), b(b_), c(c_) {};

    pair<bool, Point> ray_intersect(Ray ray) {
        Point res(0, 0, 0);
		auto N = normal_vector();

		// Step 1: finding P

		// check if ray and plane are parallel ?
        long double NdotRayDirection = v_dot_product(N, ray.dir);
		if (fabs(NdotRayDirection) < EPS) 
            return{ false, res }; // they are parallel so they don't intersect !

		// compute d parameter using equation 2
        long double d = -v_dot_product(N, a);

		// compute t (equation 3)
        long double t = -(v_dot_product(N, ray.start) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) 
            return { false, Point(1, 1, 1) }; // the triangle is behind

                                 // compute the intersection Point using equation 1
		auto P = ray.start + ray.dir * t;

		// Step 2: inside-outside test

				 // edge 0
		auto edge0 = b - a;
		auto vp0 = P - a;
		auto C = v_cross_product(edge0, vp0);

		if (v_dot_product(N, C) < 0) 
			return { false, res }; // P is on the right side 

											   // edge 1
		auto edge1 = c - b;
		auto vp1 = P - b;
		C = v_cross_product(edge1, vp1);
		if (v_dot_product(N, C) < 0)  
			return { false, res }; // P is on the right side 

												// edge 2
		auto edge2 = a - c;
		auto vp2 = P - c;
		C = v_cross_product(edge2, vp2);
		if (v_dot_product(N, C) < 0) 
			return { false, res }; // P is on the right side; 

		return { true, P }; // this ray hits the triangle 
	}

    Point normal_vector() {
		auto side1 = b - a;
		auto side2 = c - a;
		auto nv = v_cross_product(side1, side2);
		nv.normalize();
		return nv;
	}

    Point normal(Point pnt){
        auto side1 = b - a;
        auto side2 = c - a;

        auto nv = v_cross_product(side1, side2);
        nv.normalize();
        return nv;
    }

    Point a, b, c;

};

class Quadrilateral : public GeomObj {
public:
    Quadrilateral(Point a_, Point b_, Point c_, Point d_) :
        GeomObj(Color(0, 0, 0)), a(a_), b(b_), c(c_), d(d_) {};
    Quadrilateral(Point a_, Point b_, Point c_, Point d_, Color color_) :
        GeomObj(color_),  a(a_), b(b_), c(c_), d(d_) {};

    Point a, b, c, d;

    Point normal(Point pnt){

        return (Triangle(a, b, d)).normal_vector();
    }

    pair<bool, Point> ray_intersect(Ray ray) {
        Triangle abd(a, b, d);
        Triangle bcd(b, c, d);
        pair<bool, Point> t1 = abd.ray_intersect(ray);
        pair<bool, Point> t2 = bcd.ray_intersect(ray);
        if (t1.first)
            return t1;
        if (t2.first)
            return t2;
        else
            return {false, Point(0, 0, 0)};
    }
};
