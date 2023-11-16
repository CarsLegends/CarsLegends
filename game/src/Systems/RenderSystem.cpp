#include "RenderSystem.hpp"

#include <utility>

#include "../Components/Camera.hpp"
#include "../Components/Transform.hpp"

namespace Systems
{
	RenderSystem::RenderSystem() = default;

	void RenderSystem::Initialize(std::shared_ptr<Coordinator> coordinator)
	{
		// TODO: Make shader read files from Resources/Shaders
		this->mShader = std::make_unique<ShaderProgram>("Resources/Shaders/test.vert", "Resources/Shaders/test.frag");
		this->mCoordinator = std::move(coordinator);

		// TODO: Add logic to create another camera in case of an event
		const Entity mainCamera = this->mCoordinator->CreateEntity();
		this->mCameras.push_back(mainCamera);
		this->mCoordinator->AddComponent<Camera>(mainCamera, { vec3(0.0f, 0.0f, 4.0f) });
	}

	void RenderSystem::Update(float deltaTime)
	{
		mShader->Activate();
		for (const auto& iEntity : this->mEntities)
		{
			MoveCamera();
			MoveEntity(iEntity, deltaTime);

			auto& renderable = this->mCoordinator->GetComponent<Renderable>(iEntity);
			if (!renderable.mLoaded)
			{
				return;
			}

			for (auto& mesh : renderable.mMeshes)
			{
				unsigned int indexDiffuse = 0;
				unsigned int indexSpecular = 0;

				for (unsigned int i = 0; i < mesh.mTextures.size(); i++)
				{
					std::string number;
					std::string type = mesh.mTextures[i].mType;

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

	void RenderSystem::MoveEntity(Entity entity, float deltaTime) const
	{
		auto& transform = this->mCoordinator->GetComponent<Transform>(entity);

		mat4 matrix = mat4(1.0f);
		matrix = translate(matrix, transform.mPosition);
		matrix = scale(matrix, transform.mScale);
		transform.mCurrentTransformation = rotate(transform.mCurrentTransformation, radians(transform.mRotationAngle * deltaTime), transform.mRotationAxis);
		matrix *= transform.mCurrentTransformation;

		this->mShader->SendUniformMatrix4f("model", matrix);
	}

	void RenderSystem::MoveCamera() const
	{
		const auto cameraEntity = mCameras.front();
		auto& camera = this->mCoordinator->GetComponent<Camera>(cameraEntity);
		this->mShader->SendUniformMatrix4f("cameraMatrix", camera.mMatrix);
	}
}
