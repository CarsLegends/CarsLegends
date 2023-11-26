#include "CameraSystem.hpp"

#include <utility>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../Events/EventParameters.hpp"
#include "../Events/EventTypes.hpp"

namespace Systems
{
	CameraSystem::CameraSystem() = default;

	void CameraSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);

		this->mCoordinator->AddEventListener(WINDOW_KEYBOARD_INPUT_BUTTON, [this](auto&& PH1)
		{
			CameraSystem::ButtonInputListener(std::forward<decltype(PH1)>(PH1));
		});

		this->mCoordinator->AddEventListener(WINDOW_INPUT_CURSOR, [this](auto&& PH1)
		{
			CameraSystem::MouseInputListener(std::forward<decltype(PH1)>(PH1));
		});
	}

	void CameraSystem::Update(float deltaTime)
	{

		for (const auto& iEntity : this->mEntities)
		{
			auto& camera = this->mCoordinator->GetComponent<Camera>(iEntity);

			MoveCameraByKeyboardInput(camera, deltaTime);
			MoveCameraByMouseInput(camera);

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

	void CameraSystem::MoveCameraByKeyboardInput(Camera& camera, float deltaTime)
	{
		const auto speed = (deltaTime * CAMERA_SPEED);

		if (mButtons.test(static_cast<std::size_t>(Buttons::A)))
		{
			camera.mPosition += speed * -normalize(cross(camera.mOrientation, camera.mUp));
		}

		if (mButtons.test(static_cast<std::size_t>(Buttons::D)))
		{
			camera.mPosition += speed * normalize(cross(camera.mOrientation, camera.mUp));
		}

		if (mButtons.test(static_cast<std::size_t>(Buttons::S)))
		{
			camera.mPosition += speed * -camera.mOrientation;
		}

		if (mButtons.test(static_cast<std::size_t>(Buttons::W)))
		{
			camera.mPosition += speed * camera.mOrientation;
		}

		if (mButtons.test(static_cast<std::size_t>(Buttons::Space)))
		{
			camera.mPosition += speed * camera.mUp;
		}

		if (mButtons.test(static_cast<std::size_t>(Buttons::LeftShift)))
		{
			camera.mPosition += speed * -camera.mUp;
		}

		this->mButtons.reset();
	}

	void CameraSystem::MoveCameraByMouseInput(Camera& camera)
	{
		if (!this->mMouseState.mRightClickReleased)
		{
			this->mCoordinator->SendEvent(WINDOW_HIDE_CURSOR);

			if (this->mFirstClick)
			{
				this->mMouseState.mPosition.y = WINDOW_HEIGHT / 2;
				this->mMouseState.mPosition.x = WINDOW_WIDTH / 2;
				this->mFirstClick = false;
			}

			const vec2 mousePosition(this->mMouseState.mPosition);
			const float xRotation = CAMERA_SENSITIVITY * (mousePosition.y - WINDOW_HEIGHT / 2) / WINDOW_HEIGHT;
			const float yRotation = CAMERA_SENSITIVITY * (mousePosition.x - WINDOW_WIDTH / 2) / WINDOW_WIDTH;

			const vec3 newOrientation = rotate(camera.mOrientation, radians(-xRotation), normalize(cross(camera.mOrientation, camera.mUp)));

			if (abs(angle(newOrientation, camera.mUp) - radians(90.0f)) <= radians(85.0f))
			{
				camera.mOrientation = newOrientation;
			}

			camera.mOrientation = rotate(camera.mOrientation, radians(-yRotation), camera.mUp);

			CenterCursor();
		}
	}

	void CameraSystem::ButtonInputListener(Event& event)
	{
		this->mButtons = event.GetParam<std::bitset<WINDOW_BUTTONS_COUNT>>(WINDOW_KEYBOARD_INPUT_BUTTON_PARAMETER);
	}

	void CameraSystem::MouseInputListener(Event& event)
	{
		this->mMouseState = event.GetParam<MouseState>(WINDOW_INPUT_CURSOR_PARAMETER);

		if (this->mMouseState.mRightClickReleased)
		{
			this->mCoordinator->SendEvent(WINDOW_SHOW_CURSOR);
			this->mFirstClick = true;
		}
	}

	void CameraSystem::CenterCursor() const
	{
		Event event(WINDOW_SET_CURSOR_POSITION);
		event.SetParam(WINDOW_SET_CURSOR_PARAMETER, vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
		this->mCoordinator->SendEvent(event);
	}
}
