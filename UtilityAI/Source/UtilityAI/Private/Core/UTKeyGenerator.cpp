#include "Core/UTKeyGenerator.h"
#include <iomanip>
#include <random>
#include <sstream>

using namespace Core;

UTKeyGenerator& UTKeyGenerator::Instance()
{
	static UTKeyGenerator Singleton;
	return Singleton;
}

std::string UTKeyGenerator::GenerateKey()
{
	static std::random_device RandomDevice;
	static std::mt19937 Gen(RandomDevice());
	static std::uniform_int_distribution<uint32_t> Dist(0, 0xFFFFFFFF);

	std::ostringstream OSS;
	OSS << std::hex << std::setfill('0')
		<< std::setw(8) << Dist(Gen)
		<< "-"
		<< std::setw(4) << (Dist(Gen) >> 16)
		<< "-"
		<< std::setw(4) << ((Dist(Gen) >> 16) & 0x0FFF | 0x4000)
		<< "-"
		<< std::setw(4) << ((Dist(Gen) >> 16) & 0x3FFF | 0x8000)
		<< "-"
		<< std::setw(12) << Dist(Gen) << Dist(Gen) << Dist(Gen);
	return OSS.str();
}