#ifndef DATA_STRUCTURES_EVENT_HPP
#define DATA_STRUCTURES_EVENT_HPP

#include <unordered_map>
#include <any>

#include "../DataStructures/Types.hpp"

namespace Events
{
	using namespace DataStructures;

	class Event
	{
	public:
		Event() = default;

		Event(EventId type)
		{
			this->mType = type;
		}

		template <typename T>
		void SetParam(EventId id, T value)
		{
			this->mData[id] = value;
		}

		template <typename T>
		T GetParam(EventId id)
		{
			return std::any_cast<T>(mData[id]);
		}

		EventId GetType() const
		{
			return mType;
		}

	private:
		EventId mType{};
		// Requires of cpp17 standard
		std::unordered_map<EventId, std::any> mData{};
	};
}

#endif
