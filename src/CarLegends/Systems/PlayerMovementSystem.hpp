﻿#ifndef SYSTEMS_PLAYER_MOVEMENT_HPP
#define SYSTEMS_PLAYER_MOVEMENT_HPP

#include <memory>
#include <glm/glm.hpp>

#include "ISystem.hpp"
#include "../Engine/Window/Structs/ControllerState.hpp"
#include "../Configurations/WindowConfiguration.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Components/Playable.hpp"
#include "../Configurations/ControllerConfiguration.hpp"

namespace Systems
{
	using namespace Windows;
	using namespace Coordinators;
	using namespace Configuration;
	using namespace Components;
	using namespace glm;

	class PlayerMovementSystem : public ISystem
	{
	public:
		void Initialize(const std::shared_ptr<Coordinator>& coordinator);
		void Update(float deltaTime) override;
	private:
		std::shared_ptr<Coordinator> mCoordinator;
		std::array<ControllerState, WINDOW_PLAYER_COUNT> mPlayersControllerState;

		void ControllerInputListener(Event& event);
	};
}

#endif