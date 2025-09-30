#include <Core/UTFunctionRegistry.h>

using namespace UAI;

UTFunctionRegistry& UTFunctionRegistry::Instance()
{
	static UTFunctionRegistry Singleton;
	return Singleton;
}

UTFunctionRegistry::~UTFunctionRegistry()
{
	Clear();
}

void UTFunctionRegistry::Clear()
{
	Functions.clear();
}
