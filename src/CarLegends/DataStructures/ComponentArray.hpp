#ifndef DATA_STRUCTURES_COMPONENT_ARRAY_HPP
#define  DATA_STRUCTURES_COMPONENT_ARRAY_HPP

#include <array>
#include <unordered_map>

#include "Types.hpp"
#include "../Configurations/ManagersConfiguration.hpp"

namespace DataStructures {
	using namespace std;
	using namespace Configuration;

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray {
	public:
		void InsertData(Entity entity, T component);
		void RemoveData(Entity entity);
		T& GetData(Entity entity);
		void EntityDestroyed(Entity entity) override;

	private:
		array<T, MAX_ENTITIES> mComponentArray;
		unordered_map<Entity, size_t> mEntityToIndexMap;
		unordered_map<size_t, Entity> mIndexToEntityMap;
		size_t mSize;
	};
}

#endif