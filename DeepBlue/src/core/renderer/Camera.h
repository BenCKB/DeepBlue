#pragma once
#include "../../math/Vector.h"
#include "../../math/Matrix.h"
#include "../../math/Transform.h"

/*
*	Saves information about viewing frustum like
*	near- and far plane position as well as aspect ratio
*	and field of view.
*/
struct Frustum {
	
	Frustum() {
		m_near = 0.3f; 
		m_far = 10000.0f;
		m_fov = 1.047f; 
		m_ratio = 1.6923f;
	}

	Frustum(float f, float n, float fov, float r) {
		m_far = f;
		m_near = n;
		m_fov = fov;
		m_ratio = r;
	}

	float m_far;
	float m_near;
	float m_fov;
	float m_ratio;
};

/*
*	Represents the virtual camera in the scene.
*	Using the camera position, up- and lookAt vector,
*	the view matrix can be calculated. Using a frustum, 
*	we can also calculate the projection matrix.
*	Includes helper functions to move and rotate the camera
*	in camera-space.
*	
*	NOTE: The matrices returned, need to be transposed,
*	before being sent to the GPU! 
*/
class Camera {

public:
	Vec3f pos;
	Vec3f up;
	Vec3f lookAt;

	Frustum frustum;
	Transform t;

	Camera() {}

	Camera(Vec3f pos_, Vec3f up_, Vec3f lookAt_) : pos(pos_), up(up_), lookAt(lookAt_) {
		pos = pos_;
		up = up.normalize();
		lookAt = lookAt.normalize();
		frustum = Frustum();
	}

	void setFrustum(Frustum _frustum) {
		frustum = _frustum;
	}

	void translate(Vec3f direction, float dx) {
		direction = direction.normalize();
		pos = pos + (direction * dx);
	}

	void rotateX(float angle) {

		Vec3f dir = cross(lookAt, gY).normalize();
		
		Vec4f result = Mat4::rotation(dir, angle) * Vec4f(lookAt, 1);

		lookAt.x = result.x;
		lookAt.y = result.y;
		lookAt.z = result.z;

		lookAt = lookAt.normalize();

		up = cross(dir, lookAt).normalize();
	}

	void rotateY(float angle) {

		Vec3f dir = cross(lookAt, gY).normalize();

		Vec4f result = Mat4::rotation(gY, angle) * Vec4f(lookAt.x, lookAt.y, lookAt.z, 1);
		
		lookAt.x = result.x;
		lookAt.y = result.y;
		lookAt.z = result.z;
		lookAt = lookAt.normalize();

		up = cross(dir, lookAt).normalize();
	}


	Mat4 getViewMatrix() {
		Vec3f right = cross(lookAt,up);
		right = right.normalize();

		//Vec3f u = cross(lookAt);

		Mat4 m(right.x,		right.y,	right.z,	0.0,
				up.x, up.y, up.z, 0.0,
			   -lookAt.x,	-lookAt.y,	-lookAt.z,	0.0,
			   0.0,			0.0,		0.0,		1);
		return m * Mat4::translation(pos * -1);
	}

	Mat4 getOrthoProjectionMatrix() {
		float r = frustum.m_ratio * 1.1f;
		float l = -frustum.m_ratio * 1.1f;
		float t = 1.1f;
		float b = -1.1f;
		float n = frustum.m_near; //0.5
		float f = frustum.m_far;//50.0
		Mat4 projectionMatrix(2 / (r - l), 0, 0, -((r + l) / (r - l)),
			0, 2 / (t - b), 0, -((t + b) / (t - b)),
			0, 0, -2 / (f - n), -((f + n) / (f - n)),
			0, 0, 0, 1
		);
		return projectionMatrix;
	}

	Mat4 getProjectionMatrix() {
		float range = frustum.m_far - frustum.m_near;
		Mat4 projectionMatrix(	1 / (tanf(frustum.m_fov / 2) * frustum.m_ratio),	0,										0,												0,
								0,													1 / (tanf(frustum.m_fov / 2)),			0,												0,
								0,													0,										-(frustum.m_far + frustum.m_near) / (range),	-((2 * frustum.m_far * frustum.m_near) / (range)),
								0,													0,										-1,												0
		);

		return projectionMatrix;
		//return getOrthoProjectionMatrix();
	}

	Mat4 getPV() {
		return (getProjectionMatrix() * getViewMatrix());
	}
	private:
		Vec3f gY = Vec3f(0.0f, 1.0f, 0.0f);
};