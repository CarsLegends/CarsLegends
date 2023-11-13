#include "Game.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/ModelLoadingSystem.hpp"
#include "Systems/RenderSystem.hpp"

namespace Game {
	extern Coordinator gCoordinator;

	bool Game::IsRunning() const {
		return this->mRunning;
	}

	void Game::RegisterComponents() {
	}

	void Game::RegisterEntities() {
	}

	void Game::RegisterSystems() {
		auto cameraSystem = gCoordinator.RegisterSystem<CameraSystem>();
		{
			
		}

		auto modelLoadingSystem = gCoordinator.RegisterSystem<ModelLoadingSystem>();
		{

		}

		auto renderSystem =  gCoordinator.RegisterSystem<RenderSystem>();
		{

		}
	}

	void Game::Update() {
	}
}
