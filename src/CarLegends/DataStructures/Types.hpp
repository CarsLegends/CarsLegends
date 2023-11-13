#ifndef DATA_STRUCTURES_TYPES_HPP
#define DATA_STRUCTURES_TYPES_HPP

#include <bitset>
#include "../Configurations/ManagersConfiguration.hpp"

namespace DataStructures {
	using namespace std;
	using namespace Configuration;

	using Entity = uint32_t;
	using EventId = uint32_t;
	using ParameterId = uint32_t;
	using ComponentType = uint8_t;
	using Signature = bitset<MAX_COMPONENTS>;
	
}

#endif

using Entity = uint32_t;