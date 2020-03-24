#pragma once
#include <cmath>
#include "Vector.h"

class Mat4 {
public:
	//Matrix data
	float m[4][4] = { 0 };

	// Constructors
	Mat4() {
			m[0][0] = 1.0f;		m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
			m[1][0] = 0;		m[1][1] = 1.0f;		m[1][2] = 0;		m[1][3] = 0;
			m[2][0] = 0;		m[2][1] = 0;		m[2][2] = 1.0f;		m[2][3] = 0;
			m[3][0] = 0;		m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1.0f;
	}

	Mat4(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33) {
		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
		m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
	}

	//Copy Constructor
	Mat4(const Mat4& n) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = n.m[i][j];
			}
		}
	}

	//Matrix multiplication
	Mat4 operator*(const Mat4& n) {
		Mat4 t;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				t.m[i][j] = m[i][0] * n.m[0][j] +
					m[i][1] * n.m[1][j] +
					m[i][2] * n.m[2][j] +
					m[i][3] * n.m[3][j];
			}
		}
		return t;
	}

	Vec4f operator*(const Vec4f& v) {
		Vec4f u;
		u.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
		u.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
		u.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
		u.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;

		return u;
	}

	//Matrix transpose
	Mat4 transpose() {
		return Mat4 (m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	static Mat4 translation(Vec3f d) {
		return Mat4(1, 0, 0, d.x,
			0, 1, 0, d.y,
			0, 0, 1, d.z,
			0, 0, 0, 1);
	}

	static Mat4 scale(Vec3f& s) {
		return Mat4(s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1);
	}

	static Mat4 scale(float s) {
		return Mat4(s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, s, 0,
			0, 0, 0, 1);
	}

	static Mat4 rotationX(float angle) {
		float sinAngle = std::sinf(angle);
		float cosAngle = std::cosf(angle);

		return Mat4(1, 0, 0, 0,
			0, cosAngle, -sinAngle, 0,
			0, sinAngle, cosAngle, 0,
			0, 0, 0, 1);
	}

	static Mat4 rotationY(float angle) {
		float sinAngle = std::sinf(angle);
		float cosAngle = std::cosf(angle);

		return Mat4(cosAngle, 0, sinAngle, 0,
			0, 1, 0, 0,
			-sinAngle, 0, cosAngle, 0,
			0, 0, 0, 1);
	}

	static Mat4 rotationZ(float angle) {
		float sinAngle = std::sinf(angle);
		float cosAngle = std::cosf(angle);

		return Mat4(cosAngle, -sinAngle, 0, 0,
			sinAngle, cosAngle, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	//Rotate around arbitrary axis
	static Mat4 rotation(Vec3f& v, float angle) {
		float sinAngle = std::sinf(angle);
		float cosAngle = std::cosf(angle);
		Vec3f axis = v.normalize();
		return Mat4(axis.x * axis.x * (1 - cosAngle) + cosAngle, axis.x * axis.y * (1 - cosAngle) - axis.z * sinAngle, axis.x * axis.z * (1 - cosAngle) + axis.y * sinAngle, 0,
			axis.x * axis.y * (1 - cosAngle) + axis.z * sinAngle, axis.y * axis.y * (1 - cosAngle) + cosAngle, axis.y * axis.z * (1 - cosAngle) - axis.x * sinAngle, 0,
			axis.x * axis.z * (1 - cosAngle) - axis.y * sinAngle, axis.y * axis.z * (1 - cosAngle) + axis.x * sinAngle, axis.z * axis.z * (1 - cosAngle) + cosAngle, 0,
			0, 0, 0, 1);
	}
};

//class Mat3 {
//public:
//	//Matrix data
//	float m[3][3] = { 0 };
//
//	// Constructors
//	Mat3() {
//		m[0][0] = 1.0f;		m[0][1] = 0;		m[0][2] = 0;
//		m[1][0] = 0;		m[1][1] = 1.0f;		m[1][2] = 0;
//		m[2][0] = 0;		m[2][1] = 0;		m[2][2] = 1.0f;
//	}
//
//	Mat3(float a00, float a01, float a02, 
//		float a10, float a11, float a12, 
//		float a20, float a21, float a22) {
//		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; 
//		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; 
//		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; 
//	}
//
//	//Copy Constructor
//	Mat3(const Mat3& n) {
//		for (int i = 0; i < 3; ++i) {
//			for (int j = 0; j < 3; ++j) {
//				m[i][j] = n.m[i][j];
//			}
//		}
//	}
//
//	//Matrix multiplication
//	Mat3 operator*(const Mat3& n) {
//		Mat3 t;
//		for (int i = 0; i < 3; ++i) {
//			for (int j = 0; j < 3; ++j) {
//				t.m[i][j] = m[i][0] * n.m[0][j] +
//					m[i][1] * n.m[1][j] +
//					m[i][2] * n.m[2][j];
//			}
//		}
//		return t;
//	}
//
//	Mat3 operator*(const float n) {
//		Mat3 t;
//		for (int i = 0; i < 3; ++i) {
//			for (int j = 0; j < 3; ++j) {
//				t.m[i][j] = m[i][j] * n;
//			}
//		}
//		return t;
//	}
//
//	Vec3f operator*(const Vec3f& v) {
//		Vec3f u;
//		u.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
//		u.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
//		u.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
//
//		return u;
//	}
//
//	//Matrix transpose
//	Mat3 transpose() {
//		return Mat3(m[0][0], m[1][0], m[2][0],
//			m[0][1], m[1][1], m[2][1],
//			m[0][2], m[1][2], m[2][2]);
//	}
//
//	int det() {
//		return	m[0][0] * m[1][1] * m[2][2] +
//			m[0][1] * m[1][2] * m[2][0] +
//			m[0][2] * m[1][0] * m[2][1] -
//			(m[0][1] * m[1][0] * m[2][2] +
//				m[0][0] * m[1][2] * m[2][1] +
//				m[0][2] * m[1][1] * m[2][0]);
//	}
//
//	Mat3 inverse() {
//		//a m[0][0]
//		//b m[0][1]
//		//c m[0][2]
//		//d m[1][0]
//		//e m[1][1]
//		//f m[1][2]
//		//g m[2][0]
//		//h m[2][1]
//		//i m[2][2]
//		float var = 1 / det();
//		Mat3 mat;
//		mat.m[0][0] = (m[1][1] * m[2][2]) - (m[1][2] * m[2][1]); //ei-fh
//		mat.m[0][1] = (m[0][2] * m[2][1]) - (m[0][1] * m[2][2]); //ch-bi
//		mat.m[0][2] = (m[0][1] * m[1][2]) - (m[0][2] * m[1][1]); //bf-ce
//		mat.m[1][0] = (m[1][2] * m[2][0]) - (m[1][0] * m[2][2]); //fg-di
//		mat.m[1][1] = (m[0][0] * m[2][2]) - (m[0][2] * m[2][0]); //ai-cg
//		mat.m[1][2] = (m[0][2] * m[1][0]) - (m[0][0] * m[1][2]); //cd-af
//		mat.m[2][0] = (m[1][0] * m[2][1]) - (m[1][1] * m[2][0]); //dh-eg
//		mat.m[2][1] = (m[0][1] * m[2][0]) - (m[0][0] * m[2][1]); //bg-ah
//		mat.m[2][2] = (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]); //ae-bd
//		return mat * var;
//	}
//};
