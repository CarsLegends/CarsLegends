#ifndef WINDOW_WINDOW_HPP
#define WINDOW_WINDOW_HPP

#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>

#include "../../Coordinators/Coordinator.hpp"

namespace Windows
{
	using namespace Coordinators;

	enum class Buttons
	{
		W,
		A,
		S,
		D,
	};

	constexpr uint32_t TOTAL_BUTTONS = 8;

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
		std::bitset<TOTAL_BUTTONS> mButtons;
	};
}

#endif