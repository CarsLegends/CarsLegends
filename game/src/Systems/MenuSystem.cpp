#include "MenuSystem.hpp"

#include <utility>

#include "nuklear/nuklear.h"
#include "nuklear/nuklear_glfw_gl3.h"

namespace Systems
{
	MenuSystem::MenuSystem() = default;

	void MenuSystem::Initialize(std::shared_ptr<Coordinator> coordinator, Window window)
	{
		this->mCoordinator = std::move(coordinator);
		this->mNkContext = window.getNkContext();
	}
}
