#include "CameraSystem.hpp"

#include <utility>

#include "../Events/EventParameters.hpp"
#include "../Events/EventTypes.hpp"

namespace Systems
{
	CameraSystem::CameraSystem() = default;

	void CameraSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
		this->mCoordinator->AddEventListener(WINDOW_INPUT, [this](auto&& PH1)
		{
			CameraSystem::InputListener(std::forward<decltype(PH1)>(PH1));
		});
	}

	void CameraSystem::Update(float deltaTime)
	{
		auto speed = (deltaTime * CAMERA_SPEED);

		for (const auto& iEntity : this->mEntities)
		{
			auto& camera = this->mCoordinator->GetComponent<Camera>(iEntity);

			if (mButtons.test(static_cast<std::size_t>(Buttons::A)))
			{
				camera.mPosition += speed * normalize(cross(camera.mOrientation, camera.mUp));
			}

			if (mButtons.test(static_cast<std::size_t>(Buttons::D)))
			{
				camera.mPosition += speed * -normalize(cross(camera.mOrientation, camera.mUp));
			}

			if (mButtons.test(static_cast<std::size_t>(Buttons::S)))
			{
				camera.mPosition += speed * camera.mOrientation;
			}

			if (mButtons.test(static_cast<std::size_t>(Buttons::W)))
			{
				camera.mPosition += speed * -camera.mOrientation;
			}

			if (mButtons.test(static_cast<std::size_t>(Buttons::Space)))
			{
				camera.mPosition += speed * -camera.mUp;
			}

			if (mButtons.test(static_cast<std::size_t>(Buttons::LeftShift)))
			{
				camera.mPosition += speed * camera.mUp;
			}

			this->mButtons.reset();

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

	void CameraSystem::InputListener(Event& event)
	{
		this->mButtons = event.GetParam<std::bitset<WINDOW_BUTTONS_COUNT>>(WINDOW_INPUT_PARAMETER);
	}
}
