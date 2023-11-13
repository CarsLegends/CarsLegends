#ifndef SYSTEMS_RENDER_SYSTEM_HPP
#define SYSTEMS_RENDER_SYSTEM_HPP

#include <memory>

#include "ISystem.hpp"
#include "../Components/Rendable.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "Shaders/ShaderProgram.hpp"
#include "Buffers/ElementBuffer.hpp"

namespace Systems {
	using namespace std;
	using namespace Shaders;
	using namespace Components;
	using namespace Coordinators;

	class RenderSystem : public ISystem {
	public:
		RenderSystem();
		void Update() override;

	private:
		shared_ptr<ShaderProgram> mShader;
		vector<Entity> mCameras;

		static void SendVertex(Renderable::Mesh& mesh);
	};
}

#endif
