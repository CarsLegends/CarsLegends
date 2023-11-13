#ifndef MANAGERS_COMPONENT_MANAGER_HPP
#define MANAGERS_COMPONENT_MANAGER_HPP
#include <memory>
#include <unordered_map>

#include "../DataStructures/ComponentArray.hpp"

namespace Managers {
	using namespace std;
	using namespace DataStructures;

	class ComponentManager {
	public:
		template<typename T> void RegisterComponent();
		template<typename T> ComponentType GetComponentType();
		template<typename T> void AddComponent(Entity entity, T component);
		template<typename T> void RemoveComponent(Entity entity);
		template<typename T> T& GetComponent(Entity entity);
		void EntityDestroyed(Entity entity) const;
	private:
		unordered_map<const char*, ComponentType> mComponentTypes{};
		unordered_map<const char*, shared_ptr<IComponentArray>> mComponentArrays{};
		ComponentType mNextComponentType{};

		template<typename T> shared_ptr<ComponentArray<T>> GetComponentArray();
	};
}

#endif