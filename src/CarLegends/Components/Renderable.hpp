#ifndef COMPONENTS_RENDERABLE_HPP
#define COMPONENTS_RENDERABLE_HPP

#include <vector>

#include "../Engine/Buffers/VertexArray.hpp"
#include "../Engine/Buffers/Structs/Vertex.hpp"
#include "../Engine/Textures/Texture.hpp"

namespace Components
{
	using namespace Buffers;
	using namespace Textures;

	struct Renderable
	{
		struct Mesh
		{
			std::vector<Vertex> mVertices{};
			std::vector<uint32_t> mIndices{};
			std::vector<Texture> mTextures{};
			VertexArray mVertexArray{};
		};

		std::vector<Mesh> mMeshes{};
		std::string mDirectory;
		bool mShowHitBox;
		bool mLoaded;

		Renderable();
		Renderable(std::string directory);
	};

	inline Renderable::Renderable()
	{
		this->mShowHitBox = false;
		this->mLoaded = false;
	}

	inline Renderable::Renderable(std::string directory)
	{
		this->mDirectory = directory;
		this->mShowHitBox = false;
		this->mLoaded = false;
	}
}

#endif
