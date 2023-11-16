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
#include "../Engine/Window/Structs/MouseState.hpp"

namespace Systems
{
	using namespace Components;
	using namespace Configuration;
	using namespace Coordinators;
	using namespace Windows;
	using namespace glm;

	class CameraSystem : public ISystem
	{
	public:
		CameraSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update(float deltaTime) override;

	private:
		std::shared_ptr<Coordinator> mCoordinator;
		std::bitset<WINDOW_BUTTONS_COUNT> mButtons;
		MouseState mMouseState;
		bool mFirstClick = false;

		void MoveCameraByKeyboardInput(Camera& camera, float deltaTime);
		void MoveCameraByMouseInput(Camera& camera);

		void ButtonInputListener(Event& event);
		void MouseInputListener(Event& event);

		void CenterCursor() const;
	};
}

#endif
