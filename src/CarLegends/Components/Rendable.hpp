#ifndef COMPONENTS_RENDERABLE_HPP
#define COMPONENTS_RENDERABLE_HPP

#include <vector>

#include "Buffers/VertexArray.hpp"
#include "Buffers/Structs/Vertex.hpp"
#include "Textures/Texture.hpp"

namespace Components {
	using namespace std;
	using namespace Buffers;
	using namespace Textures;

	struct Renderable {
		struct Mesh {
			vector<Vertex> mVertices{};
			vector<uint32_t> mIndices{};
			vector<Texture> mTextures{};
			VertexArray mVertexArray{};
		};

		vector<Mesh> mMeshes{};
		string mDirectory;
		bool mLoaded = false;

		explicit Renderable(string directory);
	};

	inline Renderable::Renderable(string directory) {
		this->mDirectory = directory;
	}
}

#endif