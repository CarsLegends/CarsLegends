#ifndef SYSTEMS_HIT_BOX_RENDER_HPP
#define SYSTEMS_HIT_BOX_RENDER_HPP

#include "ISystem.hpp"
#include "../Coordinators/Coordinator.hpp"
#include "../Engine/Shaders/ShaderProgram.hpp"
#include "../Engine/Buffers/Structs/Vertex.hpp"
#include "../Engine/Buffers/ElementBuffer.hpp"
#include "../Components/Renderable.hpp"
#include "../Events/EventTypes.hpp"
#include "../Components/Transform.hpp"
#include "../Components/Camera.hpp"

namespace Systems
{
	using namespace Coordinators;
	using namespace Shaders;
	using namespace Buffers;
	using namespace Events;
	using namespace Components;

	class HitBoxRenderSystem final : public ISystem
	{
	public:
		HitBoxRenderSystem();
		void Initialize(const std::shared_ptr<Coordinator>& coordinator, const std::shared_ptr<ShaderProgram>& shader);
		void Update(float deltaTime) override;

	private:
		std::shared_ptr<Coordinator> mCoordinator;
		std::shared_ptr<ShaderProgram> mShader;

		VertexArray mVertexArray{};
		Entity mCamera;
		bool mShowHitBoxes;

		void ShowHitBoxHandler(Event& event);
		void DontShowHitBoxHandler(Event& event);
		void MoveEntity(Entity entity) const;
		void InitializeDataInGPU();
		void MoveCamera() const;

		inline static std::vector<Vertex> mSimplestHitBoxVertices = 
		{
			Vertex(vec3{ -0.5, -0.5, -0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{  0.5, -0.5, -0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{  0.5,  0.5, -0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{ -0.5,  0.5, -0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{ -0.5, -0.5,  0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{  0.5, -0.5,  0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{  0.5,  0.5,  0.5}, { 46.0f, 139.0f, 87.0f }),
			Vertex(vec3{ -0.5,  0.5,  0.5}, { 46.0f, 139.0f, 87.0f }),
		};

		inline static std::vector<uint32_t> mSimplestHitBoxElements = 
		{
		  0, 1, 2, 3,
		  4, 5, 6, 7,
		  0, 4, 1, 5,
		  2, 6, 3, 7
		};
	};

}

#endif