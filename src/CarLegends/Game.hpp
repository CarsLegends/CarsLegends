#ifndef GAME_HPP
#define GAME_HPP

#include "Coordinators/Coordinator.hpp"

namespace Game {
	using namespace Coordinators;

	class Game {
	public:
		bool IsRunning() const;

		void RegisterEntities();
		void RegisterComponents();
		void RegisterSystems();

		void Update();
	private:
		bool mRunning = true;
	};
}
#endif