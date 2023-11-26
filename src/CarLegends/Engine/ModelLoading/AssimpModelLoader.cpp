#include "AssimpModelLoader.hpp"

namespace ModelLoaders
{
	using namespace glm;
	using namespace Assimp;

	vector<ModelData> AssimpModelLoader::LoadModel(string path)
	{
		this->mMeshes = vector<ModelData>();
		if(this->mModelsLoaded.find(path) != this->mModelsLoaded.end())
		{
			return this->mModelsLoaded[path];
		}

		Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			exit(EXIT_FAILURE);
		}
		this->mDirectory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);

		this->mModelsLoaded[path] = this->mMeshes;
		return this->mMeshes;
	}

	void AssimpModelLoader::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->mMeshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	ModelData AssimpModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		ModelData modelData;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.mPosition = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			if (mesh->HasNormals())
			{
				vertex.mNormal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}

			if (mesh->mTextureCoords[0])
			{
				vertex.mTextureUv = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertex.mTextureUv = vec2(0.0f, 0.0f);
			}

			modelData.mVertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				modelData.mIndices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		modelData.mTextures.insert(modelData.mTextures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		modelData.mTextures.insert(modelData.mTextures.end(), specularMaps.begin(), specularMaps.end());

		return modelData;
	}

	vector<Texture> AssimpModelLoader::LoadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString string;
			material->GetTexture(type, i, &string);
			bool skip = false;

			for (auto& loadedTexture : this->mTexturesLoaded)
			{
				if (strcmp(loadedTexture.mPath.data(), string.C_Str()) == 0)
				{
					textures.push_back(loadedTexture);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				Texture texture(string.C_Str(), this->mDirectory, typeName.c_str(), i);
				texture.mType = typeName.c_str();

				textures.push_back(texture);
				this->mTexturesLoaded.push_back(texture);
			}
		}

		return textures;
	}
}
