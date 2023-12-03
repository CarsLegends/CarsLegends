#include "LightSystem.hpp"


namespace Systems
{
	LightSystem::LightSystem() = default;

	void LightSystem::Initialize(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<ShaderProgram>& shader)
	{
		this->mCoordinator = coordinator;
		this->mShader = shader;
	}

	void LightSystem::Update(float deltaTime)
	{
		assert(this->mEntities.size() != MAX_LIGHT_SOURCE && "Limit of amount of light sources passed");

		this->mShader->SendUniformInt("amountLights", this->mEntities.size());
		uint32_t lightSourceIndex = 0;
		for (const Entity iEntity : mEntities)
		{
			auto& lightSource = this->mCoordinator->GetComponent<LightSource>(iEntity);
			this->mShader->SendUniformVector4f(("lightColor" + std::to_string(lightSourceIndex)).c_str(), lightSource.mLightColor);
			this->mShader->SendUniformVector3f(("lightPosition" + std::to_string(lightSourceIndex)).c_str(), lightSource.mLightPosition);
			lightSourceIndex++;
		}
	}
}
