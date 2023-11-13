#ifndef SYSTEMS_MODEL_LOADING_HPP
#define SYSTEMS_MODEL_LOADING_HPP

#include <vector>

#include "ISystem.hpp"
#include "Textures/Texture.hpp"
#include "Buffers/ElementBuffer.hpp"
#include "ModelLoading/AssimpModelLoader.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Components/Rendable.hpp"

namespace Systems {
	using namespace std;
	using namespace Textures;
	using namespace Coordinators;
	using namespace ModelLoaders;
	using namespace Components;
	using namespace Buffers;

	class ModelLoadingSystem : public ISystem {
	public:
		ModelLoadingSystem();

		void Update() override;

	private:
		vector<Texture> mTexturesLoaded;
		AssimpModelLoader mModelLoader{};

		void SendVertex(Renderable::Mesh& mesh);
	};
}

#endif