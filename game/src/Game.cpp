#include "Game.hpp"

#include <random>

#include "Components/Transform.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/ModelLoadingSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Events/EventTypes.hpp"
#include "Events/EventParameters.hpp"

namespace Game
{
	using namespace Systems;
	using namespace Events;

	Game::Game()
	{
		this->mWindow = Window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
		this->mCoordinator = make_shared<Coordinator>();
		this->mRunning = true;
	}

	Game::~Game()
	{
		this->mWindow.Shutdown();
	}

	bool Game::IsRunning() const
	{
		return this->mRunning;
	}

	void Game::RegisterListeners()
	{
		this->mCoordinator->AddEventListener(WINDOW_QUIT, [this](auto&& PH1)
		{
			Game::QuitHandler(std::forward<decltype(PH1)>(PH1));
		});

		this->mCoordinator->AddEventListener(WINDOW_HIDE_CURSOR, [this](auto&& PH1)
		{
			Game::HideCursorHandler(std::forward<decltype(PH1)>(PH1));
		});

		this->mCoordinator->AddEventListener(WINDOW_SHOW_CURSOR, [this](auto&& PH1)
		{
			Game::ShowCursorHandler(std::forward<decltype(PH1)>(PH1));
		});

		this->mCoordinator->AddEventListener(WINDOW_SET_CURSOR_POSITION, [this](auto&& PH1)
		{
			Game::SetCursorPositionHandler(std::forward<decltype(PH1)>(PH1));
		});
	}

	void Game::RegisterComponents() const
	{
		this->mCoordinator->RegisterComponent<Camera>();
		this->mCoordinator->RegisterComponent<Renderable>();
		this->mCoordinator->RegisterComponent<Transform>();
	}

	void Game::RegisterEntities() const
	{
		std::default_random_engine generator;
		std::uniform_real_distribution randPosition(-20.0f, 20.0f);
		std::uniform_int_distribution randRotation(0, 1);
		std::uniform_real_distribution randScale(0.0f, 0.5f);
		std::uniform_real_distribution randAngle(0.0f, 360.0f);

		int counter = 0;
		while (counter < MAX_ENTITIES - 1)
		{
			const Entity entity = this->mCoordinator->CreateEntity();

			this->mCoordinator->AddComponent<Renderable>(entity, { 
				"./Resources/Models/cat/scene.gltf"
			});

			auto scale = randScale(generator);
			this->mCoordinator->AddComponent<Transform>(entity, {
				vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
				vec3(randRotation(generator), randRotation(generator), randRotation(generator)),
				vec3(scale, scale, scale),
				randAngle(generator)
			});
			counter++;
		}
	}

	void Game::RegisterSystems()
	{
		const auto modelLoadingSystem = this->mCoordinator->RegisterSystem<ModelLoadingSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Renderable>());
			this->mCoordinator->SetSystemSignature<ModelLoadingSystem>(signature);
		}
		modelLoadingSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(modelLoadingSystem);

		const auto cameraSystem = this->mCoordinator->RegisterSystem<CameraSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Camera>());
			this->mCoordinator->SetSystemSignature<CameraSystem>(signature);
		}
		cameraSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(cameraSystem);

		const auto renderSystem = this->mCoordinator->RegisterSystem<RenderSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Renderable>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
			this->mCoordinator->SetSystemSignature<RenderSystem>(signature);
		}
		renderSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(renderSystem);
	}

	void Game::Update(float deltaTime)
	{
		this->mWindow.ProcessEvents(this->mCoordinator);
		for(const auto& system : this->mSystems)
		{
			system->Update(deltaTime);
		}
		this->mWindow.Update();
	}

	void Game::QuitHandler(Event& event)
	{
		this->mRunning = false;
	}

	void Game::ShowCursorHandler(Event& event) const
	{
		this->mWindow.ShowCursor();
	}

	void Game::HideCursorHandler(Event& event) const
	{
		this->mWindow.HideCursor();
	}

	void Game::SetCursorPositionHandler(Event& event) const
	{
		this->mWindow.SetCursorPosition(event.GetParam<vec2>(WINDOW_SET_CURSOR_PARAMETER));
	}
}
