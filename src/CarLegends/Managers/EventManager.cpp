#include "EventManager.hpp"

namespace Managers
{
	void EventManager::AddListener(EventId eventId, std::function<void(Event&)> const& listener)
	{
		this->mListeners[eventId].push_back(listener);
	}

	void EventManager::SendEvent(Event& event)
	{
		const uint32_t type = event.GetType();

		for (auto const& listener : this->mListeners[type])
		{
			listener(event);
		}
	}

	void EventManager::SendEvent(EventId eventId)
	{
		Event event(eventId);

		for (auto const& listener : this->mListeners[eventId])
		{
			listener(event);
		}
	}
}
