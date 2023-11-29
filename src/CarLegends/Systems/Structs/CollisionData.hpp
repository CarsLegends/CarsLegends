#ifndef SYSTEMS_MANIFOLD_HPP
#define SYSTEMS_MANIFOLD_HPP

#include <glm/glm.hpp>

#include "Simplex.hpp"

namespace Systems
{
	using namespace glm;

	struct CollisionData
	{
		vec3 mNormal;
		float mDepth;

		CollisionData();
		CollisionData(vec3 normal, float depth);
	};

	inline CollisionData::CollisionData(vec3 normal, float depth)
	{
		this->mNormal = normal;
		this->mDepth = depth;
	}
}

#endif
