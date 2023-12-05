#include "Game.hpp"

#include <random>

#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/LightSource.h"
#include "Components/Playable.hpp"
#include "Components/HitBox.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/ModelLoadingSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/GravitySystem.hpp"
#include "Events/EventTypes.hpp"
#include "Events/EventParameters.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/HitBoxCompositionSystem.hpp"
#include "Systems/HitBoxRenderSystem.hpp"
#include "Systems/LightSystem.hpp"
#include "Systems/PlayerMovementSystem.hpp"


namespace Game
{
	using namespace Systems;
	using namespace Events;

	Game::Game()
	{
		this->mWindow = Window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
		this->mCoordinator = make_shared<Coordinator>();
		this->mShader = std::make_unique<ShaderProgram>("Resources/Shaders/test.vert", "Resources/Shaders/test.frag");
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
		this->mCoordinator->RegisterComponent<RigidBody>();
		this->mCoordinator->RegisterComponent<Gravity>();
		this->mCoordinator->RegisterComponent<Playable>();
		this->mCoordinator->RegisterComponent<HitBox>();
		this->mCoordinator->RegisterComponent<LightSource>();
	}

	void Game::RegisterEntities()
	{
		this->mCamera = this->mCoordinator->CreateEntity();
		this->mCoordinator->AddComponent<Camera>(this->mCamera, { vec3(0.0f, 10.0f, 50.0f) });

		const Entity player1 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(player1, {
			"./Resources/Models/car/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(player1, {
			vec3(-10.0f, 10.0f, 0.0f),
			vec3(0, radians(90.0f), 0),
			vec3(0.02f, 0.02f, 0.02f),
		});
		//football
		//Add config for constant forces
		this->mCoordinator->AddComponent<Gravity>(player1, {
			vec3(0.0f, -9.81f, 0.0f)
		});

		this->mCoordinator->AddComponent<Playable>(player1, {});

		this->mCoordinator->AddComponent<RigidBody>(player1, {});

		this->mCoordinator->AddComponent<HitBox>(player1, {});

		const Entity player2 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(player2, {
			"./Resources/Models/car/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(player2, {
			vec3(10.0f, 10.0f, 0.0f),
			vec3(0, radians(-90.0f), 0),
			vec3(0.02f, 0.02f, 0.02f),
		});

		//Add config for constant forces
		this->mCoordinator->AddComponent<Gravity>(player2, {
			vec3(0.0f, -9.81f, 0.0f)
		});

		this->mCoordinator->AddComponent<Playable>(player2, { 1 });

		this->mCoordinator->AddComponent<RigidBody>(player2, {});

		this->mCoordinator->AddComponent<HitBox>(player2, { });

		const Entity table = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(table, {
			"./Resources/Models/table/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(table, {
			vec3(0.0f, -35.0f, -20.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(150.0f, 150.0f, 150.0f),
		});

		this->mCoordinator->AddComponent<RigidBody>(table, {});

		this->mCoordinator->AddComponent<HitBox>(table, { true });

		const Entity lightSource0 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<LightSource>(lightSource0, { vec4(3.0f, 3.0f, 3.0f, 1.0f) , vec3(0.0f, 10.0f, 10.0f) });

		const Entity lightSource1 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<LightSource>(lightSource1, { vec4(1.0f, 1.0f, 1.0f, 1.0f) , vec3(5.0f, 10.0f, -60.0f) });
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

		const auto playerMovementSystem = this->mCoordinator->RegisterSystem<PlayerMovementSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Playable>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
			this->mCoordinator->SetSystemSignature<PlayerMovementSystem>(signature);
		}
		playerMovementSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(playerMovementSystem);

		const auto cameraSystem = this->mCoordinator->RegisterSystem<CameraSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Camera>());
			this->mCoordinator->SetSystemSignature<CameraSystem>(signature);
		}
		cameraSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(cameraSystem);

		const auto lightSystem = this->mCoordinator->RegisterSystem<LightSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<LightSource>());
			this->mCoordinator->SetSystemSignature<LightSystem>(signature);
		}
		lightSystem->Initialize(this->mCoordinator, this->mShader);
		this->mSystems.push_back(lightSystem);

		const auto hitBoxCompositionSystem = this->mCoordinator->RegisterSystem<HitBoxCompositionSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<HitBox>());
			signature.set(this->mCoordinator->GetComponentType<Renderable>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
			this->mCoordinator->SetSystemSignature<HitBoxCompositionSystem>(signature);
		}
		hitBoxCompositionSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(hitBoxCompositionSystem);

		const auto collisionSystem = this->mCoordinator->RegisterSystem<CollisionSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<HitBox>());
			this->mCoordinator->SetSystemSignature<CollisionSystem>(signature);
		}
		collisionSystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(collisionSystem);

		const auto gravitySystem = this->mCoordinator->RegisterSystem<GravitySystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Gravity>());
			signature.set(this->mCoordinator->GetComponentType<RigidBody>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
			this->mCoordinator->SetSystemSignature<GravitySystem>(signature);
		}
		gravitySystem->Initialize(this->mCoordinator);
		this->mSystems.push_back(gravitySystem);

		const auto hitBoxRenderSystem = this->mCoordinator->RegisterSystem<HitBoxRenderSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<HitBox>());
			this->mCoordinator->SetSystemSignature<HitBoxRenderSystem>(signature);
		}
		hitBoxRenderSystem->Initialize(this->mCoordinator, this->mShader);
		this->mSystems.push_back(hitBoxRenderSystem);

		const auto renderSystem = this->mCoordinator->RegisterSystem<RenderSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Renderable>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
			this->mCoordinator->SetSystemSignature<RenderSystem>(signature);
		}
		renderSystem->Initialize(this->mCoordinator, this->mShader, this->mCamera);
		this->mSystems.push_back(renderSystem);
	}

	void Game::Update(float deltaTime)
	{
		this->mWindow.ProcessEvents(this->mCoordinator);
		for (const auto& system : this->mSystems)
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
