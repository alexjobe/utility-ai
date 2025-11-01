#pragma once
#include <string>

namespace UTCore
{
class UTObject
{
public:
	explicit UTObject();
	virtual ~UTObject() = default;

	std::string GetKey() const { return Key; }
	std::string GetName() const { return Name; }
	void SetName(const std::string& InName) { Name = InName; }

	virtual std::string ToString() const;

private:
	const std::string Key;
	std::string Name;
};
}