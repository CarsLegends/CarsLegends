#ifndef SYSTEMS_ISYSTEM_HPP
#define SYSTEMS_ISYSTEM_HPP

#include <set>

#include "../DataStructures/Types.hpp"

namespace Systems {
	using namespace std;
	using namespace DataStructures;

	class ISystem {
	public:
		set<Entity> mEntities;

		virtual void Update() = 0;
	};
}
#endif