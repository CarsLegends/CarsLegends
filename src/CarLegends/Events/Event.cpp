#include "Event.hpp"

namespace Events {
	Event::Event() = default;

	Event::Event(EventId type) {
		this->mType = type;
	}

	template <typename T>
	void Event::SetParam(EventId id, T value) {
		this->mData[id] = value;
	}

	template <typename T>
	T Event::GetParam(EventId id) {
		return any_cast<T>(mData[id]);
	}

	EventId Event::GetType() const {
		return mType;
	}
}
