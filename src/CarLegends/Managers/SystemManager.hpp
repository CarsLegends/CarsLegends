#ifndef MANAGERS_SYSTEM_MANAGER_HPP
#define MANAGERS_SYSTEM_MANAGER_HPP

#include <memory>
#include <unordered_map>

#include "../DataStructures/Types.hpp"
#include "../Systems/ISystem.hpp"

namespace Managers {
	using namespace std;
	using namespace DataStructures;
	using namespace Systems;

	class SystemManager {
	public:
		template<typename T> shared_ptr<T> RegisterSystem();
		template<typename T> void SetSignature(Signature signature);
		void EntitySignatureChanged(Entity entity, Signature entitySignature);
		void EntityDestroyed(Entity entity);

	private:
		unordered_map<const char*, Signature> mSignatures{};
		unordered_map<const char*, shared_ptr<ISystem>> mSystems{};
	};
}
#endif