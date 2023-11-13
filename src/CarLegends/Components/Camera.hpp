#ifndef COMPONENTS_CAMERA_HPP
#define COMPONENTS_CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Components
{
	using namespace glm;

	struct Camera
	{
		mat4 mProjectionMatrix;
		vec3 mOrientation;
		vec3 mPosition;
		vec3 mUp;

		Camera(vec3 position);
	};

	inline Camera::Camera(vec3 position)
	{
		this->mProjectionMatrix = mat4x4(1.0f);
		this->mOrientation = vec3(0.0f);
		this->mPosition = position;
		this->mUp = vec3(0.0f);
	}
}
#endif
