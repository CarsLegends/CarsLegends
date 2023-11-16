#ifndef MANAGERS_EVENT_MANAGER_HPP
#define MANAGERS_EVENT_MANAGER_HPP

#include <functional>

#include "../DataStructures/Types.hpp"
#include "../Events/Event.hpp"

namespace Managers
{
	using namespace	DataStructures;
	using namespace Events;

	class EventManager
	{
	public:
		void AddListener(EventId eventId, std::function<void(Event&)> const& listener);
		void SendEvent(Event& event);
		void SendEvent(EventId eventId);
	private:
		std::unordered_map<EventId, std::list<std::function<void(Event&)>>> mListeners;
	};
}

#endif