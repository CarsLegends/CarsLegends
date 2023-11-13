#ifndef MANAGERS_ENTITY_MANAGER_HPP
#define MANAGERS_ENTITY_MANAGER_HPP

#include <cstdint>
#include <queue>

#include "../DataStructures/Types.hpp"

namespace Managers {
	using namespace std;
	using namespace DataStructures;
	using namespace Configuration;

	class EntityManager {
	public:
		EntityManager();

		Entity CreateEntity();
		void RemoveEntity(Entity entity);

		void SetSignature(Entity entity, Signature signature);
		Signature GetSignature(Entity entity) const;
		
	private:
		queue<Entity> mEntities{};
		vector<Signature> mSignatures{};
		uint32_t mLivingEntitiesCount;
	};
}
#endif
