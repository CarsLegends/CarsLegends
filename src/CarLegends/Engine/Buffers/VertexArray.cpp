#include "VertexArray.hpp"

namespace Buffers
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &this->mId);
	}

	void VertexArray::LinkAttributes(VertexBuffer vertexBuffer)
	{
		LinkAttribute(vertexBuffer, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
		LinkAttribute(vertexBuffer, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, mNormal)));
		LinkAttribute(vertexBuffer, 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, mColor)));
		LinkAttribute(vertexBuffer, 3, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, mTextureUv)));
	}

	void VertexArray::DrawElements(std::vector<uint32_t> indices)
	{
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, nullptr);
	}

	void VertexArray::DrawHitBoxesOutline()
	{
		glLineWidth(4);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr);
		glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(4 * sizeof(GLuint)));
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(8 * sizeof(GLuint)));
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(this->mId);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::Delete() const
	{
		glDeleteVertexArrays(1, &this->mId);
	}

	void VertexArray::LinkAttribute(
		VertexBuffer vertexBuffer,
		GLuint layout,
		GLuint numComponents,
		GLenum type,
		GLsizeiptr stride,
		void* offset)
	{
		vertexBuffer.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		vertexBuffer.Unbind();
	}
}
