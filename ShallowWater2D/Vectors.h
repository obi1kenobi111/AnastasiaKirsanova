#pragma once
#include <vector>
#include <math.h>
using namespace std;

struct vec4r {
	float a;
	float b;
	float c;
	float d;
};

struct vec3r {
	float x;
	float y;
	float z;
};

struct vec2r {
	float x;
	float y;
};

struct mat3r {
	vec3r line1;
	vec3r line2;
	vec3r line3;

};


vec3r CrossProduct(vec3r a, vec3r b);
vec3r normalize(vec3r vec);
vec3r make_vec3r(float x, float y, float z);
void CalculateNormals(vector<vector<vec3r>>& normals, vector<vector<double>> Surface, vector<double> x, vector<double> y);


inline vec3r operator - (vec3r a, vec3r b) {

	vec3r c = make_vec3r(a.x - b.x, a.y - b.y, a.z - b.z);
	return c;
}

 vec2r operator - (vec2r a, vec2r b);


inline void CalculateNormals(vector<vector<vec3r>>& normals, vector<vector<double>> Surface, vector<double> x, vector<double> y) {

	normals.resize(Surface.size() - 1);
	for (int i = 0; i < normals.size(); i++) {
		normals[i].resize(Surface[0].size() - 1); 
	}

	for (int i = 0; i < Surface.size() - 1; i++) {
		for (int j = 0; j < Surface[0].size() - 1; j++) {

			vec3r p0 = make_vec3r(x[i], y[j], Surface[i][j]); 
			vec3r p1 = make_vec3r(x[i + 1], y[j], Surface[i + 1][j]);
			vec3r p2 = make_vec3r(x[i + 1], y[j + 1], Surface[i + 1][j + 1]);
			vec3r vec1 = p1 - p0;
			vec3r vec2 = p2 - p0;
			normals[i][j] = normalize(CrossProduct(vec1, vec2));
		}
	}
}

inline vec3r CrossProduct(vec3r a, vec3r b)
{
		vec3r c;
		c.x = a.y * b.z - a.z * b.y;
		c.y = a.z * b.x - a.x * b.z;
		c.z = a.x * b.y - a.y * b.x;
	
		return c;
}

inline vec3r normalize(vec3r vec) {

	double length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	vec3r vec_normalized = make_vec3r(vec.x / length, vec.y / length, abs(vec.z) / length); 
	
	return vec_normalized;
}
inline vec4r make_vec4r(float a, float b, float c, float d) {
	vec4r vec;
	vec.a = a;
	vec.b = b;
	vec.c = c;
	vec.d = d;
	return vec;
}

inline vec3r make_vec3r(float x, float y, float z)
{
		vec3r c;
		c.x = x;
		c.y = y;
		c.z = z;
	return c;
}

inline vec2r make_vec2r(float x, float y) {

	vec2r c;
	c.x = x;
	c.y = y;
	return c;
}

inline float mod(vec2r c) {

	float d;
	d = sqrt(c.x * c.x + c.y * c.y );

	return d;
}


inline float mod3(vec3r c) {

	float d;
	d = sqrt(c.x*c.x + c.y*c.y +c.z*c.z);

	return d;
}

inline float CalcAngle(vec3r a, vec3r b) {

	float alpha = acos(a.x*b.x + a.y*b.y + a.z*b.z)/mod3(a) * mod3(b);
	
	return alpha;
}

inline  mat3r CalculateQuaternionMatrix(vec4r Quaternion) {

	mat3r mat;
	float a, b, c, d;
	a = Quaternion.a;
	b = Quaternion.b;
	c = Quaternion.c;
	d = Quaternion.d;
	mat.line1 = make_vec3r(a*a + b*b - c*c-d*d, 2*b*c - 2*a*d, 2*b*d+2*a*c);
	mat.line2 = make_vec3r(2*b*c +2*a*d, a*a -b*b + c*c -d*d, 2*c*d - 2*a*b);
	mat.line3 = make_vec3r(2*b*d -2*a*c, 2*c*d + 2*a*b, a*a -b*b-c*c+d*d);

	return mat;
}

inline vec3r operator *(mat3r mat, vec3r vec) {

	vec3r new_vec = make_vec3r(mat.line1.x*vec.x + mat.line1.y * vec.y + mat.line1.z * vec.z,
		mat.line2.x * vec.x + mat.line2.y * vec.y + mat.line2.z * vec.z,
		mat.line3.x * vec.x + mat.line3.y * vec.y + mat.line3.z * vec.z);


	return new_vec;
}

