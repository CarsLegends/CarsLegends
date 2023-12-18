#ifndef SYSTEMS_GUI_HPP
#define SYSTEMS_GUI_HPP

#include <chrono>

#include "ISystem.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Components/Buttons.hpp"
#include "../Components/Labels.hpp"
#include "../Engine/Window/Structs/MouseState.hpp"
#include "../Engine/Buffers/VertexArray.hpp"
#include "../Events/EventTypes.hpp"
#include "../Events/EventParameters.hpp"
#include "../Engine/Shaders/ShaderProgram.hpp"
#include "../Engine/Fonts/Font.h"
#include "../Engine/Fonts/Structs/Character.h"
#include "../Engine/Window/Window.hpp"
#include "../Engine/Buffers/ElementBuffer.hpp"
#include "../Engine/Fonts/Font.h"
#include "../Configurations/ControllerConfiguration.hpp"

namespace Systems
{
	using namespace Coordinators;
	using namespace Components;
	using namespace Shaders;
	using namespace Buffers;
	using namespace Windows;
	using namespace Fonts;

	class GUISystem final : public ISystem
	{
	public:
		GUISystem();
		void Initialize(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<ShaderProgram>& shader);
		void Update(float deltaTime) override;
	private:
		std::shared_ptr<Coordinator> mCoordinator;
		std::shared_ptr<ShaderProgram> mShader;
		std::map<int, Font> mLoadedFonts;
		VertexArray mVertexArray{};
		VertexBuffer mVertexBuffer{};
		ControllerState mControllerState;

		std::chrono::time_point<std::chrono::steady_clock> mPreviousTime = std::chrono::high_resolution_clock::now();
		std::_Array_iterator<Button, MAX_BUTTONS> mHoveredButton;

		inline static std::vector<uint32_t> mIndices = { 0, 1, 2, 0, 2, 3 };
		inline static mat4 mProjection = ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));

		static vec2 GetStringProportions(const std::string& text, Font& font);
		void CreateFont(int fontSize);
		void UpdateButtonList(Buttons& buttons);
		void CheckHoveredButton() const;
		void ControllerInputListener(Event& event);
		void InitializeGPUData(const std::vector<Vertex>& vertices, std::vector<uint32_t> indices);
		void RenderButtonBox(const Button& button);
		void RenderButtonLabel(const Button& button, Font& font);
		void RenderLabel(const ::std::string& label, ::Fonts::Font& font, vec2 stringPosition, vec3 color);
	};
}

#endif