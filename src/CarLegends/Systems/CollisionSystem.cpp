#include "CollisionSystem.hpp"

#include <iostream>

#include "../Components/RigidBody.hpp"
#include "../Components/Transform.hpp"

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
			const auto& hitBoxA = this->mCoordinator->GetComponent<HitBox>(iPair.first);
			Collider colliderA = UpdateVerticesPosition(hitBoxA);

			const auto& hitBoxB = this->mCoordinator->GetComponent<HitBox>(iPair.second);
			Collider colliderB = UpdateVerticesPosition(hitBoxB);

			auto [collision, simplex] = CheckForCollision(colliderA, colliderB);

			if(!collision)
			{
				continue;
			}

			const CollisionData collisionData = EPA(simplex, colliderA, colliderB);
			const vec3 collisionDistance = collisionData.mNormal * collisionData.mDepth;

			auto& transformA = this->mCoordinator->GetComponent<Transform>(iPair.first);
			auto& transformB = this->mCoordinator->GetComponent<Transform>(iPair.second);
			auto& rigidA = this->mCoordinator->GetComponent<RigidBody>(iPair.first);
			auto& rigidB = this->mCoordinator->GetComponent<RigidBody>(iPair.second);

			if (!hitBoxA.mStatic)
			{
				rigidA.mVelocity -= rigidB.mVelocity / 100.0f;
				transformA.mPosition -= collisionDistance;
			}

			if (!hitBoxB.mStatic)
			{
				rigidB.mVelocity += rigidA.mVelocity / 100.0f;
				transformB.mPosition += collisionDistance;
			}

			if (collisionData.mNormal.y == -1.0f)
			{
				if (rigidA.mVelocity.y < 0)
				{
					rigidA.mVelocity.y = 0;
				}

				if (rigidB.mVelocity.y < 0)
				{
					rigidB.mVelocity.y = 0;
				}
			}
		}
	}

	Collider CollisionSystem::UpdateVerticesPosition(const HitBox& hitBox)
	{
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

	CollisionData CollisionSystem::EPA(const Simplex& simplex, const Collider& colliderA, const Collider& colliderB)
	{
		std::vector polytope(simplex.begin(), simplex.end());
		std::vector<size_t> faces = {
			0, 1, 2,
			0, 3, 1,
			0, 2, 3,
			1, 3, 2
		};

		auto [normals, minFace] = GetFaceNormals(polytope, faces);
		vec3  minNormal;
		float minDistance = std::numeric_limits<float>::infinity();

		while (minDistance == std::numeric_limits<float>::infinity())
		{
			minNormal.x = normals[minFace].x;
			minNormal.y = normals[minFace].y;
			minNormal.z = normals[minFace].z;

			minDistance = normals[minFace].w;

			vec3 support = Support(colliderA, colliderB, minNormal);
			const float sDistance = dot(minNormal, support);

			if (abs(sDistance - minDistance) > 0.001f) 
			{
				minDistance = std::numeric_limits<float>::infinity();
				std::vector<std::pair<size_t, size_t>> uniqueEdges;

				for (size_t i = 0; i < normals.size(); i++) 
				{
					if (SameDirection(normals[i], support)) 
					{
						const size_t iFace = i * 3;

						AddIfUniqueEdge(uniqueEdges, faces, iFace, iFace + 1);
						AddIfUniqueEdge(uniqueEdges, faces, iFace + 1, iFace + 2);
						AddIfUniqueEdge(uniqueEdges, faces, iFace + 2, iFace);

						faces[iFace + 2] = faces.back(); faces.pop_back();
						faces[iFace + 1] = faces.back(); faces.pop_back();
						faces[iFace] = faces.back(); faces.pop_back();

						normals[i] = normals.back(); // pop-erase
						normals.pop_back();

						i--;
					}
				}
				std::vector<size_t> newFaces;
				for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) 
				{
					newFaces.push_back(edgeIndex1);
					newFaces.push_back(edgeIndex2);
					newFaces.push_back(polytope.size());
				}

				polytope.push_back(support);

				auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);
				float oldMinDistance = FLT_MAX;
				for (size_t i = 0; i < normals.size(); i++) 
				{
					if (normals[i].w < oldMinDistance) 
					{
						oldMinDistance = normals[i].w;
						minFace = i;
					}
				}

				if (newNormals[newMinFace].w < oldMinDistance) 
				{
					minFace = newMinFace + normals.size();
				}

				faces.insert(faces.end(), newFaces.begin(), newFaces.end());
				normals.insert(normals.end(), newNormals.begin(), newNormals.end());
			}
		}

		return { minNormal,  minDistance };
	}

	std::pair<std::vector<vec4>, size_t> CollisionSystem::GetFaceNormals(const std::vector<vec3>& polytope, const std::vector<size_t>& faces)
	{
		std::vector<vec4> normals;
		size_t minTriangle = 0;
		float  minDistance = std::numeric_limits<float>::infinity();

		for (size_t iFace = 0; iFace < faces.size(); iFace += 3) 
		{
			vec3 faceA = polytope[faces[iFace]];
			vec3 faceB = polytope[faces[iFace + 1]];
			vec3 faceC = polytope[faces[iFace + 2]];

			vec3 normal = normalize(cross(faceB - faceA, faceC - faceA));
			float distance = dot(normal, faceA);

			if (distance < 0) 
			{
				normal *= -1;
				distance *= -1;
			}

			normals.emplace_back(normal, distance);

			if (distance < minDistance) 
			{
				minTriangle = iFace / 3;
				minDistance = distance;
			}
		}

		return { normals, minTriangle };
	}

	void CollisionSystem::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
	{
		const auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));

		if (reverse != edges.end()) 
		{
			edges.erase(reverse);
		}
		else 
		{
			edges.emplace_back(faces[a], faces[b]);
		}
	}
}
