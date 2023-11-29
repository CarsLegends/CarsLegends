#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include <memory>
#include <glm/glm.hpp>

#include "ISystem.hpp"
#include "Structs/Simplex.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Engine/Buffers/Structs/Vertex.hpp"
#include "../Components/HitBox.hpp"

namespace Systems
{
	using namespace Coordinators;
	using namespace Components;
	using namespace Buffers;
	using namespace glm;

	using Collider = std::vector<vec3>;

	class CollisionSystem final : public ISystem
	{
	public:
		CollisionSystem();
		void Initialize(const std::shared_ptr<Coordinator>& coordinator);
		void Update(float deltaTime) override;
	private:
		std::shared_ptr<Coordinator> mCoordinator;

		Collider UpdateVerticesPosition(Entity entity) const;
		static std::vector<std::pair<Entity, Entity>> GetUniquePairsOfEntities(const std::set<Entity>& entities);

		// GJK
		static std::pair<bool, Simplex> CheckForCollision(const Collider& colliderA, const Collider& colliderB);

		static vec3 Support(const Collider& colliderA, const Collider& colliderB, vec3 direction);
		static bool NextSimplex(Simplex& points, vec3& direction);
		static bool SameDirection(const vec3& direction, const vec3& ao);
		static vec3 FindFurthestPoint(const Collider& colliderA, vec3 direction);

		static bool Line(Simplex& points, vec3& direction);
		static bool Triangle(Simplex& points, vec3& direction);
		static bool Tetrahedron(Simplex& points, vec3& direction);

		//EPA
		//CollisionPoints EPA(const Simplex& simplex, const Collider& colliderA, const Collider& colliderB);


		// Basic 1x1x1 box
		inline static std::vector<vec3> mSimplestBoxVertices =
		{
			vec3 { -0.5, -0.5, -0.5 },
			vec3 {  0.5, -0.5, -0.5 },
			vec3 {  0.5,  0.5, -0.5 },
			vec3 { -0.5,  0.5, -0.5 },
			vec3 { -0.5, -0.5,  0.5 },
			vec3 {  0.5, -0.5,  0.5 },
			vec3 {  0.5,  0.5,  0.5 },
			vec3 { -0.5,  0.5,  0.5 }
		};
	};
}

#endif