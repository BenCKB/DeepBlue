#pragma once
#include "Vector.h"

class Quaternion {

public:
	Vec4f m; // xyz : rotation axis, w : angle

	Quaternion() {
		m.x = 0.0;
		m.y = 0.0;
		m.z = 0.0;
		m.w = 0.0;
	}

	Quaternion(float a, Vec3f v) {
		m.w = a;
		m.x = v.x;
		m.y = v.y;
		m.z = v.z;
	}
	
	Quaternion operator* (Quaternion s) { //Multiplication as suggested by Nick Bobic
		float A, B, C, D, E, F, G, H;
		A = (m.w + m.x) * (s.m.w + s.m.x);
		B = (m.z - m.y) * (s.m.y - s.m.z);
		C = (m.w - m.x) * (s.m.y + s.m.z);
		D = (m.y + m.z) * (s.m.w - s.m.x);
		E = (m.x + m.z) * (s.m.x + s.m.y);
		F = (m.x - m.z) * (s.m.x - s.m.y);
		G = (m.w + m.y) * (s.m.w - s.m.z);
		H = (m.w - m.y) * (s.m.w + s.m.z);
		float newA = B + (-E - F + G + H) / 2;
		return Quaternion(newA, Vec3f(A - (E + F + G + H) / 2, C + (E - F + G - H) / 2, D + (E - F - G + H) / 2));
	}

	inline Quaternion conj() {
		return Quaternion(m.w, Vec3f(-1 * m.x, -1 * m.y, -1 * m.z));
	}

	inline float norm() {
		return (m.w * m.w) + (m.x * m.x) + (m.y * m.y) + (m.z * m.z);
	}

	inline Quaternion inv() {
		Quaternion c = this->conj();
		float o = (1 / this->norm());
		return Quaternion(c.m.w * o, Vec3f(c.m.x, c.m.y, c.m.z) * o);
	}
};