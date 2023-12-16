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
		bool mGoal;

		HitBox();
		HitBox(bool isStatic, bool football = false, bool goal = false);
	};

	inline HitBox::HitBox()
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
		this->mStatic = false;
		this->mFootball = false;
		this->mGoal = false;
	}

	inline HitBox::HitBox(bool isStatic, bool football, bool goal)
	{
		this->mScale = vec3(0.0f);
		this->mCenter = vec3(0.0f);
		this->mTransformations = mat4(1.0f);
		this->mStatic = isStatic;
		this->mFootball = football;
		this->mGoal = goal;
	}
}

#endif 