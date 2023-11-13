//#include "Coordinator.hpp"
//
//namespace Coordinators
//{
//	Coordinator::Coordinator()
//	{
//		this->mComponentManager = std::make_unique<ComponentManager>();
//		this->mEntityManager = std::make_unique<EntityManager>();
//		this->mSystemManager = std::make_unique<SystemManager>();
//	}
//
//	Entity Coordinator::CreateEntity() const
//	{
//		return this->mEntityManager->CreateEntity();
//	}
//
//	void Coordinator::DestroyEntity(Entity entity) const
//	{
//		this->mEntityManager->DestroyEntity(entity);
//		this->mSystemManager->EntityDestroyed(entity);
//		this->mComponentManager->EntityDestroyed(entity);
//	}
//
//	template <typename T>
//	void Coordinator::RegisterComponent() const
//	{
//		this->mComponentManager->RegisterComponent<T>();
//	}
//
//	template <typename T>
//	void Coordinator::AddComponent(Entity entity, T component)
//	{
//		this->mComponentManager->AddComponent<T>(entity, component);
//
//		auto signature = this->mEntityManager->GetSignature(entity);
//		signature.set(this->mComponentManager->GetComponentType<T>(), true);
//
//		this->mEntityManager->SetSignature(entity, signature);
//		this->mSystemManager->EntitySignatureChanged(entity, signature);
//	}
//
//	template <typename T>
//	void Coordinator::RemoveComponent(Entity entity) const
//	{
//		this->mComponentManager->RemoveComponent<T>(entity);
//
//		auto signature = this->mEntityManager->GetSignature(entity);
//		signature.set(this->mComponentManager->GetComponentType<T>(), false);
//
//		this->mEntityManager->SetSignature(entity, signature);
//		this->mSystemManager->EntitySignatureChanged(entity, signature);
//	}
//
//	template <typename T>
//	T& Coordinator::GetComponent(Entity entity)
//	{
//		return this->mComponentManager->GetComponent<T>(entity);
//	}
//
//	template <typename T>
//	ComponentType Coordinator::GetComponentType() const
//	{
//		return this->mComponentManager->GetComponentType<T>();
//	}
//
//	template <typename T>
//	std::shared_ptr<T> Coordinator::RegisterSystem()
//	{
//		return this->mSystemManager->RegisterSystem<T>();
//	}
//
//	template <typename T>
//	void Coordinator::SetSystemSignature(Signature signature) const
//	{
//		this->mSystemManager->SetSignature<T>(signature);
//	}
//}
