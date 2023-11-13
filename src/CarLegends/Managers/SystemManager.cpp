#include <cassert>

#include "SystemManager.hpp"

namespace Managers {
	using namespace std;

	template <typename T>
	shared_ptr<T> SystemManager::RegisterSystem() {
		const char* typeName = typeid(T).name();
		assert(this->mSystems.find(typeName) == this->mSystems.end() && "Registering system more than once.");

		auto system = make_shared<T>();
		this->mSystems.insert({ typeName, system });
		return system;
	}

	template <typename T>
	void SystemManager::SetSignature(Signature signature) {
		const char* typeName = typeid(T).name();
		assert(this->mSystems.find(typeName) == this->mSystems.end() && "Registering system more than once.");

		this->mSignatures.insert({ typeName, signature });
	}

	void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature) {
		for (auto const& pair : mSystems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = this->mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature) {
				system->mEntities.insert(entity);
			} else {
				system->mEntities.erase(entity);
			}
		}
	}

	void SystemManager::EntityDestroyed(Entity entity) {
		for (auto const& pair : this->mSystems) {
			auto const& system = pair.second;
			system->mEntities.erase(entity);
		}
	}
}
