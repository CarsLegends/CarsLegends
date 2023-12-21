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

			if (playable.mPlayerNumber == 0 && controllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButton::Start)))
			{
				mCoordinator->SendEvent(WINDOW_RESET_GOAL);
			}

			if (std::abs(controllerState.mLeftJoystickY) > 0.09f && std::abs(controllerState.mLeftJoystickX) > 0.09f)
			{
				transform.mEulerAngles.y = atan(controllerState.mLeftJoystickX, controllerState.mLeftJoystickY);
			}

			if (std::abs(controllerState.mRightTrigger) > 0.09f)
			{
				rigidBody.mVelocity.z += cos(transform.mEulerAngles.y) * controllerState.mRightTrigger * deltaTime;
				rigidBody.mVelocity.x += sin(transform.mEulerAngles.y) * controllerState.mRightTrigger * deltaTime;
			}

			if (std::abs(controllerState.mLeftTrigger) > -0.09f)
			{
				rigidBody.mVelocity.z -= cos(transform.mEulerAngles.y) * controllerState.mLeftTrigger * deltaTime;
				rigidBody.mVelocity.x -= sin(transform.mEulerAngles.y) * controllerState.mLeftTrigger * deltaTime;
			}

			if(controllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButton::A)) && transform.mPosition.y <= 0.0f)
			{
				rigidBody.mVelocity.y += 20.0f * deltaTime;
			}

			if (controllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButton::B)) && transform.mPosition.y <= 0.0f)
			{
				if (rigidBody.mVelocity.z > 0)
				{
					rigidBody.mVelocity.z -= rigidBody.mVelocity.z / 100;
				}

				if (rigidBody.mVelocity.x > 0)
				{
					rigidBody.mVelocity.x -= rigidBody.mVelocity.x / 100;
				}

				if (rigidBody.mVelocity.z < 0)
				{
					rigidBody.mVelocity.z += -rigidBody.mVelocity.z / 100;
				}

				if (rigidBody.mVelocity.x < 0)
				{
					rigidBody.mVelocity.x += -rigidBody.mVelocity.x / 100;
				}
			}
		}
	}

	void PlayerMovementSystem::ControllerInputListener(Event& event)
	{
		const auto [controllerState, playerNumber] = event.GetParam<std::pair<ControllerState, int>>(WINDOW_CONTROLLER_INPUT_PARAMETER);
		this->mPlayersControllerState[playerNumber] = controllerState;
	}
}
