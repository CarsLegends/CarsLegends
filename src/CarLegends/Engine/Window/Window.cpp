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
		glfwWindowHint(GLFW_SAMPLES, WINDOW_SAMPLES_COUNT);

#ifdef NDEBUG
		const auto window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
#else
		const auto window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
#endif

		if (window == nullptr)
		{
			std::cout << "Failed to create a GLFW Window" << "\n";
			glfwTerminate();
			abort();
		}
		this->mWindow = window;

		glfwMakeContextCurrent(this->mWindow);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		glViewport(0, 0, windowWidth, windowHeight);

		glfwSetFramebufferSizeCallback(this->mWindow, FrameBufferSizeCallback);

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Window::Update() const
	{
		glfwSwapBuffers(mWindow);
	}

	void Window::ProcessEvents(const std::shared_ptr<Coordinator>& coordinator)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		ReadKeyBoardInput(coordinator);
		ReadMouseInput(coordinator);
		ReadControllerInput(coordinator);
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

	void Window::ReadKeyBoardInput(const std::shared_ptr<Coordinator>& coordinator)
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
			Event event(WINDOW_KEYBOARD_INPUT_BUTTON);
			event.SetParam(WINDOW_KEYBOARD_INPUT_BUTTON_PARAMETER, this->mButtons);
			coordinator->SendEvent(event);
		}
		this->mButtons.reset();

		if (glfwGetKey(this->mWindow, GLFW_KEY_H) == GLFW_PRESS)
		{
			coordinator->SendEvent(WINDOW_SHOW_HITBOXES);
		}

		if (glfwGetKey(this->mWindow, GLFW_KEY_G) == GLFW_PRESS)
		{
			coordinator->SendEvent(WINDOW_DONT_SHOW_HITBOXES);
		}

	}

	void Window::ReadControllerInput(const std::shared_ptr<Coordinator>& coordinator)
	{
		for (int iPlayer = 0; iPlayer < WINDOW_PLAYER_COUNT; ++iPlayer)
		{
			int joystickNumber;

			switch (iPlayer)
			{
			case 0:
				joystickNumber = GLFW_JOYSTICK_1;
				break;
			case 1:
				joystickNumber = GLFW_JOYSTICK_2;
				break;
			default:
				std::cout << "Unsupported number of players." << std::endl;
				abort();
			}

			if (!glfwJoystickPresent(joystickNumber) && !glfwJoystickIsGamepad(joystickNumber))
			{
				//std::cout << "Player number " << joystickNumber + 1 << " is not connected" << std::endl;
				continue;
			}

			int controllerAxesCount;
			const float* axes = glfwGetJoystickAxes(joystickNumber, &controllerAxesCount);

			this->mControllerState.mLeftJoystickX = axes[static_cast<size_t>(Axes::LeftX)];
			this->mControllerState.mLeftJoystickY = axes[static_cast<size_t>(Axes::LeftY)];
			this->mControllerState.mRightJoystickX = axes[static_cast<size_t>(Axes::RightX)];
			this->mControllerState.mRightJoystickY = axes[static_cast<size_t>(Axes::RightY)];
			this->mControllerState.mLeftTrigger = axes[static_cast<size_t>(Axes::LeftTrigger)];
			this->mControllerState.mRightTrigger = axes[static_cast<size_t>(Axes::RightTrigger)];

			int controllerButtonsCount;
			const unsigned char* buttons = glfwGetJoystickButtons(joystickNumber, &controllerButtonsCount);

			constexpr auto buttonA = static_cast<size_t>(ControllerButtons::A);
			if (buttons[buttonA] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonA);
			}

			constexpr auto buttonB = static_cast<size_t>(ControllerButtons::B);
			if (buttons[buttonB] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonB);
			}

			constexpr auto buttonY = static_cast<size_t>(ControllerButtons::Y);
			if (buttons[buttonY] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonY);
			}

			constexpr auto buttonX = static_cast<size_t>(ControllerButtons::X);
			if (buttons[buttonX] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonX);
			}

			constexpr auto buttonRB = static_cast<size_t>(ControllerButtons::RB);
			if (buttons[buttonRB] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonRB);
			}

			constexpr auto buttonLB = static_cast<size_t>(ControllerButtons::LB);
			if (buttons[buttonLB] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonLB);
			}

			constexpr auto buttonBack = static_cast<size_t>(ControllerButtons::Back);
			if (buttons[buttonBack] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonBack);
			}

			constexpr auto buttonRestart = static_cast<size_t>(ControllerButtons::Start);
			if (buttons[buttonRestart] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonRestart);
			}

			constexpr auto buttonL3 = static_cast<size_t>(ControllerButtons::L3);
			if (buttons[buttonL3] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonL3);
			}

			constexpr auto buttonR3 = static_cast<size_t>(ControllerButtons::R3);
			if (buttons[buttonR3] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonR3);
			}

			constexpr auto buttonUp = static_cast<size_t>(ControllerButtons::Up);
			if (buttons[buttonUp] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonUp);
			}

			constexpr auto buttonRight = static_cast<size_t>(ControllerButtons::Right);
			if (buttons[buttonRight] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonRight);
			}

			constexpr auto buttonDown = static_cast<size_t>(ControllerButtons::Down);
			if (buttons[buttonDown] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonDown);
			}

			constexpr auto buttonLeft = static_cast<size_t>(ControllerButtons::Left);
			if (buttons[buttonLeft] == GLFW_PRESS)
			{
				this->mControllerState.mControllerButtons.set(buttonLeft);
			}

			Event event(WINDOW_CONTROLLER_INPUT);
			event.SetParam(WINDOW_CONTROLLER_INPUT_PARAMETER, std::pair(this->mControllerState, iPlayer));
			coordinator->SendEvent(event);

			this->mControllerState.mControllerButtons.reset();
		}
	}

	void Window::ReadMouseInput(const std::shared_ptr<Coordinator>& coordinator)
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
