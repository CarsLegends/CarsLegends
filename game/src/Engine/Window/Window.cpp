#include <iostream>

#include "Window.hpp"
#include "../../Events/EventTypes.hpp"
#include "../../Events/EventParameters.hpp"
#include "../../Events/Event.hpp"

namespace Windows
{
	Window::Window() = default;

	Window::Window(std::string const& windowTitle, unsigned windowWidth, unsigned windowHeight)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		//auto window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
		auto window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create a GLFW Window" << "\n";
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		this->mWindow = window;

		glfwMakeContextCurrent(this->mWindow);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		glViewport(0, 0, windowWidth, windowHeight);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glEnable(GL_DEPTH_TEST);
	}

	void Window::Update() const
	{
		glfwSwapBuffers(mWindow);
	}

	void Window::ProcessEvents(std::shared_ptr<Coordinator> coordinator)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		ReadKeyBoardInput(coordinator);
		ReadMouseInput(coordinator);
	}

	void Window::Shutdown() const
	{
		glfwDestroyWindow(this->mWindow);
		glfwTerminate();
	}

	void Window::ShowCursor() const
	{
		glfwSetInputMode(this->mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::HideCursor() const
	{
		glfwSetInputMode(this->mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::SetCursorPosition(vec2 position) const
	{
		glfwSetCursorPos(this->mWindow, position.x, position.y);
	}

	void Window::ReadKeyBoardInput(std::shared_ptr<Coordinator> coordinator)
	{
		if (glfwGetKey(this->mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(this->mWindow))
		{
			coordinator->SendEvent(WINDOW_QUIT);
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			this->mButtons.set(static_cast<std::size_t>(Buttons::W));
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			this->mButtons.set(static_cast<std::size_t>(Buttons::A));
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			this->mButtons.set(static_cast<std::size_t>(Buttons::S));
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			this->mButtons.set(static_cast<std::size_t>(Buttons::D));
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			this->mButtons.set(static_cast<std::size_t>(Buttons::Space));
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			this->mButtons.set(static_cast<std::size_t>(Buttons::LeftShift));
		}

		if (this->mButtons.any())
		{
			Event event(WINDOW_INPUT_BUTTON);
			event.SetParam(WINDOW_INPUT_BUTTON_PARAMETER, this->mButtons);
			coordinator->SendEvent(event);
		}

		this->mButtons.reset();
	}

	void Window::ReadMouseInput(std::shared_ptr<Coordinator> coordinator)
	{
		if (glfwGetMouseButton(this->mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			double mouseX;
			double mouseY;
			glfwGetCursorPos(this->mWindow, &mouseX, &mouseY);

			this->mMouseState.mPosition.x = static_cast<float>(mouseX);
			this->mMouseState.mPosition.y = static_cast<float>(mouseY);
			this->mMouseState.mRightClickReleased = false;

			Event event(WINDOW_INPUT_CURSOR);
			event.SetParam(WINDOW_INPUT_CURSOR_PARAMETER, this->mMouseState);
			coordinator->SendEvent(event);
		}
		else if (glfwGetMouseButton(this->mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
		{
			this->mMouseState.mPosition.x = static_cast<float>(WINDOW_WIDTH/2);
			this->mMouseState.mPosition.y = static_cast<float>(WINDOW_HEIGHT/2);
			this->mMouseState.mRightClickReleased = true;

			Event event(WINDOW_INPUT_CURSOR);
			event.SetParam(WINDOW_INPUT_CURSOR_PARAMETER, this->mMouseState);
			coordinator->SendEvent(event);
		}
	}
}
