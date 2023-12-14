#include "GUISystem.hpp"

namespace Systems
{
	GUISystem::GUISystem() = default;

	void GUISystem::Initialize(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<ShaderProgram>& shader)
	{
		this->mCoordinator = coordinator;
		this->mShader = shader;

		this->mCoordinator->AddEventListener(WINDOW_CONTROLLER_INPUT, [this](auto&& PH1)
		{
			GUISystem::ControllerInputListener(std::forward<decltype(PH1)>(PH1));
		});
	}

	void GUISystem::Update(float deltaTime)
	{
		this->mShader->Activate();
		this->mShader->SendUniformInt("useColor", true);
		this->mShader->SendUniformInt("useProjection", true);
		this->mShader->SendUniformMatrix4f("projection", mProjection);

		for (const Entity iEntity : this->mEntities)
		{
			auto& buttonsContainer = this->mCoordinator->GetComponent<Buttons>(iEntity);
			UpdateButtonList(buttonsContainer);

			for(unsigned int i = 0; i < buttonsContainer.mRegisteredButtonsCount; i++)
			{
				const auto iButton = buttonsContainer.mButtons[i];
				CreateFont(iButton.mFontSize);
				RenderButtonBox(iButton);
			}

			const auto& labelsContainer = this->mCoordinator->GetComponent<Labels>(iEntity);
			for (unsigned int i = 0; i < labelsContainer.mRegisteredLabelsCount; i++)
			{
				const auto iLabel = labelsContainer.mLabels[i];

				CreateFont(iLabel.mFontSize);
				auto& font = mLoadedFonts[iLabel.mFontSize];

				vec2 labelPosition = iLabel.mPosition;
				if (iLabel.mScreenCentered)
				{
					const vec2 labelProportions = GetStringProportions(iLabel.mLabel, font);
					labelPosition.x -= labelProportions.x / 2;
				}

				RenderLabel(iLabel.mLabel, font, labelPosition);
			}
		}

		CheckHoveredButton();

		this->mShader->SendUniformInt("useColor", false);
		this->mShader->SendUniformInt("useProjection", false);
	}

	void GUISystem::InitializeGPUData(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		this->mVertexArray.Bind();
		const VertexBuffer vertexBuffer(vertices);
		const ElementBuffer elementBuffer(indices);

		this->mVertexArray.LinkAttributes(vertexBuffer);

		this->mVertexArray.Unbind();
		vertexBuffer.Unbind();
		elementBuffer.Unbind();
	}

	void GUISystem::RenderButtonBox(Button button)
	{
		const float xOffset = button.mWidth / 2.0f;
		const float yOffset = button.mHeight / 2.0f;

		vec3 color = button.mColor;

		if (button.mHovered)
		{
			color = { 0.91f, 0.88f, 0.83f };
		}

		const std::vector<Vertex> vertices = 
		{
			{ { button.mPosition.x - xOffset, button.mPosition.y + yOffset, 0.0f }, color },
			{ { button.mPosition.x + xOffset, button.mPosition.y + yOffset, 0.0f }, color },
			{ { button.mPosition.x + xOffset, button.mPosition.y - yOffset, 0.0f }, color },
			{ { button.mPosition.x - xOffset, button.mPosition.y - yOffset, 0.0f }, color },
		};

		InitializeGPUData(vertices, mIndices);

		this->mVertexArray.Bind();
		this->mVertexArray.DrawElements(mIndices);
		this->mVertexArray.Unbind();

		RenderButtonLabel(button, mLoadedFonts[button.mFontSize]);
	}

	void GUISystem::RenderButtonLabel(const Button& button, Font& font)
	{
		const vec2 labelProportions = GetStringProportions(button.mLabel, font);

		const vec2 gap = {
			(button.mWidth - labelProportions.x) * 0.5f,
			(button.mHeight - labelProportions.y) * 0.5f
		};

		const vec2 labelStartPosition = {
			button.mPosition.x - button.mWidth * 0.5f + gap.x,
			button.mPosition.y - button.mHeight * 0.5f + gap.y
		};

		RenderLabel(button.mLabel, font, labelStartPosition);
	}

	void GUISystem::RenderLabel(const std::string& label, Font& font, vec2 stringPosition)
	{
		Window::EnableBlending();
		this->mShader->SendUniformInt("useText", true);

		float offset = 0;
		for (auto itCharacter = label.cbegin(); itCharacter != label.cend(); ++itCharacter)
		{
			Character character = font.mCharacters[*itCharacter];

			vec2 characterPosition = {
				offset + stringPosition.x + character.mBearing.x * 0.9f,
				stringPosition.y - (character.mSize.y - character.mBearing.y) * 0.9f
			};

			const auto width = static_cast<float>(character.mSize.x) * 0.9f;
			const auto height = static_cast<float>(character.mSize.y) * 0.9f;

			character.mTexture.TextureUnit(*this->mShader, "text", 0);
			character.mTexture.Bind();

			const std::vector<Vertex> vertices = {
				{ { characterPosition.x        , characterPosition.y + height, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ { characterPosition.x        , characterPosition.y         , 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
				{ { characterPosition.x + width, characterPosition.y         , 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
				{ { characterPosition.x + width, characterPosition.y + height, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
			};

			InitializeGPUData(vertices, mIndices);

			this->mVertexArray.Bind();
			this->mVertexArray.DrawElements(mIndices);
			this->mVertexArray.Unbind();

			offset += (character.mAdvance >> 6) * 0.9f;
		}

		this->mShader->SendUniformInt("useText", false);
		Window::DisableBlending();
	}

	vec2 GUISystem::GetStringProportions(const std::string& text, Font& font)
	{
		int width = 0;
		int height = 0;

		for (auto itCharacter = text.begin(); itCharacter != text.end(); ++itCharacter)
		{
			const Character character = font.mCharacters[*itCharacter];

			width += static_cast<int>(character.mSize.x * 0.9f) + character.mBearing.x;

			if (character.mSize.y > height)
			{
				height = character.mSize.y * 0.9f;
			}
		}

		return { width, height };
	}

	void GUISystem::CreateFont(int fontSize)
	{
		if (this->mLoadedFonts.find(fontSize) == this->mLoadedFonts.end())
		{
			mLoadedFonts[fontSize] = Font(fontSize);
		}
	}

	void GUISystem::UpdateButtonList(Buttons& buttons)
	{
		const auto startTimeFrame = std::chrono::high_resolution_clock::now();
		const float timeDifference = std::chrono::duration<float>(startTimeFrame - this->mPreviousTime).count();

		if(timeDifference < 0.07f)
		{
			return;
		}

		this->mPreviousTime = startTimeFrame;

		std::pair hoveredButton = std::make_pair(0, buttons.mButtons.begin());

		int counter = 0;
		for(auto itButton = buttons.mButtons.begin(); itButton != buttons.mButtons.end(); ++itButton)
		{
			if (counter >= buttons.mRegisteredButtonsCount)
			{
				break;
			}

			const auto button = *itButton;
			if(button.mHovered)
			{
				hoveredButton = std::make_pair(counter, itButton);
			}
			counter++;
		}

		this->mHoveredButton = hoveredButton.second;

		if (this->mControllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButton::Up)))
		{
			this->mHoveredButton->mHovered = false;

			if (hoveredButton.first == 0)
			{
				buttons.mButtons[buttons.mRegisteredButtonsCount - 1].mHovered = true;
			}
			else
			{
				buttons.mButtons[hoveredButton.first - 1].mHovered = true;
			}

			this->mControllerState.mControllerButtons.reset();
			return;
		}

		if (this->mControllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButton::Down)))
		{
			this->mHoveredButton->mHovered = false;

			if (hoveredButton.first == buttons.mRegisteredButtonsCount - 1)
			{
				buttons.mButtons[0].mHovered = true;
			}
			else
			{
				buttons.mButtons[hoveredButton.first + 1].mHovered = true;
			}
		}

		this->mControllerState.mControllerButtons.reset();
	}

	void GUISystem::CheckHoveredButton()
	{
		const auto startTimeFrame = std::chrono::high_resolution_clock::now();
		const float timeDifference = std::chrono::duration<float>(startTimeFrame - this->mPreviousTime).count();

		if (timeDifference < 0.07f)
		{
			return;
		}

		this->mPreviousTime = startTimeFrame;

		if (this->mControllerState.mControllerButtons.test(static_cast<std::size_t>(ControllerButton::A)))
		{
			this->mCoordinator->SendEvent(this->mHoveredButton->mEvent);
		}
	}

	void GUISystem::ControllerInputListener(Event& event)
	{
		const auto [controllerState, playerNumber] = event.GetParam<std::pair<ControllerState, int>>(WINDOW_CONTROLLER_INPUT_PARAMETER);

		if(playerNumber == 0)
		{
			this->mControllerState = controllerState;
		}
	}
}
