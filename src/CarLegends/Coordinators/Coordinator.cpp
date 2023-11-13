#include "Coordinator.hpp"

namespace Coordinators {
	Coordinator::Coordinator() {
		this->mComponentManager = make_unique<ComponentManager>();
		this->mEntityManager = make_unique<EntityManager>();
		this->mSystemManager = make_unique<SystemManager>();
	}

	Entity Coordinator::CreateEntity() {
		return this->mEntityManager->CreateEntity();
	}

	void Coordinator::DestroyEntity(Entity entity) {
		this->mEntityManager->RemoveEntity(entity);
		this->mSystemManager->EntityDestroyed(entity);
		this->mComponentManager->EntityDestroyed(entity);
	}

	template <typename T>
	void Coordinator::RegisterComponent() {
		this->mComponentManager->RegisterComponent<T>();
	}

	template <typename T>
	void Coordinator::AddComponent(Entity entity, T component) {
		this->mComponentManager->AddComponent<T>(entity, component);

		auto signature = this->mEntityManager->GetSignature(entity);
		signature.set(this->mComponentManager->GetComponentType<T>(), true);

		this->mEntityManager->SetSignature(entity, signature);
		this->mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template <typename T>
	void Coordinator::RemoveComponent(Entity entity) {
		this->mComponentManager->RemoveComponent<T>(entity);

		auto signature = this->mEntityManager->GetSignature(entity);
		signature.set(this->mComponentManager->GetComponentType<T>(), false);

		this->mEntityManager->SetSignature(entity, signature);
		this->mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template <typename T>
	T& Coordinator::GetComponent(Entity entity) {
		return this->mComponentManager->GetComponent<T>(entity);
	}

	template <typename T>
	ComponentType Coordinator::GetComponentType() {
		return this->mComponentManager->GetComponentType<T>();
	}

	template <typename T>
	shared_ptr<T> Coordinator::RegisterSystem() {
		return this->mSystemManager->RegisterSystem<T>();
	}

	template <typename T>
	void Coordinator::SetSystemSignature(Signature signature) {
		this->mSystemManager->SetSignature<T>(signature);
	}
}
