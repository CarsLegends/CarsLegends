#ifndef BUFFERS_STRUCTS_VERTEX_HPP
#define BUFFERS_STRUCTS_VERTEX_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Buffers {
	using namespace glm;

	struct Vertex {
		vec3 position;
		vec3 normal;
		vec3 color;
		vec2 texUv;
	};
}
#endif