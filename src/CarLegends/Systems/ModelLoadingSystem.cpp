#include "ModelLoadingSystem.hpp"

namespace Systems
{

	ModelLoadingSystem::ModelLoadingSystem() = default;

	void ModelLoadingSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}

	void ModelLoadingSystem::Update()
	{
		for (const auto& iEntity : this->mEntities)
		{
			auto& renderable = this->mCoordinator->GetComponent<Renderable>(iEntity);

			if (renderable.mLoaded)
			{
				continue;
			}

			vector<ModelData> model = mModelLoader.LoadModel(renderable.mDirectory);

			for (auto iMesh : model)
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

				for (auto iTexture : iMesh.mTextures)
				{
					mesh.mTextures.push_back(iTexture);
				}

				renderable.mMeshes.push_back(mesh);
				SendVertex(mesh);
			}
		}
	}

	void ModelLoadingSystem::SendVertex(Renderable::Mesh& mesh)
	{
		const VertexBuffer vertexBuffer(mesh.mVertices);
		const ElementBuffer elementBuffer(mesh.mIndices);

		mesh.mVertexArray.Bind();
		mesh.mVertexArray.LinkAttributes(vertexBuffer);
		mesh.mVertexArray.Unbind();

		vertexBuffer.Unbind();
		elementBuffer.Unbind();
	}
}
