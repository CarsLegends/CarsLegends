#ifndef DATA_STRUCTURES_COMPONENT_ARRAY_HPP
#define DATA_STRUCTURES_COMPONENT_ARRAY_HPP

#include <array>
#include <unordered_map>

#include "Types.hpp"
#include "../Configurations/ManagersConfiguration.hpp"

namespace DataStructures
{
	using namespace Configuration;

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template <typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(Entity entity, T component)
		{
			assert(this->mEntityToIndexMap.find(entity) == this->mEntityToIndexMap.end()
				&& "Component added to same entity more than once.");

			const size_t newIndex = this->mSize;
			this->mEntityToIndexMap[entity] = newIndex;
			this->mIndexToEntityMap[newIndex] = entity;

			this->mComponentArray[newIndex] = component;

			++this->mSize;
		}

		void RemoveData(Entity entity)
		{
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

		T& GetData(Entity entity)
		{
			assert(this->mEntityToIndexMap.find(entity) != this->mEntityToIndexMap.end()
				&& "Retrieving non-existent component.");

			return this->mComponentArray[this->mEntityToIndexMap[entity]];
		}

		void EntityDestroyed(Entity entity)
		{
			if (this->mEntityToIndexMap.find(entity) != this->mEntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}

	private:
		std::array<T, MAX_ENTITIES> mComponentArray;
		std::unordered_map<Entity, size_t> mEntityToIndexMap;
		std::unordered_map<size_t, Entity> mIndexToEntityMap;
		size_t mSize = 0;
	};
}

#endif
