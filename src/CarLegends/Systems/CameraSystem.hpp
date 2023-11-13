#ifndef SYSTEMS_CAMERA_HPP
#define SYSTEMS_CAMERA_HPP

#include <memory>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "ISystem.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Components/Camera.hpp"
#include "../Configurations/WindowConfiguration.hpp"
#include "../Configurations/CameraConfiguration.hpp"

namespace Systems
{
	using namespace Components;
	using namespace Configuration;
	using namespace Coordinators;

	class CameraSystem : public ISystem
	{
	public:
		CameraSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update() override;

	private:
		std::shared_ptr<Coordinator> mCoordinator;

		inline static mat4 sProjection = perspective(
			radians(CAMERA_FOV),
			static_cast<float>(WINDOW_WIDTH / WINDOW_HEIGHT),
			CAMERA_NEAR_CLIP_PLANE,
			CAMERA_FAR_CLIP_PLANE
		);
	};
}

#endif
