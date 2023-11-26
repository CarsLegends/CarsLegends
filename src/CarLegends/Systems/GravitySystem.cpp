#include "GravitySystem.hpp"

#include <glm/ext/quaternion_exponential.hpp>

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

			transform.mPosition += rigidBody.mVelocity * deltaTime + vec3(0.5f, 0.5f, 0.5f) * rigidBody.mAcceleration * vec3(pow(2, deltaTime), pow(2, deltaTime), pow(2, deltaTime));

			if (transform.mPosition.y < 0.0f)
			{
				transform.mPosition.y = 0.0f;
				rigidBody.mVelocity.y = 0.0f;
				continue;
			}

			rigidBody.mVelocity += gravity.mForce * deltaTime;
		}
	}
}
