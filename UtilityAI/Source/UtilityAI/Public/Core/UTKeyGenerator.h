#pragma once
#include <string>

namespace Core
{
class UTKeyGenerator
{
public:
	static UTKeyGenerator& Instance();

	// Creates a UUID
	std::string GenerateKey();
};
}
