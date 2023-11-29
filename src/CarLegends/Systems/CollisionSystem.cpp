#include "CollisionSystem.hpp"

#include <iostream>

namespace Systems
{
	CollisionSystem::CollisionSystem() = default;

	void CollisionSystem::Initialize(const std::shared_ptr<Coordinator>& coordinator)
	{
		this->mCoordinator = coordinator;
	}

	void CollisionSystem::Update(float deltaTime)
	{
		const std::vector<std::pair<Entity, Entity>> pairs = GetUniquePairsOfEntities(this->mEntities);

		for (const auto& iPair : pairs)
		{
			Collider colliderA = UpdateVerticesPosition(iPair.first);
			Collider colliderB = UpdateVerticesPosition(iPair.second);

			if(auto [collision, simplex] = CheckForCollision(colliderA, colliderB); collision)
			{
				std::cout << "COLLISION" << std::endl;
			}
			else
			{
				std::cout << "NO COLLISION" << std::endl;
			}
		}
	}

	Collider CollisionSystem::UpdateVerticesPosition(Entity entity) const
	{
		const auto& hitBox = this->mCoordinator->GetComponent<HitBox>(entity);

		Collider collider;

		for (vec3 vertex : mSimplestBoxVertices)
		{
			vec3 updatedVertex = { hitBox.mTransformations * vec4(vertex, 1) };
			collider.push_back(updatedVertex);
		}

		return collider;
	}

	std::vector<std::pair<Entity, Entity>> CollisionSystem::GetUniquePairsOfEntities(const std::set<Entity>& entities)
	{
		std::set<std::pair<Entity, Entity> > pairs;

		for (auto iEntity : entities)
		{
			for (auto jEntity : entities)
			{
				if (iEntity == jEntity)
				{
					continue;
				}

				auto complementaryPair = std::make_pair(jEntity, iEntity);
				if (pairs.find(complementaryPair) != pairs.end())
				{
					continue;
				}

				pairs.insert(std::make_pair(iEntity, jEntity));
			}
		}

		std::vector<std::pair<Entity, Entity>> result;
		result.reserve(pairs.size());

		for (const auto iPair : pairs)
		{
			result.push_back(iPair);
		}

		return result;
	}

	std::pair<bool, Simplex> CollisionSystem::CheckForCollision(const Collider& colliderA, const Collider& colliderB)
	{
		vec3 support = Support(colliderA, colliderB, vec3(1, 0, 0));

		Simplex simplex;
		simplex.push_front(support);

		vec3 direction = -support;

		while (true) {
			support = Support(colliderA, colliderB, direction);

			if (dot(support, direction) <= 0) 
			{
				return std::make_pair(false, simplex);
			}

			simplex.push_front(support);

			if (NextSimplex(simplex, direction)) 
			{
				return std::make_pair(true, simplex);
			}
		}
	}

	vec3 CollisionSystem::Support(const Collider& colliderA, const Collider& colliderB, vec3 direction)
	{
		return FindFurthestPoint(colliderA, direction) - FindFurthestPoint(colliderB, -direction);
	}

	bool CollisionSystem::NextSimplex(Simplex& points, vec3& direction)
	{
		switch (points.size())
		{
		case 2: return Line(points, direction);
		case 3: return Triangle(points, direction);
		case 4: return Tetrahedron(points, direction);
		default: return false;
		}
	}

	bool CollisionSystem::SameDirection(const vec3& direction, const vec3& ao)
	{
		return dot(direction, ao) > 0;
	}

	vec3 CollisionSystem::FindFurthestPoint(const Collider& colliderA, vec3 direction)
	{
		vec3  maxPoint{};
		float maxDistance = -std::numeric_limits<float>::infinity();

		for (vec3 vertex : colliderA) 
		{
			const float distance = dot(vertex, direction);

			if (distance > maxDistance) 
			{
				maxDistance = distance;
				maxPoint = vertex;
			}
		}

		return maxPoint;
	}

	bool CollisionSystem::Line(Simplex& points, vec3& direction)
	{
		vec3 pointA = points[0];
		const vec3 pointB = points[1];

		const vec3 vectorAB = pointB - pointA;
		const vec3 vectorAO = -pointA;

		if (SameDirection(vectorAB, vectorAO)) 
		{
			direction = cross(cross(vectorAB, vectorAO), vectorAB);
		}

		else {
			points = { pointA };
			direction = vectorAO;
		}

		return false;
	}

	bool CollisionSystem::Triangle(Simplex& points, vec3& direction)
	{
		vec3 pointA = points[0];
		vec3 pointB = points[1];
		vec3 pointC = points[2];

		const vec3 vectorAB = pointB - pointA;
		const vec3 vectorAC = pointC - pointA;
		const vec3 vectorAO = -pointA;

		const vec3 planeNormal = cross(vectorAB, vectorAC);

		if (SameDirection(cross(planeNormal, vectorAC), vectorAO)) 
		{
			if (SameDirection(vectorAC, vectorAO)) 
			{
				points = { pointA, pointC };
				direction = cross(cross(vectorAC, vectorAO), vectorAC);
			}

			else 
			{
				return Line(points = { pointA, pointB }, direction);
			}
		}

		else 
		{

			if (SameDirection(cross(vectorAB, planeNormal), vectorAO)) 
			{
				return Line(points = { pointA, pointB }, direction);
			}

			if (SameDirection(planeNormal, vectorAO)) 
			{
				direction = planeNormal;
			}

			else
			{
				points = { pointA, pointC, pointB };
				direction = -planeNormal;
			}
		}

		return false;
	}

	bool CollisionSystem::Tetrahedron(Simplex& points, vec3& direction)
	{
		vec3 pointA = points[0];
		vec3 pointB = points[1];
		vec3 pointC = points[2];
		vec3 pointD = points[3];

		vec3 vectorAB = pointB - pointA;
		vec3 vectorAC = pointC - pointA;
		vec3 vectorAD = pointD - pointA;
		vec3 vectorAO = -pointA;

		vec3 planeABCNormal = cross(vectorAB, vectorAC);
		vec3 planeACDNormal = cross(vectorAC, vectorAD);
		vec3 planeABDNormal = cross(vectorAD, vectorAB);

		if (SameDirection(planeABCNormal, vectorAO)) 
		{
			return Triangle(points = { pointA, pointB, pointC }, direction);
		}

		if (SameDirection(planeACDNormal, vectorAO)) 
		{
			return Triangle(points = { pointA, pointC, pointD }, direction);
		}

		if (SameDirection(planeABDNormal, vectorAO)) 
		{
			return Triangle(points = { pointA, pointD, pointB }, direction);
		}

		return true;
	}
}
