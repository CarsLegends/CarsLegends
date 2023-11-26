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

			if (playable.mPlayerNumber != this->mControllerState.mPlayerNumber)
			{
				continue;
			}

			if(std::abs(this->mControllerState.mLeftJoystickY) > 0.09f)
			{
				transform.mPosition.z += this->mControllerState.mLeftJoystickY / 100.0f;
			}

			if (std::abs(this->mControllerState.mLeftJoystickX) > 0.09f)
			{
				transform.mPosition.x += this->mControllerState.mLeftJoystickX / 100.0f;
			}

			if (std::abs(this->mControllerState.mRightTrigger) > 0.09f)
			{
				rigidBody.mAcceleration.z += this->mControllerState.mRightTrigger * 0.002f;
			}

			if (std::abs(this->mControllerState.mLeftTrigger) > 0.09f)
			{
				rigidBody.mAcceleration.z -= this->mControllerState.mLeftTrigger * 0.002f;
			}

			if (this->mControllerState.mRightTrigger == -1.0f && this->mControllerState.mLeftTrigger == -1.0f)
			{
				rigidBody.mAcceleration.z = 0.0f;
			}

			if(this->mControllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButtons::A)))
			{
				rigidBody.mVelocity.y += 60.0f * deltaTime;
			}
		}
	}

	void PlayerMovementSystem::ControllerInputListener(Event& event)
	{
		const auto controllerState = event.GetParam<ControllerState>(WINDOW_CONTROLLER_INPUT_PARAMETER);
		this->mControllerState = controllerState;
	}
}
