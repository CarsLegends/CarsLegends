#ifndef COMPONENTS_CAMERA_HPP
#define COMPONENTS_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Components
{
	using namespace glm;

	struct Camera
	{
		mat4 mMatrix;
		vec3 mOrientation;
		vec3 mPosition;
		vec3 mUp;

		Camera();
		Camera(vec3 position);
	};

	inline Camera::Camera()
	{
		this->mMatrix = mat4(1.0f);
		this->mOrientation = vec3(0.0f, 0.0f, -1.0f);
		this->mPosition = vec3(0.0f, 0.0f, -2.0f);
		this->mUp = vec3(0.0f, 0.5f, 0.0f);
	}

	inline Camera::Camera(vec3 position)
	{
		this->mMatrix = mat4x4(1.0f);
		this->mOrientation = vec3(0.0f, 0.0f, -1.0f);
		this->mPosition = position;
		this->mUp = vec3(0.0f, 1.0f, 0.0f);
	}
}
#endif
