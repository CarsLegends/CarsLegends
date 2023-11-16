#ifndef WINDOW_WINDOW_HPP
#define WINDOW_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>
#include <glm/vec2.hpp>

#include "../../Coordinators/Coordinator.hpp"
#include "../../Configurations/WindowConfiguration.hpp"
#include "Structs/MouseState.hpp"

namespace Windows
{
	using namespace Coordinators;
	using namespace Configuration;
	using namespace glm;

	class Window
	{
	public:
		Window();
		Window(std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight);

		void Update() const;
		void ProcessEvents(std::shared_ptr<Coordinator> coordinator);
		void Shutdown() const;

		void ShowCursor() const;
		void HideCursor() const;
		void SetCursorPosition(vec2 position) const;
	private:
		GLFWwindow* mWindow;
		std::bitset<WINDOW_BUTTONS_COUNT> mButtons;
		MouseState mMouseState;

		void ReadKeyBoardInput(std::shared_ptr<Coordinator> coordinator);
		void ReadMouseInput(std::shared_ptr<Coordinator> coordinator);
	};
}

#endif