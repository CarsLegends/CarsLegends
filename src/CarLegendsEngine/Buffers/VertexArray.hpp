#ifndef BUFFERS_VERTEX_ARRAY_H
#define BUFFERS_VERTEX_ARRAY_H

#include <glad/glad.h>
#include "VertexBuffer.hpp"

namespace Buffers {
	using namespace std;

	class VertexArray {
	public:
		GLuint mId;
		VertexArray();

		void LinkAttributes(VertexBuffer vertexBuffer);
		void DrawElements(vector<uint32_t> indices);

		void Bind() const;
		void Unbind() const;
		void Delete() const;
	private:
		void LinkAttribute(
			VertexBuffer vertexBuffer,
			GLuint layout,
			GLuint numComponents,
			GLenum type,
			GLsizeiptr stride,
			void* offset
		);
	};
}



#endif
