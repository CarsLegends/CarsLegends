#ifndef SYSTEMS_COLLISION_MESH_HPP
#define SYSTEMS_COLLISION_MESH_HPP

#include "ISystem.hpp"

#include "../Coordinators/Coordinator.hpp"
#include "../Components/CollisionModel.hpp"
#include "../Components/Renderable.hpp"
#include "../Components/Transform.hpp"

namespace Systems
{
	using namespace Systems;
	using namespace Coordinators;
	using namespace Components;

	class CollisionModelSystem : public ISystem
	{
	public:
		CollisionModelSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update(float deltaTime) override;
	private:
		std::shared_ptr<Coordinator> mCoordinator;

		static std::vector<vec3> GetUniqueVertices(std::vector<Vertex> vertices);
	};
}

#endif