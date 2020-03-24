#pragma once
#include "Vector.h"
#include "Matrix.h"

class Transform {
public:

	//Translate
	Vec3f t = Vec3f(0.0f, 0.0f, 0.0f);
	//Rotate
	Vec3f r = Vec3f(0.0f, 0.0f, 0.0f);
	//Scale
	Vec3f s = Vec3f(1.0f, 1.0f, 1.0f);

	Mat4 getModelMatrix() {
		translation = std::make_shared<Mat4>(Mat4::translation(t));
		rotation = std::make_shared<Mat4>(Mat4::rotationY(r.y));
		scale = std::make_shared<Mat4>(Mat4::scale(s.x));

		return *translation * *rotation * *scale;
	}

	Transform() {
		translation = std::make_shared<Mat4>(Mat4::translation(t));
		rotation = std::make_shared<Mat4>(Mat4::rotationY(r.y));
		scale = std::make_shared<Mat4>(Mat4::scale(s.x));
	}

private:
	std::shared_ptr<Mat4> translation;
	std::shared_ptr<Mat4> rotation;
	std::shared_ptr<Mat4> scale;
};