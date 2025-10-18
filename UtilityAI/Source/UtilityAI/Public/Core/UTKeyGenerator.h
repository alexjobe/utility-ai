#pragma once
#include <string>

namespace UTCore
{
class UTKeyGenerator
{
public:
	static UTKeyGenerator& Instance();

	// Creates a UUID
	std::string GenerateKey();
};
}
