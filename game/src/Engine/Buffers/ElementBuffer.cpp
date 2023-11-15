#include "ElementBuffer.hpp"

namespace Buffers
{
	ElementBuffer::ElementBuffer(std::vector<uint32_t>& indices)
	{
		glGenBuffers(1, &this->mId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	}

	void ElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mId);
	}

	void ElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ElementBuffer::Delete() const
	{
		glDeleteBuffers(1, &this->mId);
	}
}
