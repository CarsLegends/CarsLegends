#ifndef GAME_HPP
#define GAME_HPP
#include <memory>

#include "Coordinators/Coordinator.hpp"

namespace Game
{
	using namespace Coordinators;

	class Game
	{
	public:
		Game();

		bool IsRunning() const;

		void RegisterEntities();
		void RegisterComponents();
		void RegisterSystems() const;

		void Update();

	private:
		bool mRunning;
		std::shared_ptr<Coordinator> mCoordinator;
	};
}
#endif
