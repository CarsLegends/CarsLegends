#ifndef COMPONENTS_TRANSFORM_HPP
#define COMPONENTS_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Components
{
	using namespace glm;

	struct Transform
	{
		vec3 mScale;
		vec3 mPosition;
		vec3 mRotationAxis;
		float mRotationAngle;
		mat4 mCurrentTransformation;

		Transform();
		Transform(vec3 position, vec3 rotationAxis, vec3 scale, float rotationAngle);
	};

	inline Transform::Transform()
	{
		this->mScale = vec3(0.2f, 0.2f, 0.2f);
		this->mPosition = vec3(0.0f, 0.0f, 0.0f);
		this->mRotationAxis = vec3(1.0f, 0.0f, 0.0f);
		this->mRotationAngle = 0.0f;
		this->mCurrentTransformation = mat4(1.0f);
	}

	inline Transform::Transform(vec3 position, vec3 rotationAxis, vec3 scale, float rotationAngle)
	{
		this->mScale = scale;
		this->mPosition = position;
		this->mRotationAxis = rotationAxis;
		this->mRotationAngle = rotationAngle;
		this->mCurrentTransformation = mat4(1.0f);
	}
}
#endif