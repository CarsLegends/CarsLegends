#include "PlayerMovementSystem.hpp"

#include <glm/ext/matrix_transform.hpp>

#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"
#include "../Events/EventTypes.hpp"
#include "../Events/EventParameters.hpp"

namespace Systems
{
	void PlayerMovementSystem::Initialize(const std::shared_ptr<Coordinator>& coordinator)
	{
		this->mCoordinator = coordinator;

		this->mCoordinator->AddEventListener(WINDOW_CONTROLLER_INPUT, [this](auto&& PH1)
		{
			PlayerMovementSystem::ControllerInputListener(std::forward<decltype(PH1)>(PH1));
		});
	}

	void PlayerMovementSystem::Update(float deltaTime)
	{
		for (const auto iEntity : this->mEntities)
		{
			const auto& playable = this->mCoordinator->GetComponent<Playable>(iEntity);
			auto& transform = this->mCoordinator->GetComponent<Transform>(iEntity);
			auto& rigidBody = this->mCoordinator->GetComponent<RigidBody>(iEntity);

			auto controllerState = this->mPlayersControllerState[playable.mPlayerNumber];

			if(std::abs(controllerState.mLeftJoystickY) > 0.09f)
			{
				transform.mPosition.z += controllerState.mLeftJoystickY / 100.0f;
			}

			if (std::abs(controllerState.mLeftJoystickX) > 0.09f)
			{
				transform.mPosition.x += controllerState.mLeftJoystickX / 100.0f;
			}

			if (std::abs(controllerState.mRightTrigger) > 0.09f)
			{
				rigidBody.mAcceleration.z += controllerState.mRightTrigger * 0.002f;
			}

			if (std::abs(controllerState.mLeftTrigger) > 0.09f)
			{
				rigidBody.mAcceleration.z -= controllerState.mLeftTrigger * 0.002f;
			}

			if (controllerState.mRightTrigger == -1.0f && controllerState.mLeftTrigger == -1.0f)
			{
				rigidBody.mAcceleration.z = 0.0f;
			}

			if(controllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButtons::A)))
			{
				rigidBody.mVelocity.y += 10.0f * deltaTime;
			}
		}
	}

	void PlayerMovementSystem::ControllerInputListener(Event& event)
	{
		const auto [controllerState, playerNumber] = event.GetParam<std::pair<ControllerState, int>>(WINDOW_CONTROLLER_INPUT_PARAMETER);
		this->mPlayersControllerState[playerNumber] = controllerState;
	}
}
