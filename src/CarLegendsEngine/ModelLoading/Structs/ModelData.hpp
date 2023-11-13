#ifndef MODEL_LOADING_STRUCTS_MODEL_DATA_HPP
#define MODEL_LOADING_STRUCTS_MODEL_DATA_HPP

#include <vector>
#include "../../Buffers/Structs/Vertex.hpp"
#include "../../Textures/Texture.hpp"

namespace ModelLoaders {
	using namespace std;
	using namespace Buffers;
	using namespace Textures;

	struct ModelData {
		vector<Vertex> mVertices{};
		vector<uint32_t> mIndices{};
		vector<Texture> mTextures{};
	};
}

#endif