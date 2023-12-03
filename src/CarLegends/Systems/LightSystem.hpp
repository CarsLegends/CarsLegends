#ifndef SYSTEMS_LIGHT_HPP
#define SYSTEMS_LIGHT_HPP

#include <memory>

#include "ISystem.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Configurations/ManagersConfiguration.hpp"
#include "../Engine/Shaders/ShaderProgram.hpp"
#include "../Components/LightSource.h"

namespace Systems
{
	using namespace Coordinators;
	using namespace Shaders;
	using namespace Configuration;
	using namespace Components;

	class LightSystem final : public ISystem
	{
	public:
		LightSystem();
		void Initialize(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<ShaderProgram>& shader);
		void Update(float deltaTime) override;
	private:
		std::shared_ptr<Coordinator> mCoordinator;
		std::shared_ptr<ShaderProgram> mShader;
	};
}

#endif
