//#ifndef SYSTEMS_MANIFOLD_HPP
//#define SYSTEMS_MANIFOLD_HPP
//
//#include <glm/glm.hpp>
//
//namespace Systems
//{
//	using namespace glm;
//
//	struct Manifold
//	{
//		vec3 mPointA; // Furthest point of mPointA into mPointB
//		vec3 mPointB; // Furthest point of mPointB into mPointA
//		vec3 mNormal; // mPointB – mPointA normalized
//		float mDepth; // Length of mPointB – mPointA
//		bool mHasCollision;
//
//		Manifold();
//		Manifold(CollisionObject* a, CollisionObject* b, const Simplex& points);
//	};
//}
//
//#endif
