#ifndef SYSTEMS_RENDER_SYSTEM_HPP
#define SYSTEMS_RENDER_SYSTEM_HPP

#include <memory>

#include "ISystem.hpp"
#include "../Components/Rendable.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Engine/Shaders/ShaderProgram.hpp"
#include "../Engine/Buffers/ElementBuffer.hpp"

namespace Systems
{
	using namespace Shaders;
	using namespace Coordinators;
	using namespace Components;

	class RenderSystem : public ISystem
	{
	public:
		RenderSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update() override;

	private:
		std::shared_ptr<ShaderProgram> mShader;
		std::vector<Entity> mCameras;
		std::shared_ptr<Coordinator> mCoordinator;
	};
}

#endif
