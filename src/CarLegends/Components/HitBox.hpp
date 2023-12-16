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
		bool mFootball;

		HitBox();
		HitBox(bool isStatic, bool mFootball = false);
	};

	inline HitBox::HitBox()
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
		this->mStatic = false;
		this->mFootball = false;
	}

	inline HitBox::HitBox(bool isStatic, bool football)
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
		this->mStatic = isStatic;
		this->mFootball = football;
	}
}

#endif 