#ifndef WINDOW_WINDOW_HPP
#define WINDOW_WINDOW_HPP

#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>

#include "../../Coordinators/Coordinator.hpp"
#include "../../Configurations/WindowConfiguration.hpp"

namespace Windows
{
	using namespace Coordinators;
	using namespace Configuration;

	class Window
	{
	public:
		Window();
		Window(std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight);

		void Update() const;
		void ProcessEvents(std::shared_ptr<Coordinator> coordinator);
		void Shutdown() const;

	private:
		GLFWwindow* mWindow;
		std::bitset<WINDOW_BUTTONS_COUNT> mButtons;
	};
}

#endif