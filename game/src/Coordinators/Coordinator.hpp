#ifndef COORDINATORS_COORDINATOR_HPP
#define COORDINATORS_COORDINATOR_HPP

#include <memory>

#include "../DataStructures/ComponentArray.hpp"
#include "../Managers/ComponentManager.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Managers/EventManager.hpp"
#include "../Managers/SystemManager.hpp"

namespace Coordinators
{
	using namespace DataStructures;
	using namespace Managers;

	class Coordinator
	{
	public:
		Coordinator()
		{
			this->mComponentManager = std::make_unique<ComponentManager>();
			this->mEntityManager = std::make_unique<EntityManager>();
			this->mSystemManager = std::make_unique<SystemManager>();
			this->mEventManager = std::make_unique<EventManager>();
		}

		Entity CreateEntity() const
		{
			return this->mEntityManager->CreateEntity();
		}

		void DestroyEntity(Entity entity) const
		{
			this->mEntityManager->DestroyEntity(entity);
			this->mSystemManager->EntityDestroyed(entity);
			this->mComponentManager->EntityDestroyed(entity);
		}

		template <typename T>
		void RegisterComponent() const
		{
			this->mComponentManager->RegisterComponent<T>();
		}

		template <typename T>
		void AddComponent(Entity entity, T component)
		{
			this->mComponentManager->AddComponent<T>(entity, component);

			auto signature = this->mEntityManager->GetSignature(entity);
			signature.set(this->mComponentManager->GetComponentType<T>(), true);

			this->mEntityManager->SetSignature(entity, signature);
			this->mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		void RemoveComponent(Entity entity) const
		{
			this->mComponentManager->RemoveComponent<T>(entity);

			auto signature = this->mEntityManager->GetSignature(entity);
			signature.set(this->mComponentManager->GetComponentType<T>(), false);

			this->mEntityManager->SetSignature(entity, signature);
			this->mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template <typename T>
		T& GetComponent(Entity entity)
		{
			return this->mComponentManager->GetComponent<T>(entity);
		}

		template <typename T>
		ComponentType GetComponentType() const
		{
			return this->mComponentManager->GetComponentType<T>();
		}

		template <typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return this->mSystemManager->RegisterSystem<T>();
		}

		template <typename T>
		void SetSystemSignature(Signature signature) const
		{
			this->mSystemManager->SetSignature<T>(signature);
		}

		void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener) const
		{
			mEventManager->AddListener(eventId, listener);
		}

		void SendEvent(Event& event) const
		{
			mEventManager->SendEvent(event);
		}

		void SendEvent(EventId eventId) const
		{
			mEventManager->SendEvent(eventId);
		}

	private:
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<EntityManager> mEntityManager;
		std::unique_ptr<SystemManager> mSystemManager;
		std::unique_ptr<EventManager> mEventManager;
	};
}

#endif
