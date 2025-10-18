#include "Core/UTKeyGenerator.h"
#include <iomanip>
#include <random>
#include <sstream>

using namespace UTCore;

UTKeyGenerator& UTKeyGenerator::Instance()
{
	static UTKeyGenerator Singleton;
	return Singleton;
}

std::string UTKeyGenerator::GenerateKey()
{
	/*
	* UUID Format: xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx
	* M bits encode the version (for UUIDv4, 0100 in binary = 4 in hex)
	* N bits encode the variant (for RFC 4122 UUIDs, 10xxxxxx in binary)
	*/
	static std::random_device RandomDevice;
	static std::mt19937 Gen(RandomDevice());
	static std::uniform_int_distribution<uint32_t> Dist(0, 0xFFFFFFFF);

	std::ostringstream OSS;
	OSS << std::hex << std::setfill('0')
		// First group : 8 hex digits(32 bits)
		<< std::setw(8) << Dist(Gen)
		<< "-"
		// Second group : 4 hex digits(16 bits)
		<< std::setw(4) << (Dist(Gen) >> 16)
		<< "-"
		// Third group: version (4 bits forced to 0100)
		<< std::setw(4) << ((Dist(Gen) >> 16) & 0x0FFF | 0x4000)
		<< "-"
		// Fourth group: variant (top 2 bits forced to 10)
		<< std::setw(4) << ((Dist(Gen) >> 16) & 0x3FFF | 0x8000)
		<< "-"
		// Fifth group: 12 hex digits (48 bits total)
		<< std::setw(12) << Dist(Gen) << Dist(Gen) << Dist(Gen);
	return OSS.str();
}