#include "HitBoxCompositionSystem.hpp"

namespace Systems
{
	HitBoxCompositionSystem::HitBoxCompositionSystem() = default;

	void HitBoxCompositionSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}

	void HitBoxCompositionSystem::Update(float deltaTime)
	{
		for (const auto iEntity : this->mEntities)
		{
			auto& renderable = this->mCoordinator->GetComponent<Renderable>(iEntity);

			vec3 minVertex;
			vec3 maxVertex;

			minVertex.x = maxVertex.x = renderable.mMeshes[0].mVertices[0].mPosition.x;
			minVertex.y = maxVertex.y = renderable.mMeshes[0].mVertices[0].mPosition.y;
			minVertex.z = maxVertex.z = renderable.mMeshes[0].mVertices[0].mPosition.z;

			for (const auto& iMesh : renderable.mMeshes)
			{
				for (const auto& vertex : iMesh.mVertices)
				{
					if (vertex.mPosition.x < minVertex.x) minVertex.x = vertex.mPosition.x;
					if (vertex.mPosition.y < minVertex.y) minVertex.y = vertex.mPosition.y;
					if (vertex.mPosition.z < minVertex.z) minVertex.z = vertex.mPosition.z;

					if (vertex.mPosition.x > maxVertex.x) maxVertex.x = vertex.mPosition.x;
					if (vertex.mPosition.y > maxVertex.y) maxVertex.y = vertex.mPosition.y;
					if (vertex.mPosition.z > maxVertex.z) maxVertex.z = vertex.mPosition.z;
				}
			}
			auto& hitBox = this->mCoordinator->GetComponent<HitBox>(iEntity);

			hitBox.mScale = vec3{ maxVertex.x - minVertex.x, maxVertex.y - minVertex.y, maxVertex.z - minVertex.z };
			hitBox.mCenter = vec3{ (maxVertex.x + minVertex.x) / 2, (maxVertex.y + minVertex.y) / 2, (maxVertex.z + minVertex.z) / 2 };

			hitBox.mTransformations = mat4(1.0f);
			hitBox.mTransformations = translate(hitBox.mTransformations, hitBox.mCenter);
			hitBox.mTransformations = scale(hitBox.mTransformations, hitBox.mScale);

			const auto& transform = this->mCoordinator->GetComponent<Transform>(iEntity);
			hitBox.mTransformations = transform.mTransformations * hitBox.mTransformations;
		}
	}
}
