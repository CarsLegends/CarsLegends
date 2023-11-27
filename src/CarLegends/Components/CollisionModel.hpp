#ifndef COMPONENTS_COLLISION_MESH_HPP
#define COMPONENTS_COLLISION_MESH_HPP

#include <utility>
#include <vector>
#include <glm/vec3.hpp>

#include "../Engine/Buffers/VertexArray.hpp"

namespace Components
{
	using namespace glm;
	using namespace Buffers;
	using Collider = std::vector<std::vector<vec3>>;

	struct CollisionModel
	{
		Collider mCollider;
		VertexArray mVertexArray{};

		CollisionModel();
		explicit CollisionModel(const Collider& vertices);
	};

	inline CollisionModel::CollisionModel()
	{
		this->mCollider = {};
	}

	inline CollisionModel::CollisionModel(const Collider& collider)
	{
		this->mCollider = Collider(collider);
	}
}

#endif 