#include "Game.hpp"

#include <random>

#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Playable.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/ModelLoadingSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/GravitySystem.hpp"
#include "Events/EventTypes.hpp"
#include "Events/EventParameters.hpp"
#include "Systems/CollisionModelSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/PlayerMovementSystem.hpp"


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
		this->mCoordinator->RegisterComponent<RigidBody>();
		this->mCoordinator->RegisterComponent<Gravity>();
		this->mCoordinator->RegisterComponent<CollisionModel>();
		this->mCoordinator->RegisterComponent<Playable>();
	}

	void Game::RegisterEntities() const
	{
		const Entity car = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(car, {
				"./Resources/Models/car/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(car, {
			vec3(0.0f, 0.0f, 0.0f),
			vec3(1.0f, 0.0f, 0.0f),
			vec3(0.02f, 0.02f, 0.02f),
			0.0f
		});

		//Add config for constant forces
		this->mCoordinator->AddComponent<Gravity>(car, {
			vec3(0.0f, -9.81f, 0.0f)
		});

		this->mCoordinator->AddComponent<CollisionModel>(car, {});

		this->mCoordinator->AddComponent<Playable>(car, {});
		
		this->mCoordinator->AddComponent<RigidBody>(car, {});

		const Entity table = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(table, {
			"./Resources/Models/table/scene.gltf"
		});

		this->mCoordinator->AddComponent<CollisionModel>(table, {});

		this->mCoordinator->AddComponent<Transform>(table, {
			vec3(0.0f, -30.0f, -50.0f),
			vec3(1.0f, 0.0f, 0.0f),
			vec3(100.0f, 100.0f, 100.0f),
			0.0f
		});

		this->mCoordinator->AddComponent<RigidBody>(table, {});

		//const Entity car = this->mCoordinator->CreateEntity();

		//this->mCoordinator->AddComponent<Renderable>(car, {
		//		"./Resources/Models/car/scene.gltf"
		//});

		//this->mCoordinator->AddComponent<Transform>(car, {
		//	vec3(1.0f, 0.5f, 0.5f),
		//	vec3(1.0f, 0.0f, 0.0f),
		//	vec3(0.02f, 0.02f, 0.02f),
		//	0.0f
		//});

		//const Entity cardBox = this->mCoordinator->CreateEntity();

		//this->mCoordinator->AddComponent<Renderable>(cardBox, {
		//		"./Resources/Models/cardbox/scene.gltf"
		//});

		//this->mCoordinator->AddComponent<Transform>(cardBox, {
		//	vec3(1.0f, 1.0f, 0.0f),
		//	vec3(0.0f, 1.0f, 0.0f),
		//	vec3(5.0f, 5.0f, 5.0f),
		//	0.0f
		//});

		/*std::default_random_engine generator;
		std::uniform_real_distribution randPosition(-20.0f, 20.0f);
		std::uniform_int_distribution randRotation(0, 1);
		std::uniform_real_distribution randScale(0.0f, 0.5f);
		std::uniform_real_distribution randAngle(0.0f, 360.0f);

		int counter = 0;
		while (counter < MAX_ENTITIES - 1)
		{
			const Entity cat = this->mCoordinator->CreateEntity();

			this->mCoordinator->AddComponent<Renderable>(cat, {
				"./Resources/Models/cat/scene.gltf"
			});

			const auto scale = randScale(generator);
			this->mCoordinator->AddComponent<Transform>(cat, {
				vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
				vec3(randRotation(generator), randRotation(generator), randRotation(generator)),
				vec3(scale, scale, scale),
				randAngle(generator)
			});

			this->mCoordinator->AddComponent<Gravity>(cat, {
				vec3(0.0f, -0.98f, 0.0f)
			});

			this->mCoordinator->AddComponent<RigidBody>(cat, {});

			counter++;
		}*/
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

		//const auto collisionModelSystem = this->mCoordinator->RegisterSystem<CollisionModelSystem>();
		//{
		//	Signature signature;
		//	signature.set(this->mCoordinator->GetComponentType<Renderable>());
		//	signature.set(this->mCoordinator->GetComponentType<CollisionModel>());
		//	signature.set(this->mCoordinator->GetComponentType<Transform>());
		//	this->mCoordinator->SetSystemSignature<CollisionModelSystem>(signature);
		//}
		//collisionModelSystem->Initialize(this->mCoordinator);
		//this->mSystems.push_back(collisionModelSystem);

		//const auto collisionSystem = this->mCoordinator->RegisterSystem<CollisionSystem>();
		//{
		//	Signature signature;
		//	signature.set(this->mCoordinator->GetComponentType<RigidBody>());
		//	signature.set(this->mCoordinator->GetComponentType<CollisionModel>());
		//	signature.set(this->mCoordinator->GetComponentType<Transform>());
		//	this->mCoordinator->SetSystemSignature<CollisionSystem>(signature);
		//}
		//collisionSystem->Initialize(this->mCoordinator);
		//this->mSystems.push_back(collisionSystem);

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

		const auto renderSystem = this->mCoordinator->RegisterSystem<RenderSystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Renderable>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
			signature.set(this->mCoordinator->GetComponentType<CollisionModel>());
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
