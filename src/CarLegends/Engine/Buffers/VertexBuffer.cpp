#include "VertexBuffer.hpp"

namespace Buffers
{
	VertexBuffer::VertexBuffer() = default;

	VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
	{
		glGenBuffers(1, &this->mId);
		glBindBuffer(GL_ARRAY_BUFFER, this->mId);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	void VertexBuffer::UpdateVertexBuffer(const std::vector<Vertex>& vertices) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->mId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::CreateVertexBufferText()
	{
		glGenBuffers(1, &this->mId);
		glBindBuffer(GL_ARRAY_BUFFER, this->mId);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
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
