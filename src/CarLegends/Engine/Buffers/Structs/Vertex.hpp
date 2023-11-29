#ifndef BUFFERS_STRUCTS_VERTEX_HPP
#define BUFFERS_STRUCTS_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Buffers
{
	using namespace glm;

	struct Vertex
	{
		vec3 mPosition;
		vec3 mNormal;
		vec3 mColor;
		vec2 mTextureUv;

		Vertex() = default;
		Vertex(vec3 position, vec3 color)
		{
			this->mPosition = position;
			this->mNormal = {};
			this->mColor = color;
			this->mTextureUv = {};
		}
	};
}
#endif
