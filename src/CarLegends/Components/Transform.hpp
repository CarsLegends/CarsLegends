#ifndef COMPONENTS_TRANSFORM_HPP
#define COMPONENTS_TRANSFORM_HPP

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace Components
{
	using namespace glm;

	struct Transform
	{
		vec3 mScale;
		vec3 mPosition;
		vec3 mEulerAngles;
		mat4 mTransformations;

		Transform();
		Transform(vec3 position, vec3 eulerAngles, vec3 vectorScale);
	};

	inline Transform::Transform()
	{
		this->mScale = vec3(0.2f, 0.2f, 0.2f);
		this->mPosition = vec3(0.0f, 0.0f, 0.0f);
		this->mEulerAngles = vec3(1.0f, 0.0f, 0.0f);
		this->mTransformations = mat4(1.0f);
	}

	inline Transform::Transform(vec3 position, vec3 eulerAngles, vec3 vectorScale)
	{
		this->mScale = vectorScale;
		this->mPosition = position;
		this->mEulerAngles = eulerAngles;

		this->mTransformations = mat4(1.0f);
		this->mTransformations = translate(this->mTransformations, this->mPosition);
		this->mTransformations *= toMat4(quat(this->mEulerAngles));
		this->mTransformations = scale(this->mTransformations, this->mScale);
	}
}
#endif