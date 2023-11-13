#ifndef EVENTS_EVENTS_HPP
#define EVENTS_EVENTS_HPP

#include "../DataStructures/Types.hpp"

namespace Events
{
	using namespace DataStructures;

	namespace Window
	{
		const EventId QUIT = "QUIT"_hash;
		const EventId RESIZED = "RESIZED"_hash;
		const EventId INPUT = "INPUT"_hash;

		namespace Resized
		{
			const ParameterId WIDTH = "WIDTH"_hash;
			const ParameterId HEIGHT = "HEIGHT"_hash;
		}
	}
}

#endif
