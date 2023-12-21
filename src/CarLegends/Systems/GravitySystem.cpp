#include "GravitySystem.hpp"

#include <glm/ext/quaternion_exponential.hpp>

#include "../Components/Renderable.hpp"

namespace Systems
{
	GravitySystem::GravitySystem() = default;

	void GravitySystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}

	void GravitySystem::Update(float deltaTime)
	{
		for (auto const& entity : mEntities)
		{
			auto& rigidBody = this->mCoordinator->GetComponent<RigidBody>(entity);
			auto& transform = this->mCoordinator->GetComponent<Transform>(entity);
			auto const& gravity = this->mCoordinator->GetComponent<Gravity>(entity);

			if (rigidBody.mAcceleration.y > gravity.mForce.y)
			{
				// Should be name gravity acceleration
				rigidBody.mAcceleration.y += gravity.mForce.y;
			}

			if (rigidBody.mVelocity.x >= 0)
			{
				rigidBody.mVelocity.x -= rigidBody.mVelocity.x / 100.0f;
			}
			else
			{
				rigidBody.mVelocity.x += -rigidBody.mVelocity.x / 100.0f;
			}

			if (rigidBody.mVelocity.z > 0)
			{
				rigidBody.mVelocity.z -= rigidBody.mVelocity.z / 100.0f;
			}
			else
			{
				rigidBody.mVelocity.z += -rigidBody.mVelocity.z / 100.0f;
			}

			rigidBody.mVelocity += rigidBody.mAcceleration * deltaTime;

			if (rigidBody.mVelocity.y < -10.0f)
			{
				rigidBody.mVelocity.y = -10.0f;
			}

			transform.mPosition += rigidBody.mVelocity * deltaTime;
			continue;
		}
	}
}
