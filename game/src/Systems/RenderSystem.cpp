#include "RenderSystem.hpp"

#include "../Components/Camera.hpp"

namespace Systems
{
	RenderSystem::RenderSystem() = default;

	void RenderSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		this->mShader = std::make_unique<ShaderProgram>("../Resources/Shaders/test.vert",
			"../Resources/Shaders/test.frag");

		// TODO: Add logic to create another camera in case of an event
		const Entity mainCamera = this->mCoordinator->CreateEntity();
		this->mCameras.push_back(mainCamera);
		this->mCoordinator->AddComponent(mainCamera, Camera(vec3(0.0f, 0.0f, 0.0f)));
	}

	void RenderSystem::Update()
	{
		mShader->Activate();
		for (const auto& iEntity : this->mEntities)
		{
			auto& renderable = this->mCoordinator->GetComponent<Renderable>(iEntity);

			if (!renderable.mLoaded)
			{
				continue;
			}

			for (auto& mesh : renderable.mMeshes)
			{
				unsigned int indexDiffuse = 0;
				unsigned int indexSpecular = 0;

				for (unsigned int i = 0; i < mesh.mTextures.size(); i++)
				{
					std::string number;
					std::string type = mesh.mTextures[i].m_Type;

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
