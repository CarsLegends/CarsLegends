#include <random>

#include "Game.hpp"

#include <chrono>
#include "raylib.h"
#include "Components/Buttons.hpp"
#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/LightSource.h"
#include "Components/Playable.hpp"
#include "Components/HitBox.hpp"
#include "Components/Labels.hpp"
#include "Systems/CameraSystem.hpp"
#include "Systems/ModelLoadingSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/GravitySystem.hpp"
#include "Events/EventTypes.hpp"
#include "Events/EventParameters.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/GUISystem.hpp"
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
		this->mGameStarted = false;
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
		this->mCoordinator->AddEventListener(WINDOW_START_GAME, [this](auto&& PH1)
		{
			Game::StartGameHandler(std::forward<decltype(PH1)>(PH1));
		});

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

		this->mCoordinator->AddEventListener(WINDOW_GAME_FPS, [this](auto&& PH1)
		{
			Game::FPSHandler(std::forward<decltype(PH1)>(PH1));
		});

		this->mCoordinator->AddEventListener(WINDOW_GAME_GOAL, [this](auto&& PH1)
		{
			Game::GoalHandler(std::forward<decltype(PH1)>(PH1));
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
		this->mCoordinator->RegisterComponent<Labels>();
		this->mCoordinator->RegisterComponent<Buttons>();
	}

	void Game::RegisterEntities()
	{
		this->mCamera = this->mCoordinator->CreateEntity();
		this->mCoordinator->AddComponent<Camera>(this->mCamera, { vec3(0.0f, 2.5f, 50.0f) });

		this->mUserInterface = this->mCoordinator->CreateEntity();

		const vector buttons = {
			Button({}, { 0.8f, 0.34f, 0.0f }, 400, 100, "Start", WINDOW_START_GAME, true).CenterHorizontally().CenterVertically().SetFontSize(128),
			Button({}, { 0.8f, 0.34f, 0.0f }, 400, 100, "Exit", WINDOW_QUIT, false).CenterHorizontally().CenterVertically().Move({ 0.0f, -150.0f }).SetFontSize(128),
		};

		const vector labels = {
			Label{ "Fps: 0, ms: 0.0", { 0.91f, 0.88f, 0.83f } , 64 }.Move({ 25.0f, 25.0f }),
			Label{ "Car Legends", { 0.91f, 0.88f, 0.83f } , 384, true }.CenterHorizontally().CenterVertically().Move({ 0.0f, 175.0f }),
			Label{ "", { 0.91f, 0.88f, 0.83f } , 128 }.Move({ 25.0f, WINDOW_HEIGHT - 50.0f }),
			Label{ "", { 0.91f, 0.88f, 0.83f } , 128 }.Move({ WINDOW_WIDTH - 250.0f, WINDOW_HEIGHT - 50.0f }),
		};

		this->mCoordinator->AddComponent<Buttons>(this->mUserInterface, Buttons(buttons));
		this->mCoordinator->AddComponent<Labels>(this->mUserInterface, Labels(labels));

		const Entity firstLightSource = this->mCoordinator->CreateEntity();
		this->mCoordinator->AddComponent<LightSource>(firstLightSource, { vec4(1.2f, 1.2f, 1.2f, 1.0f) , vec3(5.0f, 5.0f, 10.0f) });

		const Entity secondLightSource = this->mCoordinator->CreateEntity();
		this->mCoordinator->AddComponent<LightSource>(secondLightSource, { vec4(1.2f, 1.2f, 1.2f, 1.0f) , vec3(-5.0f, 10.0f, 10.0f) });

		const Entity thirdLightSource = this->mCoordinator->CreateEntity();
		this->mCoordinator->AddComponent<LightSource>(thirdLightSource, { vec4(2.0f, 2.0f, 2.0f, 1.0f) , vec3(0.0f, 2.5f, 10.0f) });

		this->mStartMenuEntities.push_back(firstLightSource);
		this->mStartMenuEntities.push_back(secondLightSource);
		this->mStartMenuEntities.push_back(thirdLightSource);

		std::default_random_engine generator;
		std::uniform_real_distribution randPosition(-20.0f, 20.0f);
		std::uniform_real_distribution randRotation(0.0f, 360.0f);
		std::uniform_real_distribution randScale(0.0f, 0.1f);
		std::uniform_int_distribution rand3DModel(0, 4);

		for(int i = 0; i < 16; ++i)
		{
			float scale = randScale(generator);
			const int modelId = rand3DModel(generator);

			const Entity iEntity = this->mCoordinator->CreateEntity();
			this->mStartMenuEntities.push_back(iEntity);

			switch (modelId)
			{
			case 0:
				scale += 1.0f;
				this->mCoordinator->AddComponent<Renderable>(iEntity, {
					"./Resources/Models/car5/scene.gltf"
				});
				break;
			case 1:
				scale += 100.0f;
				this->mCoordinator->AddComponent<Renderable>(iEntity, {
					"./Resources/Models/car3/scene.gltf"
				});
				break;
			case 2:
				this->mCoordinator->AddComponent<Renderable>(iEntity, {
					"./Resources/Models/car/scene.gltf"
				});
				break;
			case 3:
				scale += 0.9f;
				this->mCoordinator->AddComponent<Renderable>(iEntity, {
					"./Resources/Models/car4/scene.gltf"
				});
				break;
			case 4:
				scale += 20.0f;
				this->mCoordinator->AddComponent<Renderable>(iEntity, {
					"./Resources/Models/car2/scene.gltf"
				});
				break;
			default: 
				abort();
			}

			this->mCoordinator->AddComponent<Transform>(iEntity, {
				vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
				vec3(radians(randRotation(generator)), radians(randRotation(generator)), radians(randRotation(generator))),
				vec3(scale, scale, scale),
			});

			this->mCoordinator->AddComponent<HitBox>(iEntity, {});
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
			signature.set(this->mCoordinator->GetComponentType<RigidBody>());
			signature.set(this->mCoordinator->GetComponentType<Transform>());
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

		const auto GuiSystem = this->mCoordinator->RegisterSystem<GUISystem>();
		{
			Signature signature;
			signature.set(this->mCoordinator->GetComponentType<Buttons>());
			this->mCoordinator->SetSystemSignature<GUISystem>(signature);
		}
		GuiSystem->Initialize(this->mCoordinator, this->mShader);
		this->mSystems.push_back(GuiSystem);
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

	void Game::UpdateFPS(const string& fps) const
	{
		Event event(WINDOW_GAME_FPS);
		event.SetParam(WINDOW_GAME_FPS_COUNT, fps);
		this->mCoordinator->SendEvent(event);
	}

	void Game::QuitHandler(Event& event)
	{
		this->mRunning = false;
	}

	void Game::FPSHandler(Event& event) const
	{
		const auto fps = event.GetParam<std::string>(WINDOW_GAME_FPS_COUNT);

		auto& labelsContainer = this->mCoordinator->GetComponent<Labels>(this->mUserInterface);

		labelsContainer.mLabels[0].mLabel = fps;
	}

	void Game::GoalHandler(Event& event) const
	{
		Sound gol = LoadSound("Resources/sounds/golazo.wav");
		auto& footballTransform = this->mCoordinator->GetComponent<Transform>(this->mFootball);
		footballTransform.mPosition = { 0.0f, 10.0f, -10.0f };

		auto& footballRigidBody = this->mCoordinator->GetComponent<RigidBody>(this->mFootball);
		footballRigidBody.mVelocity = { 0.0f, 0.0f, 0.0f };
		footballRigidBody.mAcceleration = { 0.0f, 0.0f, 0.0f };

		auto& player1Transform = this->mCoordinator->GetComponent<Transform>(this->mPlayers.front());
		player1Transform.mPosition = vec3(-10.0f, 10.0f, -10.0f);

		auto& player2Transform = this->mCoordinator->GetComponent<Transform>(this->mPlayers.back());
		player2Transform.mPosition = vec3(10.0f, 10.0f, -10.0f);

		auto& player1Playable = this->mCoordinator->GetComponent<Playable>(this->mPlayers.front());
		auto& player2Playable = this->mCoordinator->GetComponent<Playable>(this->mPlayers.back());

		const auto goalEntity = event.GetParam<Entity>(WINDOW_GAME_GOAL);

		if (goalEntity == this->mP1Goal)
		{
			player2Playable.mGoals++;
			PlaySound(gol);
		}
		else
		{
			player1Playable.mGoals++;
			PlaySound(gol);
		}

		auto& labelsContainer = this->mCoordinator->GetComponent<Labels>(this->mUserInterface);
		labelsContainer.mLabels[2].mLabel = "P1 Goals: " + std::to_string(player1Playable.mGoals);
		labelsContainer.mLabels[3].mLabel = "P2 Goals: " + std::to_string(player2Playable.mGoals);
	}

	void Game::StartGameHandler(Event& event)
	{
		if (this->mGameStarted)
		{
			return;
		}

		this->mGameStarted = true;

		for(const auto iEntity : this->mStartMenuEntities)
		{
			this->mCoordinator->DestroyEntity(iEntity);
		}
		this->mStartMenuEntities.clear();

		auto& labelsContainer = this->mCoordinator->GetComponent<Labels>(this->mUserInterface);
		auto& ButtonsContainer = this->mCoordinator->GetComponent<Buttons>(this->mUserInterface);

		labelsContainer.mLabels[1].mLabel = "";
		labelsContainer.mLabels[2].mLabel = "P1 Goals: 0";
		labelsContainer.mLabels[3].mLabel = "P2 Goals: 0";
		ButtonsContainer = Buttons({});

		const Entity player1 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(player1, {
			"./Resources/Models/car/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(player1, {
			vec3(-10.0f, 10.0f, -10.0f),
			vec3(0, radians(90.0f), 0),
			vec3(0.04f, 0.04f, 0.04f),
		});

		//Add config for constant forces
		this->mCoordinator->AddComponent<Gravity>(player1, {
			vec3(0.0f, -9.81f, 0.0f)
		});

		this->mCoordinator->AddComponent<Playable>(player1, {});

		this->mCoordinator->AddComponent<RigidBody>(player1, {});

		this->mCoordinator->AddComponent<HitBox>(player1, {});

		this->mPlayers.push_back(player1);

		const Entity player2 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(player2, {
			"./Resources/Models/car2/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(player2, {
			vec3(10.0f, 10.0f, -10.0f),
			vec3(0, radians(90.0f), 0),
			vec3(9.0f, 9.0f, 9.0f),
		});

		//Add config for constant forces
		this->mCoordinator->AddComponent<Gravity>(player2, {
			vec3(0.0f, -9.81f, 0.0f)
		});

		this->mCoordinator->AddComponent<Playable>(player2, { 1 });

		this->mCoordinator->AddComponent<RigidBody>(player2, {});

		this->mCoordinator->AddComponent<HitBox>(player2, {});

		this->mPlayers.push_back(player2);

		this->mP1Goal = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(this->mP1Goal, {
			"./Resources/Models/goal/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(this->mP1Goal, {
			vec3(-20.0f, -5.0f, -10.0f),
			vec3(0.0f, radians(90.0f), 0.0f),
			vec3(0.04f, 0.04f, 0.04f),
		});

		this->mCoordinator->AddComponent<RigidBody>(this->mP1Goal, {});

		this->mCoordinator->AddComponent<HitBox>(this->mP1Goal, { true, false, true });

		this->mP2Goal = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(this->mP2Goal, {
			"./Resources/Models/goal/scene.gltf"
			});

		this->mCoordinator->AddComponent<Transform>(this->mP2Goal, {
			vec3(20.0f, -5.0f, -10.0f),
			vec3(0.0f, radians(-90.0f), 0.0f),
			vec3(0.04f, 0.04f, 0.04f),
		});

		this->mCoordinator->AddComponent<RigidBody>(this->mP2Goal, {});

		this->mCoordinator->AddComponent<HitBox>(this->mP2Goal, { true, false, true });

		const Entity penguin = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(penguin, {
			"./Resources/Models/penguin/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(penguin, {
			vec3(5.0f, -5.0f, -30.0f),
			vec3(0.0f, radians(-35.0f), 0.0f),
			vec3(0.2f, 0.2f, 0.2f),
		});

		this->mCoordinator->AddComponent<RigidBody>(penguin, {});

		this->mCoordinator->AddComponent<HitBox>(penguin, {});

		const Entity cat = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(cat, {
			"./Resources/Models/cat/scene.gltf"
			});

		this->mCoordinator->AddComponent<Transform>(cat, {
			vec3(-5.0f, -5.0f, -30.0f),
			vec3(0.0f, radians(25.0f), 0.0f),
			vec3(1.3f, 1.3f, 1.3f),
			});

		this->mCoordinator->AddComponent<RigidBody>(cat, {});

		this->mCoordinator->AddComponent<HitBox>(cat, {});

		const Entity car3 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(car3, {
			"./Resources/Models/car3/scene.gltf"
			});

		this->mCoordinator->AddComponent<Transform>(car3, {
			vec3(-10.0f, -5.0f, -30.0f),
			vec3(0.0f, radians(195.0f), 0.0f),
			vec3(100.0f, 100.0f, 100.0f),
		});

		this->mCoordinator->AddComponent<RigidBody>(car3, {});

		this->mCoordinator->AddComponent<HitBox>(car3, {});

		const Entity car4 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(car4, {
			"./Resources/Models/car4/scene.gltf"
			});

		this->mCoordinator->AddComponent<Transform>(car4, {
			vec3(10.0f, -5.0f, -30.0f),
			vec3(0.0f, radians(-25.0f), 0.0f),
			vec3(1.0f, 1.0f, 1.0f),
			});

		this->mCoordinator->AddComponent<RigidBody>(car4, {});

		this->mCoordinator->AddComponent<HitBox>(car4, {});

		const Entity car5 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(car5, {
			"./Resources/Models/car5/scene.gltf"
			});

		this->mCoordinator->AddComponent<Transform>(car5, {
			vec3(0, -5.0f, -30.0f),
			vec3(radians(-90.0f), 0.0f, 0.0f),
			vec3(1.0f, 1.0f, 1.0f),
			});

		this->mCoordinator->AddComponent<RigidBody>(car5, {});

		this->mCoordinator->AddComponent<HitBox>(car5, {});


		const Entity table = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(table, {
			"./Resources/Models/table/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(table, {
			vec3(0.0f, -40.0f, -30.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(150.0f, 150.0f, 150.0f),
		});

		this->mCoordinator->AddComponent<RigidBody>(table, {});

		this->mCoordinator->AddComponent<HitBox>(table, { true });

		this->mFootball = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<Renderable>(this->mFootball, {
			"./Resources/Models/football/scene.gltf"
		});

		this->mCoordinator->AddComponent<Transform>(this->mFootball, {
			vec3(0.0f, 10.0f, -10.0f),
			vec3(0.0f, 0.0f, 0.0f),
			vec3(0.06f, 0.06f, 0.06f),
		});

		//Add config for constant forces
		this->mCoordinator->AddComponent<Gravity>(this->mFootball, {
			vec3(0.0f, -9.81f, 0.0f)
		});

		this->mCoordinator->AddComponent<RigidBody>(this->mFootball, {});

		this->mCoordinator->AddComponent<HitBox>(this->mFootball, { false, true, false});

		const Entity lightSource0 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<LightSource>(lightSource0, { vec4(1.5f, 1.5f, 1.5f, 2.0f) , vec3(20.0f, 10.0f, 10.0f) });

		const Entity lightSource1 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<LightSource>(lightSource1, { vec4(1.5f, 1.5f, 1.5f, 1.0f) , vec3(20.0f, 10.0f, -80.0f) });

		const Entity lightSource2 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<LightSource>(lightSource2, { vec4(1.0f, 1.0f, 1.0f, 1.0f) , vec3(-20.0f, 10.0f, -80.0f) });

		const Entity lightSource3 = this->mCoordinator->CreateEntity();

		this->mCoordinator->AddComponent<LightSource>(lightSource3, { vec4(1.0f, 1.0f, 1.0f, 1.0f) , vec3(-20.0f, 10.0f, 10.0f) });
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
