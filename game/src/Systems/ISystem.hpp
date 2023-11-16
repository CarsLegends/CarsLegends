#ifndef SYSTEMS_ISYSTEM_HPP
#define SYSTEMS_ISYSTEM_HPP

#include <set>

#include "../DataStructures/Types.hpp"

namespace Systems
{
	using namespace DataStructures;

	class ISystem
	{
	public:
		std::set<Entity> mEntities;

		virtual ~ISystem() = default;
		virtual void Update(float deltaTime) = 0;
	};
}
#endif
