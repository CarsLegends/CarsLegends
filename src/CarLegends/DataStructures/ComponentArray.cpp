#include "ComponentArray.hpp"

#include <cassert>

namespace DataStructures {
	template <typename T>
	void ComponentArray<T>::InsertData(Entity entity, T component) {
		assert(this->mEntityToIndexMap.find(entity) == this->mEntityToIndexMap.end() 
				&& "Component added to same entity more than once.");

		const size_t newIndex = this->mSize;
		this->mEntityToIndexMap[entity] = newIndex;
		this->mIndexToEntityMap[newIndex] = entity;

		this->mComponentArray[newIndex] = component;

		++this->mSize;
	}

	template <typename T>
	void ComponentArray<T>::RemoveData(Entity entity) {
		assert(this->mEntityToIndexMap.find(entity) != this->mEntityToIndexMap.end()
				&& "Removing non-existent component.");

		size_t lastEntityIndex = this->mSize - 1;
		size_t currentEntityIndex = this->mEntityToIndexMap[entity];

		this->mComponentArray[currentEntityIndex] = this->mComponentArray[lastEntityIndex];

		const Entity lastEntity = this->mIndexToEntityMap[lastEntityIndex];
		this->mEntityToIndexMap[lastEntity] = currentEntityIndex;
		this->mIndexToEntityMap[currentEntityIndex] = lastEntity;

		this->mEntityToIndexMap.erase(entity);
		this->mIndexToEntityMap.erase(lastEntityIndex);

		--this->mSize;
	}

	template <typename T>
	T& ComponentArray<T>::GetData(Entity entity) {
		assert(this->mEntityToIndexMap.find(entity) != this->mEntityToIndexMap.end()
				&& "Retrieving non-existent component.");

		return this->mComponentArray[this->mEntityToIndexMap[entity]];
	}

	template <typename T>
	void ComponentArray<T>::EntityDestroyed(Entity entity) {
		if (this->mEntityToIndexMap.find(entity) != this->mEntityToIndexMap.end()) {
			RemoveData(entity);
		}
	}
}
