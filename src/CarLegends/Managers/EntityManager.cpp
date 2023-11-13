#include "EntityManager.hpp"

#include <cassert>

namespace Managers {
	EntityManager::EntityManager() {
		this->mLivingEntitiesCount = 0;
		this->mSignatures.reserve(MAX_ENTITIES);
		for(Entity iEntity = 0; iEntity < MAX_ENTITIES; ++iEntity) {
			this->mEntities.push(iEntity);
		}
	}

	Entity EntityManager::CreateEntity() {
		assert(this->mLivingEntitiesCount < MAX_ENTITIES && "The max ammount of entities is exceed");

		const auto entity = this->mEntities.front();
		this->mLivingEntitiesCount++;

		this->mEntities.pop();

		return entity;
	}

	void EntityManager::RemoveEntity(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity value out of range");

		this->mSignatures[entity].reset();
		this->mEntities.push(entity);

		this->mLivingEntitiesCount--;
	}

	void EntityManager::SetSignature(Entity entity, Signature signature) {
		assert(entity < MAX_ENTITIES && "Entity value out of range");

		this->mSignatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(Entity entity) const {
		assert(entity < MAX_ENTITIES && "Entity value out of range");

		return this->mSignatures[entity];
	}
}
