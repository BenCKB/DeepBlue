#pragma once
#include <math.h>

///////// 2D Vector ////////
template<typename T>
class Vec2 {
public:
	T x, y;

	Vec2() : x(0), y(0) {}
	Vec2(T _x, T _y) : x(_x), y(_y) {}

	inline T length() {
		return sqrt(x * x + y * y);
	}

	Vec2<T> operator+ (const Vec2<T>& v) {
		return Vec2(x + v.x, y + v.y);
	}

	Vec2<T> operator- (const Vec2<T>& v) {
		return Vec2(x - v.x, y - v.y);
	}

	template<typename U>
	Vec2<T> operator* (U s) {
		return Vec2(x * s, y * s);
	}

	template<typename U>
	Vec2<T> operator/ (U s) {
		if (s == 0)
			return NULL; 

		return Vec2(x / s, y / s);
	}

	inline void normalize() {
		this / this.length();
	}

};

template<typename T>
inline T dot(const Vec2<T>& v, const Vec2<T>& w) {
	return (v.x * w.x + v.y * w.y);
}

///////// 3D Vector ////////
template<typename T>
class Vec3 {
public:
	T x, y, z;

	Vec3() : x(0), y(0), z(0) {}
	Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	inline T length() {
		return sqrtf(x * x + y * y + z * z);
	}

	Vec3<T> operator+ (const Vec3<T>& v) {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3<T> operator- (const Vec3<T>& v) {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	template<typename U>
	Vec3<T> operator* (U s) {
		return Vec3(x * s, y * s, z * s);
	}

	template<typename U>
	Vec3<T> operator/ (U s) {

		return Vec3<T>(x / s, y / s, z / s);
	}

	inline Vec3 normalize() {
		return *this / this->length();
	}

};

template<typename T>
inline T dot(const Vec3<T>& v, const Vec3<T>& w) {
	return (v.x * w.x + v.y * w.y + v.z * w.z);
}

template<typename T>
inline Vec3<T> cross(const Vec3<T>& v, const Vec3<T>& w) {
	return Vec3<T>(v.y * w.z - v.z * w.y, 
				v.z * w.x - v.x * w.z,
				v.x * w.y - v.y * w.x );
}

///////// 4D Vector ////////
template<typename T>
class Vec4 {
public:
	T x, y, z, w;

	Vec4() : x(0), y(0), z(0), w(1) {}
	Vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}

	Vec4(Vec3<T> _v, T _w) : x(_v.x), y(_v.y), z(_v.z), w(_w) {}

	Vec4<T> operator+ (const Vec4<T>& v) {
		return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4<T> operator- (const Vec4<T>& v) {
		return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	template<typename U>
	Vec4<T> operator* (U s) {
		return Vec4(x * s, y * s, z * s, s * w);
	}

	template<typename U>
	Vec4<T> operator/ (U s) {
		return Vec4<T>(x / s, y / s, z / s, w / s);
	}
};


//////// TYPEDEFs ////////
typedef Vec2<float>		Vec2f;
typedef Vec2<int>		Vec2i;
typedef Vec2<double>	Vec2d;

typedef Vec3<float>		Vec3f;
typedef Vec3<int>		Vec3i;
typedef Vec3<double>	Vec3d;

typedef Vec4<float>		Vec4f;
typedef Vec4<int>		Vec4i;
typedef Vec4<double>	Vec4d;