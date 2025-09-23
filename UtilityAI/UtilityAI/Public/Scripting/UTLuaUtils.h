#pragma once
#include <optional>
#include <sol.hpp>
#include <string>
#include <vector>

namespace LuaUtils
{
	struct UTValidationResult
	{
		bool bValid = true;
		std::vector<std::string> Errors;

		void AddError(const std::string& Msg)
		{
			bValid = false;
			Errors.push_back(Msg);
		}
	};

	template<typename T>
	std::optional<T> ValidateField(const sol::table& Table, const std::string& Field, UTValidationResult& Result, bool bRequired = false)
	{
		sol::object Obj = Table.get<sol::object>(Field);

		if (bRequired && !Obj.valid())
		{
			Result.AddError("Missing required field: " + Field);
			return std::nullopt;
		}
		if (Obj.valid() && Obj.get_type() != sol::type_of<T>())
		{
			Result.AddError("Field '" + Field + "' has wrong type");
			return std::nullopt;
		}
		return Obj.valid() ? std::optional<T>(Obj.as<T>()) : std::nullopt;
	}

	template<typename ObjT, typename MemberT>
	void LoadField(
		ObjT& Out,
		MemberT ObjT::* Member,
		const sol::table& Table,
		const std::string& Field,
		UTValidationResult& Result,
		bool bRequired = false)
	{
		if (const auto Value = ValidateField<MemberT>(Table, Field, Result, bRequired))
		{
			Out.*Member = *Value;
		}
	}
}

#define LOAD_FIELD(obj, member, table, result, required) \
    LuaUtils::LoadField(obj, &std::decay_t<decltype(obj)>::member, table, #member, result, required);