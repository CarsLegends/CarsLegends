#include "CollisionModelSystem.hpp"

namespace Systems
{
	CollisionModelSystem::CollisionModelSystem() = default;

	void CollisionModelSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}

	void CollisionModelSystem::Update(float deltaTime)
	{
		for (const auto iEntity : this->mEntities)
		{
			auto& collisionModel = this->mCoordinator->GetComponent<CollisionModel>(iEntity);

			if (collisionModel.mCollider.empty())
			{
				const auto& renderable = this->mCoordinator->GetComponent<Renderable>(iEntity);

				for (const auto& iMesh : renderable.mMeshes)
				{
					//std::vector<vec3> uniqueMeshVertices = GetUniqueVertices(iMesh.mVertices);

					std::vector<vec3> colliderVertices = {};
					colliderVertices.reserve(iMesh.mVertices.size());

					for (auto& iVertex : iMesh.mVertices)
					{
						colliderVertices.push_back(iVertex.mPosition);
					}

					collisionModel.mCollider.push_back(colliderVertices);
				}
			}
		}
	}

	std::vector<vec3> CollisionModelSystem::GetUniqueVertices(std::vector<Vertex> vertices)
	{
		std::vector uniqueVertices = { vertices[0].mPosition };
		for (auto iVertex : vertices)
		{
			bool unique = true;
			for (const auto uniqueVertex : uniqueVertices)
			{
				if (iVertex.mPosition.x == uniqueVertex.x &&
					iVertex.mPosition.y == uniqueVertex.y &&
					iVertex.mPosition.z == uniqueVertex.z)
				{
					unique = false;
					break;
				}
			}

			if (unique)
			{
				uniqueVertices.push_back(iVertex.mPosition);
			}
		}

		return uniqueVertices;
	}
}
