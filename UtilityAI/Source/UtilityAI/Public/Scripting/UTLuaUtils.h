#pragma once
#include <functional>
#include <Logging/Logger.h>
#include <optional>
#include <sol.hpp>
#include <string>
#include <type_traits>
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

	// Wraps a Lua function in a std::function<Sig>
	template <typename Sig>
	std::function<Sig> WrapLuaFunction(sol::function Fn, const std::string& DebugName)
	{
		using ResultType = typename std::function<Sig>::result_type;

		return std::function<Sig>
		{
			// Lambda that matches signature Sig
			[Fn = std::move(Fn), DebugName](auto&&... Args) -> decltype(auto)
			{
				// Call the Lua function with perfectly-forwarded arguments
				sol::protected_function_result FnResult = Fn(std::forward<decltype(Args)>(Args)...);

				if (!FnResult.valid())
				{
					sol::error Err = FnResult;
					LOG_ERROR(std::format("Error calling Lua function '{}': {}", DebugName, std::string(Err.what())))
				}

				if constexpr (std::is_void_v<ResultType>)
				{
					// If Sig returns void, don't try to extract a value
					return;
				}
				else
				{
					// Otherwise, extract the return value from Lua
					return FnResult.valid() ? FnResult.get<ResultType>() : ResultType{};
				}
			}
		};
	}

	template<typename Sig>
	std::optional<std::function<Sig>> ValidateFunction(const sol::table& Table, const std::string& Field, UTValidationResult& Result, bool bRequired = false)
	{
		sol::object Obj = Table.get<sol::object>(Field);

		if (bRequired && !Obj.valid()) 
		{
			Result.AddError("Missing required function field: " + Field);
			return std::nullopt;
		}
		if (!Obj.valid()) 
		{
			return std::nullopt;
		}
		if (Obj.get_type() != sol::type::function) 
		{
			Result.AddError("Field '" + Field + "' has wrong type (expected function)");
			return std::nullopt;
		}

		sol::function Fn = Obj.as<sol::function>();

		return WrapLuaFunction<Sig>(Fn, Field);
	}

	template<typename ObjT, typename FnSig>
	void LoadFunction(
		ObjT& Out,
		std::function<FnSig> ObjT::* Member,
		const sol::table& Table,
		const std::string& Field,
		UTValidationResult& Result,
		bool bRequired = false)
	{
		if (const auto Fn = ValidateFunction<FnSig>(Table, Field, Result, bRequired)) 
		{
			Out.*Member = *Fn;
		}
	}
}

#define LOAD_FIELD(obj, member, table, result, required) \
    LuaUtils::LoadField(obj, &std::decay_t<decltype(obj)>::member, table, #member, result, required);

#define LOAD_FUNCTION(obj, member, table, result, required) \
    LuaUtils::LoadFunction(obj, &std::decay_t<decltype(obj)>::member, table, #member, result, required);