#ifndef COMPONENTS_RIGID_BODY_HPP
#define COMPONENTS_RIGID_BODY_HPP

#include <glm/glm.hpp>

namespace Components
{
	using namespace glm;

	struct RigidBody
	{
		float mMass;
		float mRestitution;
		vec3 mVelocity;
		vec3 mAcceleration;

		RigidBody(vec3 velocity, vec3 acceleration);
		RigidBody();
	};

	inline RigidBody::RigidBody(vec3 velocity, vec3 acceleration)
	{
		this->mMass = 1;
		this->mRestitution = 0;
		this->mVelocity = velocity;
		this->mAcceleration = acceleration;
	}

	inline RigidBody::RigidBody()
	{
		this->mMass = 1.0f;
		this->mRestitution = 0.0f;
		this->mVelocity = vec3(0.0f, 0.0f, 0.0f);
		this->mAcceleration = vec3(0.0f, 0.0f, 0.0f);
	}
}
#endif