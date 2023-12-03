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
		vec3 mEulerAngles;
		mat4 mTransformations;

		Transform();
		Transform(vec3 position, vec3 eulerAngles, vec3 scale);
	};

	inline Transform::Transform()
	{
		this->mScale = vec3(0.2f, 0.2f, 0.2f);
		this->mPosition = vec3(0.0f, 0.0f, 0.0f);
		this->mEulerAngles = vec3(1.0f, 0.0f, 0.0f);
		this->mTransformations = mat4(1.0f);
	}

	inline Transform::Transform(vec3 position, vec3 eulerAngles, vec3 scale)
	{
		this->mScale = scale;
		this->mPosition = position;
		this->mEulerAngles = eulerAngles;
		this->mTransformations = mat4(1.0f);
	}
}
#endif