#ifndef SYSTEMS_NUKLEAR_SYSTEM_HPP
#define SYSTEMS_NUKLEAR_SYSTEM_HPP

#include <memory>

#include "ISystem.hpp"
#include "nuklear/nuklear.h" // Asumiendo que "nuklear.h" es la única dependencia necesaria para Nuklear
#include "../Coordinators/Coordinator.hpp"
#include "../Components/Transform.hpp"	// Remove this line
#include "../Engine/Window/Window.hpp"

namespace Systems
{
	using namespace Coordinators;
	using namespace Components;
	using namespace Windows;

	class MenuSystem : public ISystem
	{
	public:
		MenuSystem();
		void Initialize(std::shared_ptr<Coordinator> coordinator, Window window);
		void Update(float deltaTime);

	private:
		struct nk_context* mNkContext; // Puntero a la estructura de contexto de Nuklear
		std::shared_ptr<Coordinator> mCoordinator;

		void RenderUI() const; // Método para renderizar la UI con Nuklear
	};
}

#endif // SYSTEMS_NUKLEAR_SYSTEM_HPP
