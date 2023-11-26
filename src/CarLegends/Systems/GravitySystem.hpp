#ifndef SYSTEMS_GRAVITY_HPP
#define SYSTEMS_GRAVITY_HPP

#include <memory>

#include "ISystem.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Gravity.hpp"

namespace Systems
{
	using namespace Components;
	using namespace Configuration;
	using namespace Coordinators;
	using namespace glm;

	class GravitySystem final : public ISystem
	{
	public:
		GravitySystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update(float deltaTime) override;

	private:
		std::shared_ptr<Coordinator> mCoordinator;
	};
}

#endif
