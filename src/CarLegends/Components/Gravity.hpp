#ifndef COMPONENTS_GRAVITY_HPP
#define COMPONENTS_GRAVITY_HPP

#include <glm/vec3.hpp>

namespace Components
{
	using namespace glm;

	struct Gravity
	{
		vec3 mForce;

		Gravity();
		Gravity(vec3 force);
	};

	inline Gravity::Gravity() = default;

	inline Gravity::Gravity(vec3 force)
	{
		this->mForce = force;
	}
}

#endif