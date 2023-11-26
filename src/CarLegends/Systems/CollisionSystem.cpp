#include "CollisionSystem.hpp"

#include <utility>

#include "../Components/CollisionModel.hpp"
#include "../Components/RigidBody.hpp"
#include "../Events/EventTypes.hpp"

namespace Systems
{
	CollisionSystem::CollisionSystem() = default;

	void CollisionSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mCoordinator = std::move(coordinator);
	}

	void CollisionSystem::Update(float deltaTime)
	{
		std::vector<Entity> entities;
		entities.reserve(this->mEntities.size());
		for (auto iEntity : this->mEntities)
		{
			entities.push_back(iEntity);
		}

		const auto entitiesPairs = FindUniquePairsOfEntities(entities);

		for (const auto iPair : entitiesPairs)
		{
			auto& iCollisionModel = this->mCoordinator->GetComponent<CollisionModel>(iPair.first);
			auto& jCollisionModel = this->mCoordinator->GetComponent<CollisionModel>(iPair.second);

			std::vector<Manifold> collisionPoints;
			for (const auto& iColliderMesh : iCollisionModel.mCollider)
			{
				for (const auto& jColliderMesh : jCollisionModel.mCollider)
				{
					auto [collision, simplex] = GJK(jColliderMesh, iColliderMesh);
					if (collision) {
						collisionPoints.push_back(EPA(simplex, jColliderMesh, iColliderMesh));
						std::cout << "COLLISION" << std::endl;
					}
					else
					{
						std::cout << "NO COLLISION" << std::endl;
					}
				}
			}

			if (collisionPoints.empty())
			{
				continue;
			}

			Manifold collisionManifold = GetMaxPen(collisionPoints);
			auto& iRigidBody = this->mCoordinator->GetComponent<RigidBody>(iPair.first);
			auto& jRigidBody = this->mCoordinator->GetComponent<RigidBody>(iPair.second);

			vec3 residualVelocity = jRigidBody.mVelocity - iRigidBody.mVelocity;
			const float angleCos = dot(residualVelocity, collisionManifold.mNormal);

			if (angleCos >= 0)
			{
				continue;
			}

			// Move to constants
			const float percent = 0.8f;
			const float slop = 0.01f;

			const float collisionRestitution = iRigidBody.mRestitution * jRigidBody.mRestitution;
			const float impulseIntensity = -(1.0f + collisionRestitution) * angleCos / (iRigidBody.mMass + jRigidBody.mMass);
			vec3 impulseForce = impulseIntensity * collisionManifold.mNormal;

			iRigidBody.mVelocity -= impulseForce * iRigidBody.mMass;
			jRigidBody.mVelocity += impulseForce * jRigidBody.mMass;

			auto& iTransform = this->mCoordinator->GetComponent<Transform>(iPair.first);
			auto& jTransform = this->mCoordinator->GetComponent<Transform>(iPair.second);

			vec3 correction = collisionManifold.mNormal * percent * fmax(collisionManifold.mDepth - slop, 0.0f) / (iRigidBody.mMass + jRigidBody.mMass);

			iTransform.mPosition -= correction * iRigidBody.mMass;
			jTransform.mPosition += correction * jRigidBody.mMass;
		}

		//auto& iRenderable = this->mCoordinator->GetComponent<Renderable>(iEntity);
		//auto& jRenderable = this->mCoordinator->GetComponent<Renderable>(jEntity);

		//std::vector<Manifold> collisionPoints;
		//for (const auto& iMesh : iRenderable.mMeshes)
		//{
		//	std::vector uniqueVerticesMeshA = { iMesh.mVertices[0].mPosition };
		//	for (auto iVertex : iMesh.mVertices)
		//	{
		//		bool unique = true;
		//		for (const auto uniqueVertex : uniqueVerticesMeshA)
		//		{
		//			if(iVertex.mPosition.x == uniqueVertex.x && 
		//			   iVertex.mPosition.y == uniqueVertex.y &&
		//			   iVertex.mPosition.z == uniqueVertex.z)
		//			{
		//				unique = false;
		//				break;
		//			}
		//		}

		//		if(unique)
		//		{
		//			uniqueVerticesMeshA.push_back(iVertex.mPosition);
		//		}
		//	}

		//	for (const auto& jMesh : jRenderable.mMeshes)
		//	{
		//		std::vector uniqueVerticesMeshB = { iMesh.mVertices[0].mPosition };
		//		for (auto iVertex : jMesh.mVertices)
		//		{
		//			bool unique = true;
		//			for (const auto uniqueVertex : uniqueVerticesMeshB)
		//			{
		//				if (iVertex.mPosition.x == uniqueVertex.x &&
		//					iVertex.mPosition.y == uniqueVertex.y &&
		//					iVertex.mPosition.z == uniqueVertex.z)
		//				{
		//					unique = false;
		//					break;
		//				}
		//			}

		//			if (unique)
		//			{
		//				uniqueVerticesMeshB.push_back(iVertex.mPosition);
		//			}
		//		}

		//		auto [collision, simplex] = GJK(uniqueVerticesMeshA, uniqueVerticesMeshB);
		//		if (collision) {
		//			collisionPoints.push_back(EPA(simplex, iMesh.mVertices, jMesh.mVertices));
		//			/*Event event(COLLISION);
		//			this->mCoordinator->SendEvent(event);*/
		//			std::cout << "COLLISION" << std::endl;
		//		}
		//		else
		//		{
		//			std::cout << "NO COLLISION" << std::endl;
		//		}
		//	}
		//}

		//if (collisionPoints.empty())
		//{
		//	continue;
		//}

		//Manifold collisionManifold = GetMaxPen(collisionPoints);
		//auto& iRigidBody = this->mCoordinator->GetComponent<RigidBody>(iEntity);
		//auto& jRigidBody = this->mCoordinator->GetComponent<RigidBody>(jEntity);

		//vec3 residualVelocity = jRigidBody.mVelocity - iRigidBody.mVelocity;
		//const float angleCos = dot(residualVelocity, collisionManifold.mNormal);

		//if (angleCos >= 0)
		//{
		//	continue;
		//}

		//// Move to constants
		//const float percent = 0.8f;
		//const float slop = 0.01f;

		//const float collisionRestitution = iRigidBody.mRestitution * jRigidBody.mRestitution;
		//const float impulseIntensity = -(1.0f + collisionRestitution) * angleCos / (iRigidBody.mMass + jRigidBody.mMass);
		//vec3 impulseForce = impulseIntensity * collisionManifold.mNormal;

		//iRigidBody.mVelocity -= impulseForce * iRigidBody.mMass;
		//jRigidBody.mVelocity += impulseForce * jRigidBody.mMass;

		//auto& iTransform = this->mCoordinator->GetComponent<Transform>(iEntity);
		//auto& jTransform = this->mCoordinator->GetComponent<Transform>(jEntity);

		//vec3 correction = collisionManifold.mNormal * percent * fmax(collisionManifold.mDepth - slop, 0.0f) / (iRigidBody.mMass + jRigidBody.mMass);

		//iTransform.mPosition -= correction * iRigidBody.mMass;
		//jTransform.mPosition += correction * jRigidBody.mMass;

	}

	std::vector<std::pair<Entity, Entity>> CollisionSystem::FindUniquePairsOfEntities(const std::vector<Entity>& Entities) {
		std::vector<std::pair<Entity, Entity>> uniquePairs;
		std::set<std::pair<Entity, Entity>> uniquePairSet;

		for (size_t iEntity = 0; iEntity < Entities.size(); ++iEntity)
		{
			for (size_t jEntity = iEntity + 1; jEntity < Entities.size(); ++jEntity)
			{
				std::pair<int, int> currentPair(Entities[iEntity], Entities[jEntity]);

				if (uniquePairSet.find(currentPair) == uniquePairSet.end())
				{
					uniquePairs.emplace_back(currentPair);
					uniquePairSet.insert(currentPair);
				}
			}
		}

		return uniquePairs;
	}

	Manifold CollisionSystem::GetMaxPen(const std::vector<Manifold>& manifolds)
	{
		if (manifolds.empty())
		{
			return {};
		}

		size_t maxNormalIndex = 0;
		float  maxNormalDist = std::numeric_limits<float>::max();

		for (size_t i = 0; i < manifolds.size(); i++)
		{
			if (manifolds[i].mDepth > maxNormalDist)
			{
				maxNormalDist = manifolds[i].mDepth;
				maxNormalIndex = i;
			}
		}

		return manifolds[maxNormalIndex];
	}

	Manifold CollisionSystem::EPA(const Simplex& simplex, const Collider& colliderA, const Collider& colliderB)
	{
		std::vector polytope(simplex.begin(), simplex.end());
		static std::vector<size_t> faces = {
			0, 1, 2,
			0, 3, 1,
			0, 2, 3,
			1, 3, 2
		};

		auto [normals, minimumFace] = GetFaceNormals(polytope, faces);

		vec3  minimumNormal;
		float minimumDistance = std::numeric_limits<float>::max();

		while (minimumDistance == std::numeric_limits<float>::max())
		{
			minimumNormal = normals[minimumFace];
			minimumDistance = normals[minimumFace].w;

			vec3 support = Support(colliderA, colliderB, minimumNormal);
			float supportDistance = dot(minimumNormal, support);

			if (abs(supportDistance - minimumDistance) > 0.001f)
			{
				minimumDistance = std::numeric_limits<float>::max();
				std::vector<std::pair<size_t, size_t>> uniqueEdges;

				for (size_t i = 0; i < normals.size(); i++)
				{
					if (dot(normals[i], vec4(support, 0.0f)) > dot(normals[i], vec4(polytope[faces[i * 3]], 0.0f)))
					{
						size_t f = i * 3;

						AddIfUniqueEdge(uniqueEdges, faces, f, f + 1);
						AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
						AddIfUniqueEdge(uniqueEdges, faces, f + 2, f);

						faces[f + 2] = faces.back(); faces.pop_back();
						faces[f + 1] = faces.back(); faces.pop_back();
						faces[f] = faces.back(); faces.pop_back();

						normals[i] = normals.back();
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

				float oldMinDistance = std::numeric_limits<float>::max();
				for (size_t i = 0; i < normals.size(); i++)
				{
					if (normals[i].w < oldMinDistance)
					{
						oldMinDistance = normals[i].w;
						minimumFace = i;
					}
				}

				if (newNormals[newMinFace].w < oldMinDistance)
				{
					minimumFace = newMinFace + normals.size();
				}

				faces.insert(faces.end(), newFaces.begin(), newFaces.end());
				normals.insert(normals.end(), newNormals.begin(), newNormals.end());
			}
		}

		Manifold points;

		points.mNormal = minimumNormal;
		points.mDepth = minimumDistance + 0.001f;
		points.mCollision = true;

		return points;
	}

	std::pair<std::vector<vec4>, size_t> CollisionSystem::GetFaceNormals(const std::vector<vec3>& polytope, const std::vector<size_t>& faces)
	{
		std::vector<vec4> normals;
		size_t minTriangle = 0;
		float  minDistance = std::numeric_limits<float>::max();

		for (size_t i = 0; i < faces.size(); i += 3)
		{
			vec3 a = polytope[faces[i]];
			vec3 b = polytope[faces[i + 1]];
			vec3 c = polytope[faces[i + 2]];

			vec3 normal = normalize(cross(b - a, c - a));
			float distance = dot(normal, a);

			if (distance < 0)
			{
				normal *= -1;
				distance *= -1;
			}

			normals.emplace_back(normal, distance);

			if (distance < minDistance)
			{
				minTriangle = i / 3;
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

	std::pair<bool, Simplex> CollisionSystem::GJK(const Collider& colliderA, const Collider& colliderB)
	{
		vec3 support = Support(colliderA, colliderB, vec3(1, 0, 0));
		Simplex points;
		points.push_front(support);
		vec3 direction = -support;

		while (true)
		{
			support = Support(colliderA, colliderB, direction);

			if (dot(support, direction) <= 0)
			{
				return std::make_pair(false, points);
			}

			points.push_front(support);
			if (NextSimplex(points, direction))
			{
				return std::make_pair(true, points);
			}
		}
	}

	vec3 CollisionSystem::Support(const Collider& colliderA, const Collider& colliderB, vec3 direction)
	{
		return FindFurthestPoint(colliderA, direction) - FindFurthestPoint(colliderB, -direction);
	}

	vec3 CollisionSystem::FindFurthestPoint(const Collider& collider, vec3 direction)
	{
		vec3  maxPoint = {};
		float maxDistance = std::numeric_limits<float>::lowest();

		for (auto& vertex : collider)
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

	bool CollisionSystem::SameDirection(const vec3& direction, const vec3& ao)
	{
		return dot(direction, ao) > 0;
	}

	bool CollisionSystem::NextSimplex(Simplex& points, vec3& direction)
	{
		switch (points.size())
		{
		case 2:
			return Line(points, direction);
		case 3:
			return Triangle(points, direction);
		case 4:
			return Tetrahedron(points, direction);
		default:
			std::cout << "Unexpected Simplex of size: " << points.size() << std::endl;
			abort();
		}
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
		else
		{
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

		vec3 vectorAO = -pointA;

		vec3 vectorAB = pointB - pointA;
		vec3 vectorAC = pointC - pointA;
		vec3 planeNormalABC = cross(vectorAB, vectorAC);
		if (SameDirection(planeNormalABC, vectorAO))
		{
			return Triangle(points = { pointA, pointB, pointC }, direction);
		}

		vec3 pointD = points[3];
		vec3 vectorAD = pointD - pointA;
		vec3 planeNormalACD = cross(vectorAC, vectorAD);
		if (SameDirection(planeNormalACD, vectorAO))
		{
			return Triangle(points = { pointA, pointC, pointD }, direction);
		}

		vec3 planeNormalADB = cross(vectorAD, vectorAB);
		if (SameDirection(planeNormalADB, vectorAO))
		{
			return Triangle(points = { pointA, pointD, pointB }, direction);
		}

		return true;
	}
}
