#ifndef DATA_STRUCTURES_EVENT_HPP
#define DATA_STRUCTURES_EVENT_HPP

#include <unordered_map>
#include <any>

#include "../DataStructures/Types.hpp"

namespace Events {
	using namespace std;
	using namespace DataStructures;
	class Event {
	public:
		Event();
		explicit Event(EventId type);
		template<typename T> void SetParam(EventId id, T value);
		template<typename T> T GetParam(EventId id);
		EventId GetType() const;

	private:
		EventId mType{};
		// Requires of cpp17 standard
		unordered_map<EventId, any> mData{};
	};

	
}

#endif 