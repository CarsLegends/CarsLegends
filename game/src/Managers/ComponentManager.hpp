#ifndef MANAGERS_COMPONENT_MANAGER_HPP
#define MANAGERS_COMPONENT_MANAGER_HPP
#include <cassert>
#include <memory>
#include <unordered_map>

#include "../DataStructures/ComponentArray.hpp"

namespace Managers
{
	using namespace DataStructures;

	class ComponentManager
	{
	public:
		template <typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();
			assert(this->mComponentTypes.find(typeName) == this->mComponentTypes.end()
				&& "Registering component type more than once.");

			this->mComponentTypes.insert({ typeName, this->mNextComponentType });
			this->mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			this->mNextComponentType++;
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();
			assert(this->mComponentTypes.find(typeName) != this->mComponentTypes.end()
				&& "Component not registered.");

			return this->mComponentTypes[typeName];
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template <typename T>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<T>()->RemoveData(entity);
		}

		template <typename T>
		T& GetComponent(Entity entity)
		{
			return GetComponentArray<T>()->GetData(entity);
		}

		void EntityDestroyed(Entity entity) const
		{
			for (const auto& pair : this->mComponentArrays)
			{
				const auto& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<const char*, ComponentType> mComponentTypes{};
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
		ComponentType mNextComponentType{};

		template <typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();
			assert(this->mComponentTypes.find(typeName) != this->mComponentTypes.end()
				&& "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(this->mComponentArrays[typeName]);
		}
	};
}

#endif
