#include "CameraSystem.hpp"

#include <utility>

namespace Systems
{
	CameraSystem::CameraSystem() = default;

	void CameraSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}

	void CameraSystem::Update()
	{
		for (const auto& iEntity : this->mEntities)
		{
			auto& camera = this->mCoordinator->GetComponent<Camera>(iEntity);

			const mat4 view = lookAt(camera.mPosition, camera.mPosition + camera.mOrientation, camera.mUp);

			camera.mProjectionMatrix = sProjection * view;
		}
	}
}
