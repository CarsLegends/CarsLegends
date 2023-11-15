#ifndef GAME_HPP
#define GAME_HPP
#include <memory>

#include "Coordinators/Coordinator.hpp"
#include "Engine/Window/Window.hpp"

namespace Game
{
	using namespace Coordinators;
	using namespace Windows;

	class Game
	{
	public:

		Game();
		~Game();

		bool IsRunning() const;

		void RegisterListeners();
		void RegisterEntities() const;
		void RegisterComponents() const;
		void RegisterSystems();

		void Update(float deltaTime);

	private:
		bool mRunning;
		Window mWindow;
		std::shared_ptr<Coordinator> mCoordinator;
		std::list<std::shared_ptr<ISystem>> mSystems{};

		void QuitHandler(Event& event);
	};
}
#endif
