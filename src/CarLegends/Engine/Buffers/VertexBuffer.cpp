#include "VertexBuffer.hpp"

namespace Buffers
{
	VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &this->mId);
		glBindBuffer(GL_ARRAY_BUFFER, this->mId);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->mId);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Delete() const
	{
		glDeleteBuffers(1, &this->mId);
	}
}
