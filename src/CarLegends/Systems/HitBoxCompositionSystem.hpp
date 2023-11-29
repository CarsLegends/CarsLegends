#ifndef SYSTEMS_COLLISION_MESH_HPP
#define SYSTEMS_COLLISION_MESH_HPP

#include "ISystem.hpp"

#include "../Coordinators/Coordinator.hpp"
#include "../Components/HitBox.hpp"
#include "../Components/Renderable.hpp"
#include "../Components/Transform.hpp"

namespace Systems
{
	using namespace Systems;
	using namespace Coordinators;
	using namespace Components;

	class HitBoxCompositionSystem : public ISystem
	{
	public:
		HitBoxCompositionSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update(float deltaTime) override;
	private:
		std::shared_ptr<Coordinator> mCoordinator;
	};
}

#endif