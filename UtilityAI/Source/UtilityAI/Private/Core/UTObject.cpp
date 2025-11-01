#include <Core/UTKeyGenerator.h>
#include <Core/UTObject.h>
#include <format>
#include <utility>

using namespace UTCore;

UTObject::UTObject()
	: Key(std::move(UTKeyGenerator::Instance().GenerateKey()))
{
}

std::string UTObject::ToString() const
{
	return std::format("[{}] {}", Key, Name);
}

