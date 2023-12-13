#include "HitBoxRenderSystem.hpp"

#include <utility>

#include "../Components/HitBox.hpp"
#include "../Engine/Shaders/ShaderProgram.hpp"

namespace Systems
{
	HitBoxRenderSystem::HitBoxRenderSystem() = default;

	void HitBoxRenderSystem::Initialize(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<ShaderProgram>& shader)
	{
		this->mCoordinator = coordinator;

		this->mCoordinator->AddEventListener(WINDOW_SHOW_HITBOXES, [this](auto&& PH1)
		{
			HitBoxRenderSystem::ShowHitBoxHandler(std::forward<decltype(PH1)>(PH1));
		});

		this->mCoordinator->AddEventListener(WINDOW_DONT_SHOW_HITBOXES, [this](auto&& PH1)
		{
			HitBoxRenderSystem::DontShowHitBoxHandler(std::forward<decltype(PH1)>(PH1));
		});

		this->mShader = shader;
		this->mShowHitBoxes = false;
	}

	void HitBoxRenderSystem::Update(float deltaTime)
	{
		if(!mShowHitBoxes)
		{
			return;
		}

		this->mShader->Activate();
		InitializeDataInGPU();

		this->mShader->SendUniformInt("useColor", true);
		for (const Entity iEntity : this->mEntities)
		{
			MoveEntity(iEntity);

			this->mVertexArray.Bind();
			this->mVertexArray.DrawHitBoxesOutline();
			this->mVertexArray.Unbind();
		}
		this->mShader->SendUniformInt("useColor", false);
	}

	void HitBoxRenderSystem::MoveEntity(Entity entity) const
	{
		auto& hitBox = this->mCoordinator->GetComponent<HitBox>(entity);
		this->mShader->SendUniformMatrix4f("model", hitBox.mTransformations);
	}

	void HitBoxRenderSystem::InitializeDataInGPU()
	{
		this->mVertexArray.Bind();
		const VertexBuffer vertexBuffer(mSimplestHitBoxVertices);
		const ElementBuffer elementBuffer(mSimplestHitBoxElements);

		this->mVertexArray.LinkAttributes(vertexBuffer);

		this->mVertexArray.Unbind();
		vertexBuffer.Unbind();
		elementBuffer.Unbind();
	}

	void HitBoxRenderSystem::ShowHitBoxHandler(Event& event)
	{
		this->mShowHitBoxes = true;
	}

	void HitBoxRenderSystem::DontShowHitBoxHandler(Event& event)
	{
		this->mShowHitBoxes = false;
	}
}
