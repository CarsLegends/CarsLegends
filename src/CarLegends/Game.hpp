#ifndef GAME_HPP
#define GAME_HPP
#include <chrono>
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

	private:
		bool mRunning;
		Window mWindow;
		Entity mCamera;
		std::shared_ptr<Coordinator> mCoordinator;
		std::list<std::shared_ptr<ISystem>> mSystems{};
		std::shared_ptr<ShaderProgram> mShader;
		std::chrono::time_point<std::chrono::steady_clock> previousTime = std::chrono::high_resolution_clock::now();

		void QuitHandler(Event& event);
		void ShowCursorHandler(Event& event) const;
		void HideCursorHandler(Event& event) const;
		void SetCursorPositionHandler(Event& event) const;
	};
}
#endif
