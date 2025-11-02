#include <Core/UTKeyGenerator.h>
#include <Core/UTObject.h>
#include <format>
#include <utility>

using namespace UTCore;

UTObject::UTObject()
	: Key(std::move(UTKeyGenerator::Instance().GenerateKey()))
{
}

UTObject::UTObject(const UTObject& Other)
	: Key(UTKeyGenerator::Instance().GenerateKey()) // New key
	, Name(Other.Name)
	, DefinitionKey(Other.DefinitionKey.empty() ? Other.Key : Other.DefinitionKey)
{
}

UTObject::UTObject(UTObject&& Other) noexcept
	: Key(Other.Key) // Copy, since it's const
	, Name(std::move(Other.Name))
	, DefinitionKey(std::move(Other.DefinitionKey))
{
}

std::string UTObject::ToString() const
{
	return std::format("[{}] {}", Key, Name);
}