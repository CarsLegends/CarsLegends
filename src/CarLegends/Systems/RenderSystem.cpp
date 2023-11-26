#include "RenderSystem.hpp"

#include <utility>
#include <glm/glm.hpp>

#include "../Components/Camera.hpp"
#include "../Components/CollisionModel.hpp"
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
		this->mCoordinator->AddComponent<Camera>(mainCamera, { vec3(0.0f, 10.0f, 50.0f) });
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
		auto& collisionModel = this->mCoordinator->GetComponent<CollisionModel>(entity);

		transform.mCurrentTransformation = mat4(1.0f);
		transform.mCurrentTransformation = translate(transform.mCurrentTransformation, transform.mPosition);
		transform.mCurrentTransformation = scale(transform.mCurrentTransformation, transform.mScale);
		transform.mCurrentTransformation = rotate(transform.mCurrentTransformation, radians(transform.mRotationAngle * deltaTime), transform.mRotationAxis);

		for (auto& collisionMesh : collisionModel.mCollider)
		{
			for (auto& vertex : collisionMesh)
			{
				vertex = vec3(transform.mCurrentTransformation * vec4(vertex, 1));
			}
		}

		this->mShader->SendUniformMatrix4f("model", transform.mCurrentTransformation);
	}

	void RenderSystem::MoveCamera() const
	{
		const auto cameraEntity = mCameras.front();
		auto& camera = this->mCoordinator->GetComponent<Camera>(cameraEntity);
		this->mShader->SendUniformMatrix4f("cameraMatrix", camera.mMatrix);
	}
}
