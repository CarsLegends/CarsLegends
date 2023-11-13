#ifndef BUFFERS_ELEMENT_BUFFER_HPP
#define BUFFERS_ELEMENT_BUFFER_HPP

#include <glad/glad.h>
#include <vector>

namespace Buffers {
	using namespace std;

	class ElementBuffer {
	public:
		GLuint mId;

		ElementBuffer(vector<uint32_t>& indices);

		void Bind() const;
		void Unbind() const;
		void Delete() const;
	};
}
#endif