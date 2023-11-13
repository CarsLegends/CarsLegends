#ifndef BUFFERS_VERTEX_BUFFER_HPP
#define BUFFERS_VERTEX_BUFFER_HPP

#include <vector>
#include <glad/glad.h>

#include "Structs/Vertex.hpp"

namespace Buffers {
	using namespace std;

	class VertexBuffer {
	public:
		GLuint mId;

		VertexBuffer(vector<Vertex>& vertices);

		void Bind() const;
		void Unbind() const;
		void Delete() const;
	};	
}
#endif