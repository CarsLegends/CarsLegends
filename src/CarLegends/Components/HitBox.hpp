#ifndef COMPONENTS_HIT_BOX_HPP
#define COMPONENTS_HIT_BOX_HPP

#include <glm/glm.hpp>

namespace Components
{
	using namespace glm;

	struct HitBox
	{
		vec3 mScale;
		vec3 mCenter;
		mat4 mTransformations;
		bool mStatic;

		HitBox();
		HitBox(bool isStatic);
	};

	inline HitBox::HitBox()
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
		mStatic = false;
	}

	inline HitBox::HitBox(bool isStatic)
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
		mStatic = isStatic;
	}
}

#endif 