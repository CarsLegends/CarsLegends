#include "RenderSystem.hpp"

#include <utility>
#include <glm/glm.hpp>

#include "../Components/Camera.hpp"
#include "../Components/CollisionModel.hpp"
#include "../Components/Transform.hpp"

namespace Systems
{
	RenderSystem::RenderSystem() = default;

	void RenderSystem::Initialize(std::shared_ptr<Coordinator> coordinator, std::shared_ptr<ShaderProgram> shader, const Entity camera)
	{
		// TODO: Make shader read files from Resources/Shaders
		this->mShader = std::move(shader);
		this->mCoordinator = std::move(coordinator);

		// TODO: Add logic to create another camera in case of an event
		this->mCamera = camera;
	}

	void RenderSystem::Update(float deltaTime)
	{
		this->mShader->Activate();

		MoveCamera();
		for (const auto& iEntity : this->mEntities)
		{
			MoveEntity(iEntity);

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

	void RenderSystem::MoveEntity(Entity entity) const
	{
		auto& transform = this->mCoordinator->GetComponent<Transform>(entity);

		transform.mTransformations = mat4(1.0f);
		transform.mTransformations = translate(transform.mTransformations, transform.mPosition);
		transform.mTransformations = scale(transform.mTransformations, transform.mScale);
		transform.mTransformations = rotate(transform.mTransformations, radians(transform.mRotationAngle), transform.mRotationAxis);

		this->mShader->SendUniformMatrix4f("model", transform.mTransformations);
	}

	void RenderSystem::MoveCamera() const
	{
		auto& camera = this->mCoordinator->GetComponent<Camera>(this->mCamera);
		this->mShader->SendUniformMatrix4f("cameraMatrix", camera.mMatrix);
	}
}
