#include <cassert>

#include "ComponentManager.hpp"

namespace Managers {
	using namespace std;

	template<typename T>
	void ComponentManager::RegisterComponent() {
		const char* typeName = typeid(T).name();
		assert(this->mComponentTypes.find(typeName) == this->mComponentTypes.end() 
				&& "Registering component type more than once.");

		this->mComponentTypes.insert({ typeName, this->mNextComponentType });
		this->mComponentArrays.insert({ typeName, make_shared<ComponentArray<T>>() });

		this->mNextComponentType++;
	}

	template<typename T>
	ComponentType ComponentManager::GetComponentType() {
		const char* typeName = typeid(T).name();
		assert(this->mComponentTypes.find(typeName) != this->mComponentTypes.end() 
				&& "Component not registered.");

		return this->mComponentTypes[typeName];
	}

	template <typename T>
	void ComponentManager::AddComponent(Entity entity, T component) {
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void ComponentManager::RemoveComponent(Entity entity) {
		GetComponentArray<T>()->RemoveData(entity);
	}

	template <typename T>
	T& ComponentManager::GetComponent(Entity entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	void ComponentManager::EntityDestroyed(Entity entity) const {
		for (auto const& pair : this->mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

	template<typename T>
	shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray() {
		const char* typeName = typeid(T).name();
		assert(this->mComponentTypes.find(typeName) != this->mComponentTypes.end() 
				&& "Component not registered before use.");

		return static_pointer_cast<ComponentArray<T>>(this->mComponentArrays[typeName]);
	}
}
