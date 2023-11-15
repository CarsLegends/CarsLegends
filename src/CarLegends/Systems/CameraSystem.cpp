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

			const mat4 projection = perspective(
				radians(CAMERA_FOV),
				static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT),
				CAMERA_NEAR_CLIP_PLANE,
				CAMERA_FAR_CLIP_PLANE
			);

			const mat4 view = lookAt(
				camera.mPosition, 
				camera.mPosition + camera.mOrientation, 
				camera.mUp
			);

			camera.mMatrix = projection * view;
		}
	}
}
