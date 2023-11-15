#ifndef MANAGERS_SYSTEM_MANAGER_HPP
#define MANAGERS_SYSTEM_MANAGER_HPP

#include <cassert>
#include <memory>
#include <unordered_map>

#include "../Systems/ISystem.hpp"

namespace Managers
{
	using namespace DataStructures;
	using namespace Systems;

	class SystemManager
	{
	public:
		template <typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			assert(this->mSystems.find(typeName) == this->mSystems.end() && "Registering system more than once.");

			auto system = std::make_shared<T>();
			this->mSystems.insert({ typeName, system });
			return system;
		}

		template <typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			assert(this->mSystems.find(typeName) == this->mSystems.end() && "Registering system more than once.");

			this->mSignatures.insert({ typeName, signature });
		}

		void EntitySignatureChanged(Entity entity, Signature entitySignature)
		{
			for (const auto& pair : mSystems)
			{
				const auto& type = pair.first;
				const auto& system = pair.second;
				const auto& systemSignature = this->mSignatures[type];

				if ((entitySignature & systemSignature) == systemSignature)
				{
					system->mEntities.insert(entity);
				}
				else
				{
					system->mEntities.erase(entity);
				}
			}
		}

		void EntityDestroyed(Entity entity) const
		{
			for (const auto& pair : this->mSystems)
			{
				const auto& system = pair.second;
				system->mEntities.erase(entity);
			}
		}

	private:
		std::unordered_map<const char*, Signature> mSignatures{};
		std::unordered_map<const char*, std::shared_ptr<ISystem>> mSystems{};
	};
}
#endif
