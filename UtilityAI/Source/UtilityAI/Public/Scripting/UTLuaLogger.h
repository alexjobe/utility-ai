#pragma once
#include <Logging/Logger.h>
#include <sol.hpp>
#include <string>

namespace LuaLog
{
	inline void Debug(const std::string& Msg)
	{
		Log::Logger::Instance().Log(Log::ELogLevel::Debug, Msg, __FILE__, __LINE__);
	}

	inline void Info(const std::string& Msg)
	{
		Log::Logger::Instance().Log(Log::ELogLevel::Info, Msg, __FILE__, __LINE__);
	}

	inline void Warn(const std::string& Msg)
	{
		Log::Logger::Instance().Log(Log::ELogLevel::Warning, Msg, __FILE__, __LINE__);
	}

	inline void Error(const std::string& Msg)
	{
		Log::Logger::Instance().Log(Log::ELogLevel::Error, Msg, __FILE__, __LINE__);
	}

	inline void Critical(const std::string& Msg)
	{
		Log::Logger::Instance().Log(Log::ELogLevel::Critical, Msg, __FILE__, __LINE__);
	}

	void RegisterLogger(sol::state& Lua)
	{
		sol::table Log = Lua.create_named_table("Log");
		Log.set_function("Debug", &LuaLog::Debug);
		Log.set_function("Info", &LuaLog::Info);
		Log.set_function("Warn", &LuaLog::Warn);
		Log.set_function("Error", &LuaLog::Error);
		Log.set_function("Critical", &LuaLog::Critical);


		// Override print to use our logger
		Lua.set_function("print", [](const std::string& Msg)
		{
			Log::Logger::Instance().Log(Log::ELogLevel::Info, Msg, __FILE__, __LINE__);
		});
	}
}