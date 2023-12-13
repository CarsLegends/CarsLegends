#include "ModelLoadingSystem.hpp"

namespace Systems
{

	ModelLoadingSystem::ModelLoadingSystem() = default;

	void ModelLoadingSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}
	void ModelLoadingSystem::Update(float deltaTime)
	{
		for (const auto& iEntity : this->mEntities)
		{
			auto& renderable = this->mCoordinator->GetComponent<Renderable>(iEntity);

			if (renderable.mLoaded)
			{
				continue;
			}

			if (this->mModelsLoaded.find(renderable.mDirectory) != this->mModelsLoaded.end())
			{
				renderable.mMeshes = this->mModelsLoaded[renderable.mDirectory];
				renderable.mLoaded = true;
				continue;
			}

			vector<ModelData> model = this->mModelLoader.LoadModel(renderable.mDirectory);

			for (auto& iMesh : model)
			{
				Renderable::Mesh mesh;

				for (auto iVertex : iMesh.mVertices)
				{
					mesh.mVertices.push_back(iVertex);
				}

				for (auto iIndex : iMesh.mIndices)
				{
					mesh.mIndices.push_back(iIndex);
				}

				for (auto& iTexture : iMesh.mTextures)
				{
					Texture texture = {
						iTexture.mPath.c_str(),
						iTexture.mDirectory,
						iTexture.mType.c_str(),
						iTexture.mUnit
					};

					mesh.mTextures.push_back(texture);
				}

				renderable.mMeshes.push_back(mesh);
				SendVertex(mesh);
			}

			this->mModelsLoaded[renderable.mDirectory] = renderable.mMeshes;
			renderable.mLoaded = true;
		}
	}

	void ModelLoadingSystem::SendVertex(Renderable::Mesh& mesh)
	{
		mesh.mVertexArray.Bind();
		const VertexBuffer vertexBuffer(mesh.mVertices);
		const ElementBuffer elementBuffer(mesh.mIndices);

		mesh.mVertexArray.LinkAttributes(vertexBuffer);

		mesh.mVertexArray.Unbind();
		vertexBuffer.Unbind();
		elementBuffer.Unbind();
	}
}
