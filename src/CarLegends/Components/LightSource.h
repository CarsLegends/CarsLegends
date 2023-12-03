#ifndef COMPONENTS_LIGHT_SOURCE_HPP
#define COMPONENTS_LIGHT_SOURCE_HPP

#include <glm/glm.hpp>

namespace Components
{
	using namespace glm;

	struct LightSource
	{
		vec4 mLightColor;
		vec3 mLightPosition;

		LightSource();
		LightSource(vec4 lightColor, vec3 lightPosition);
	};

	inline LightSource::LightSource()
	{
		this->mLightColor = vec4(1.0f);
		this->mLightPosition = vec3(0.0f);
	}

	inline LightSource::LightSource(vec4 lightColor, vec3 lightPosition)
	{
		this->mLightColor = lightColor;
		this->mLightPosition = lightPosition;
	}
}

#endif 