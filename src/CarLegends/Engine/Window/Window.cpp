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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

		// TODO: SEPARATE IN DIFFERENT PRIVATE METHODS
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

		if(this->mButtons.any())
		{
			Event event(WINDOW_INPUT);
			event.SetParam(WINDOW_INPUT_PARAMETER, this->mButtons);
			coordinator->SendEvent(event);
		}

		this->mButtons.reset();
	}

	void Window::Shutdown() const
	{
		glfwDestroyWindow(this->mWindow);
		glfwTerminate();
	}
}
