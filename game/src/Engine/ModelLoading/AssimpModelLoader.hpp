#ifndef MODEL_LOADING_ASSIMP_MODEL_LOADER_HPP
#define MODEL_LOADING_ASSIMP_MODEL_LOADER_HPP

#include <vector>
#include <iostream>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "../Shaders/ShaderProgram.hpp"
#include "../Textures/Texture.hpp"
#include "../Buffers/Structs/Vertex.hpp"
#include "Structs/ModelData.hpp"

namespace ModelLoaders
{
	using namespace Shaders;
	using namespace Textures;
	using namespace Buffers;

	class AssimpModelLoader
	{
	public:
		vector<ModelData> LoadModel(string path);

	private:
		string mDirectory;
		vector<ModelData> mMeshes;
		vector<Texture> mTexturesLoaded;
		unordered_map<string, vector<ModelData>> mModelsLoaded;

		ModelData ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessNode(aiNode* node, const aiScene* scene);
		vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	};
}
#endif
