#ifndef COORDINATORS_COORDINATOR_HPP
#define COORDINATORS_COORDINATOR_HPP

#include <memory>

#include "../DataStructures/ComponentArray.hpp"
#include "../Managers/ComponentManager.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Managers/SystemManager.hpp"

namespace Coordinators {
	using namespace std;
	using namespace	DataStructures;
	using namespace Managers;

	class Coordinator {
	public:    
		Coordinator();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		template<typename T> void RegisterComponent();
		template<typename T> void AddComponent(Entity entity, T component);
		template<typename T> void RemoveComponent(Entity entity);
		template<typename T> T& GetComponent(Entity entity);
		template<typename T> ComponentType GetComponentType();

		template<typename T> shared_ptr<T> RegisterSystem();
		template<typename T> void SetSystemSignature(Signature signature);
	private:
		unique_ptr<ComponentManager> mComponentManager;
		unique_ptr<EntityManager> mEntityManager;
		unique_ptr<SystemManager> mSystemManager;
	};
}

#endif