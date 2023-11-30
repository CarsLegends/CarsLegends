#ifndef SYSTEMS_MENU_SYSTEM_HPP
#define SYSTEMS_MENU_SYSTEM_HPP

#include <memory>

#include "ISystem.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Engine/Window/Window.hpp"

namespace Systems
{
	using namespace Coordinators;
	using namespace Windows;

	class MenuSystem : public ISystem
	{
	public:
		MenuSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator, Window window);
		void Update(float deltaTime);
		nk_context* getNkContext() const;
		
	private:
		struct nk_context* mNkContext;
		std::shared_ptr<Coordinator> mCoordinator;
	};
}

#endif // SYSTEMS_MENU_SYSTEM_HPP
