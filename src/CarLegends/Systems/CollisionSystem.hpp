#ifndef SYSTEMS_COLLISION_HPP
#define SYSTEMS_COLLISION_HPP

#include <memory>
#include <glm/vec3.hpp>

#include "ISystem.hpp"
#include "Structs/Simplex.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Engine/Buffers/Structs/Vertex.hpp"
#include "../Components/Renderable.hpp"
#include "../Components/Transform.hpp"
#include "Structs/Manifold.hpp"

namespace Systems
{
	using namespace glm;
	using namespace Coordinators;
	using namespace Buffers;
	using namespace Components;

	using Collider = std::vector<vec3>;

	class CollisionSystem final : public ISystem
	{
	public:
		CollisionSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update(float deltaTime) override;

	private:
		std::shared_ptr<Coordinator> mCoordinator;

		static std::vector<std::pair<Entity, Entity>> FindUniquePairsOfEntities(const std::vector<Entity>& Entities);

		static Manifold GetMaxPen(const std::vector<Manifold>& manifolds);

		static Manifold EPA(const Simplex& simplex, const Collider& colliderA, const Collider& colliderB);
		static std::pair<std::vector<vec4>, size_t> GetFaceNormals(const std::vector<vec3>& polytope,const std::vector<size_t>& faces);
		static void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

		static std::pair<bool, Simplex> GJK(const Collider& colliderA, const Collider& colliderB);
		static vec3 Support(const Collider& colliderA, const Collider& colliderB, vec3 direction);
		static vec3 FindFurthestPoint(const Collider& collider, vec3 direction);
		static bool SameDirection(const vec3& direction, const vec3& ao);
		static bool NextSimplex(Simplex& points, vec3& direction);
		static bool Line(Simplex& points, vec3& direction);
		static bool Triangle(Simplex& points, vec3& direction);
		static bool Tetrahedron(Simplex& points, vec3& direction);
	};
}

#endif
