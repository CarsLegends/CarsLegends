#ifndef GAME_HPP
#define GAME_HPP

#include <memory>

#include "Coordinators/Coordinator.hpp"
#include "Engine/Shaders/ShaderProgram.hpp"
#include "Engine/Window/Window.hpp"

namespace Game
{
	using namespace Coordinators;
	using namespace Windows;
	using namespace Shaders;

	class Game
	{
	public:

		Game();
		~Game();

		bool IsRunning() const;

		void RegisterListeners();
		void RegisterEntities();
		void RegisterComponents() const;
		void RegisterSystems();

		void Update(float deltaTime);
		void UpdateFPS(const std::string& fps) const;

	private:
		bool mRunning;
		Window mWindow;
		Entity mCamera;
		Entity mUserInterface;
		std::shared_ptr<Coordinator> mCoordinator;
		std::list<std::shared_ptr<ISystem>> mSystems{};
		std::shared_ptr<ShaderProgram> mShader;

		std::vector<Entity> mStartMenuEntities;
		std::vector<Entity> mGameEntities;
		bool mGameStarted;

		void QuitHandler(Event& event);
		void FPSHandler(Event& event) const;
		void StartGameHandler(Event& event);
		void ShowCursorHandler(Event& event) const;
		void HideCursorHandler(Event& event) const;
		void SetCursorPositionHandler(Event& event) const;
	};
}
#endif
