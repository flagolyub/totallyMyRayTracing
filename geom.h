#pragma once
#include <vector>
#include <math.h>

using namespace std;

const double EPS = 0.001;

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + sqrt(discr)) :
			-0.5 * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}



class RGBColor {
public:
    RGBColor() {}
    RGBColor(unsigned int r_, unsigned int g_, unsigned int b_): r(r_), g(g_), b(b_){}
	unsigned int r, g, b;
    void add(double a){

            r = (r + (int)a) < 255 ? (r + (unsigned int)a) : 255;
            g = (g + (int)a) < 255 ? (g + (unsigned int)a) : 255;
            b = (b + (int)a) < 255 ? (b + (unsigned int)a) : 255;
        }
};


class ThreeDVector {
public:
	ThreeDVector() {}
	ThreeDVector(double x_, double y_, double z_) { x = x_; y = y_; z = z_; }
	double len() {
		return sqrt(x * x + y * y + z * z);
	}
	void normalize() {
		double l = len();
		x /= l;
		y /= l;
		z /= l;
	}

	ThreeDVector operator * (float a) {
		return ThreeDVector(a * x, a * y, a * z);
	}

	ThreeDVector operator + (ThreeDVector other) {
		return ThreeDVector(x + other.x, y + other.y, z + other.z);
	}


	ThreeDVector operator - (ThreeDVector other) {
		return ThreeDVector(x - other.x, y - other.y, z - other.z);
	}

	double x, y, z;
};

ThreeDVector vector_multiply(ThreeDVector& a, ThreeDVector& b) {
	return ThreeDVector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float scalar_multiply(ThreeDVector& a, ThreeDVector& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


struct Ray{
    ThreeDVector start;
    ThreeDVector dir;
    Ray() {}	
    Ray(ThreeDVector start_, ThreeDVector dir_) : start(start_), dir(dir_) {};		
};


class GeomObj {
public:
    GeomObj(){};
    GeomObj(RGBColor color_) : color(color_){}
    virtual pair<bool, ThreeDVector> ray_intersect(Ray r) = 0;
    virtual ThreeDVector normal_in_point(ThreeDVector point) = 0;
    RGBColor color;
};

class Sphere : public GeomObj {
public:
    Sphere(ThreeDVector center_, double r_, RGBColor color_) : GeomObj(color_), center(center_), r(r_) {};

    ThreeDVector normal_in_point(ThreeDVector pnt){
            return pnt - center;
        }

	pair<bool, ThreeDVector> ray_intersect(Ray ray) {
		float t0, t1; // solutions for t if the ray intersects 
		ThreeDVector L = ray.start - center;
		float a = scalar_multiply(ray.dir, ray.dir);
		float b = 2 * scalar_multiply(ray.dir, L);
		float c = scalar_multiply(L, L) - r * r;

		if (!solveQuadratic(a, b, c, t0, t1)) 
			return{ false, center };

		if (t0 > t1) 
			std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) 
				return{ false, center }; // both t0 and t1 are negative 
		}

		return{ true, ray.start + ray.dir * t0 };
	
	}

	ThreeDVector center;
	double r;
};

class Triangle : public GeomObj {
public:
    Triangle(ThreeDVector a_, ThreeDVector b_, ThreeDVector c_) : GeomObj(RGBColor(0, 0, 0)), a(a_), b(b_), c(c_) {};
    Triangle(ThreeDVector a_, ThreeDVector b_, ThreeDVector c_, RGBColor color_) : GeomObj(color_), a(a_), b(b_), c(c_) {};

    pair<bool, ThreeDVector> ray_intersect(Ray ray) {
		ThreeDVector res(0, 0, 0);
		auto N = normal_vector();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = scalar_multiply(N, ray.dir);
		if (fabs(NdotRayDirection) < EPS) 
			return{ false, res }; // they are parallel so they don't intersect ! 

		// compute d parameter using equation 2
		float d = scalar_multiply(N, a);

		// compute t (equation 3)
		float t = (scalar_multiply(N, ray.start) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) 
			return { false, res }; // the triangle is behind 

								 // compute the intersection ThreeDVector using equation 1
		auto P = ray.start + ray.dir * t;

		// Step 2: inside-outside test

				 // edge 0
		auto edge0 = b - a;
		auto vp0 = P - a;
		auto C = vector_multiply(edge0, vp0);

		if (scalar_multiply(N, C) < 0) 
			return { false, res }; // P is on the right side 

											   // edge 1
		auto edge1 = c - b;
		auto vp1 = P - b;
		C = vector_multiply(edge1, vp1);
		if (scalar_multiply(N, C) < 0)  
			return { false, res }; // P is on the right side 

												// edge 2
		auto edge2 = a - c;
		auto vp2 = P - c;
		C = vector_multiply(edge2, vp2);
		if (scalar_multiply(N, C) < 0) 
			return { false, res }; // P is on the right side; 

		return { true, P }; // this ray hits the triangle 
	}

	ThreeDVector normal_vector() {
		auto side1 = b - a;
		auto side2 = c - a;
		auto nv = vector_multiply(side1, side2);
		nv.normalize();
		return nv;
	}

    ThreeDVector normal_in_point(ThreeDVector point){
        return this->normal_vector();
    }

	ThreeDVector a, b, c;

};

class Quadrilateral : public GeomObj {
	ThreeDVector vertexes[4];
	
	void to_format(){

	}

    ThreeDVector normal_vector(){
        ThreeDVector a = vertexes[0]; ThreeDVector b = vertexes[1]; ThreeDVector c = vertexes[2];
        auto side1 = b - a;
        auto side2 = c - a;
        auto nv = vector_multiply(side1, side2);
        nv.normalize();
        return nv;
    }

    ThreeDVector normal_in_point(ThreeDVector Point) {
		ThreeDVector a = vertexes[0]; ThreeDVector b = vertexes[1]; ThreeDVector c = vertexes[2];
		auto side1 = b - a;
		auto side2 = c - a;
		auto nv = vector_multiply(side1, side2);
		nv.normalize();
		return nv;
	}

	pair<bool, ThreeDVector> ray_intersect(Ray ray) {
		ThreeDVector res(0, 0, 0);
		auto N = normal_vector();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = scalar_multiply(N, ray.dir);
		if (fabs(NdotRayDirection) < EPS) 
			return{ false, res }; // they are parallel so they don't intersect !
		
		Triangle abd(vertexes[0], vertexes[1], vertexes[3]); 
		Triangle bcd(vertexes[1], vertexes[2], vertexes[3]);
		pair<bool, ThreeDVector> t1 = abd.ray_intersect(ray);  
		pair<bool, ThreeDVector> t2 = bcd.ray_intersect(ray);
		if (t1.first) return t1;
		if (t2.first) return t2;
		else return {false, res};  
		 
	}

	
};

