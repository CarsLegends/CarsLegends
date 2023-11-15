#include "Game.hpp"

#include "Systems/CameraSystem.hpp"
#include "Systems/ModelLoadingSystem.hpp"
#include "Systems/RenderSystem.hpp"

namespace Game
{
	using namespace Systems;
	using namespace Coordinators;


	Game::Game()
	{
		this->mRunning = true;
	}

	bool Game::IsRunning() const
	{
		return this->mRunning;
	}

	void Game::RegisterComponents()
	{
	}

	void Game::RegisterEntities()
	{
	}

	void Game::RegisterSystems() const
	{
		auto cameraSystem = this->mCoordinator->RegisterSystem<CameraSystem>();
		{
		}
		cameraSystem->Initialize(this->mCoordinator);

		auto modelLoadingSystem = this->mCoordinator->RegisterSystem<ModelLoadingSystem>();
		{
		}
		modelLoadingSystem->Initialize(this->mCoordinator);

		auto renderSystem = this->mCoordinator->RegisterSystem<RenderSystem>();
		{
		}
		renderSystem->Initialize(this->mCoordinator);
	}

	void Game::Update()
	{
	}
}
