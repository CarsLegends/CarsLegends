#include "CameraSystem.hpp"

namespace Systems {
	extern Coordinator gCoordinator;

	CameraSystem::CameraSystem() = default;

	void CameraSystem::Update() {
		for (auto const& iEntity : this->mEntities) {
			auto& camera = gCoordinator.GetComponent<Camera>(iEntity);

			const mat4 view = lookAt(camera.mPosition, camera.mPosition + camera.mOrientation, camera.mUp);

			camera.mProjectionMatrix = sProjection * view;
		}
	}
}
 