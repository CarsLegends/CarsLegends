#ifndef SYSTEMS_MODEL_LOADING_HPP
#define SYSTEMS_MODEL_LOADING_HPP

#include <vector>

#include "ISystem.hpp"
#include "../Engine/Textures/Texture.hpp"
#include "../Engine/Buffers/ElementBuffer.hpp"
#include "../Engine/ModelLoading/AssimpModelLoader.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Components/Renderable.hpp"

namespace Systems
{
	using namespace Textures;
	using namespace ModelLoaders;
	using namespace Coordinators;
	using namespace Components;
	using namespace Buffers;

	class ModelLoadingSystem final : public ISystem
	{
	public:
		ModelLoadingSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator);
		void Update(float deltaTime) override;

	private:
		vector<Texture> mTexturesLoaded;
		AssimpModelLoader mModelLoader{};
		std::shared_ptr<Coordinator> mCoordinator;
		std::unordered_map<std::string, vector<Renderable::Mesh>> mModelsLoaded;

		static void SendVertex(Renderable::Mesh& mesh);
	};
}

#endif