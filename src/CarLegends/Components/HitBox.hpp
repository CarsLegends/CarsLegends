#ifndef COMPONENTS_COLLISION_MESH_HPP
#define COMPONENTS_COLLISION_MESH_HPP

#include <glm/glm.hpp>

namespace Components
{
	using namespace glm;

	struct HitBox
	{
		vec3 mScale;
		vec3 mCenter;
		mat4 mTransformations;

		HitBox();
	};

	inline HitBox::HitBox()
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
	}
}

#endif 