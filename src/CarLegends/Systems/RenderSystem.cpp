#include "RenderSystem.hpp"

#include "../Components/Camera.hpp"

namespace Systems {
	extern Coordinator gCoordinator;

	RenderSystem::RenderSystem() {
		this->mShader = make_unique<ShaderProgram>("../Resources/Shaders/test.vert", "../Resources/Shaders/test.frag");

		// TODO: Add logic to create another camera in case of an event
		const Entity mainCamera = gCoordinator.CreateEntity();
		this->mCameras.push_back(mainCamera);
		gCoordinator.AddComponent(mainCamera, Camera(vec3(0.0f, 0.0f, 0.0f)));

	}

	void RenderSystem::Update() {
		mShader->Activate();
		for (auto const& iEntity : this->mEntities) {
			auto& renderable = gCoordinator.GetComponent<Renderable>(iEntity);

			if (!renderable.mLoaded) {
				continue;
			}

			for(auto& mesh : renderable.mMeshes) {

				unsigned int indexDiffuse = 0;
				unsigned int indexSpecular = 0;

				for (unsigned int i = 0; i < mesh.mTextures.size(); i++)
				{
					string number;
					string type = mesh.mTextures[i].m_Type;

					if (type == "diffuse")
					{
						number = std::to_string(indexDiffuse++);
					}
					else if (type == "specular")
					{
						number = std::to_string(indexSpecular++);
					}

					mesh.mTextures[i].TextureUnit(*this->mShader, (type + number).c_str(), i);
					mesh.mTextures[i].Bind();
				}

				mesh.mVertexArray.Bind();
				mesh.mVertexArray.DrawElements(mesh.mIndices);
				mesh.mVertexArray.Unbind();
			}
		}
	}
}
