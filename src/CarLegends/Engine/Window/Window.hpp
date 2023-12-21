#ifndef WINDOW_WINDOW_HPP
#define WINDOW_WINDOW_HPP

#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>
#include <glm/vec2.hpp>

#include "../../Coordinators/Coordinator.hpp"
#include "../../Configurations/WindowConfiguration.hpp"
#include "Structs/ControllerState.hpp"
#include "Structs/MouseState.hpp"
#include "../../Configurations/ControllerConfiguration.hpp"

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
		static void EnableBlending();
		static void DisableBlending();
		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

		void Update() const;
		void ProcessEvents(const std::shared_ptr<Coordinator>& coordinator);
		void Shutdown() const;

		void ShowCursor() const;
		void HideCursor() const;
		void SetCursorPosition(vec2 position) const;
	private:
		GLFWwindow* mWindow;
		std::bitset<WINDOW_BUTTONS_COUNT> mButtons;
		ControllerState mControllerState;
		MouseState mMouseState;

		void ReadKeyBoardInput(const std::shared_ptr<Coordinator>& coordinator);
		void ReadControllerInput(const std::shared_ptr<Coordinator>& coordinator);
		void ReadMouseInput(const std::shared_ptr<Coordinator>& coordinator);
	};
}

#endif