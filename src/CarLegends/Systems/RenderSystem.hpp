#ifndef SYSTEMS_RENDER_SYSTEM_HPP
#define SYSTEMS_RENDER_SYSTEM_HPP

#include <memory>

#include "ISystem.hpp"
#include "../Components/Renderable.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Engine/Shaders/ShaderProgram.hpp"
#include "../Engine/Buffers/ElementBuffer.hpp"

namespace Systems
{
	using namespace Shaders;
	using namespace Coordinators;
	using namespace Components;

	class RenderSystem final : public ISystem
	{
	public:
		RenderSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator, std::shared_ptr<ShaderProgram> Shader, const Entity camera);
		void Update(float deltaTime) override;

	private:
		std::shared_ptr<ShaderProgram> mShader;
		std::shared_ptr<Coordinator> mCoordinator;
		Entity mCamera;

		void MoveEntity(Entity entity) const;
		void MoveCamera() const;
	};
}

#endif
