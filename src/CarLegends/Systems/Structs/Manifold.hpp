#ifndef SYSTEMS_COLLISION_POINTS_HPP
#define SYSTEMS_COLLISION_POINTS_HPP
#include <glm/vec3.hpp>

namespace Systems
{
	using namespace glm;
	
	struct Manifold {
		vec3 mPointA;
		vec3 mPointB;
		vec3 mNormal;
		float mDepth;
		bool mCollision;
	};
}

#endif
