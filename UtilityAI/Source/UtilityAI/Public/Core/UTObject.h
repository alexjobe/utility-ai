#pragma once
#include <string>
#include <unordered_set>

namespace UTCore
{
class UTObject
{
public:
	std::unordered_set<std::string> OwnedTags;

	explicit UTObject();
	UTObject(const UTObject& Other);
	UTObject(UTObject&& Other) noexcept;
	virtual ~UTObject() = default;

	// Non-copyable assignment: avoid reassigning const key
	UTObject& operator=(const UTObject&) = delete;
	UTObject& operator=(UTObject&& Other) noexcept = default;

	std::string GetKey() const { return Key; }
	std::string GetName() const { return Name; }
	const std::string& GetDefinitionKey() const { return DefinitionKey; }

	bool IsDefinition() const { return DefinitionKey.empty(); }
	bool IsInstance() const { return !DefinitionKey.empty(); }

	void SetName(const std::string& InName) { Name = InName; }

	virtual std::string ToString() const;

private:
	const std::string Key;       // Unique per object instance (immutable)
	std::string Name;            // Human-readable name
	std::string DefinitionKey;   // Tracks the original definition's key
};
}