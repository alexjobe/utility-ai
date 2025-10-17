#pragma once
#include <Logging/UTLogger.h>
#include <sol.hpp>
#include <string>

namespace UTLuaLog
{
	inline void Debug(const std::string& Msg)
	{
		UTLog::Logger::Instance().Log(UTLog::ELogLevel::Debug, Msg, __FILE__, __LINE__);
	}

	inline void Info(const std::string& Msg)
	{
		UTLog::Logger::Instance().Log(UTLog::ELogLevel::Info, Msg, __FILE__, __LINE__);
	}

	inline void Warn(const std::string& Msg)
	{
		UTLog::Logger::Instance().Log(UTLog::ELogLevel::Warning, Msg, __FILE__, __LINE__);
	}

	inline void Error(const std::string& Msg)
	{
		UTLog::Logger::Instance().Log(UTLog::ELogLevel::Error, Msg, __FILE__, __LINE__);
	}

	inline void Critical(const std::string& Msg)
	{
		UTLog::Logger::Instance().Log(UTLog::ELogLevel::Critical, Msg, __FILE__, __LINE__);
	}

	void RegisterLogger(sol::state& Lua)
	{
		sol::table Log = Lua.create_named_table("Log");
		Log.set_function("Debug", &UTLuaLog::Debug);
		Log.set_function("Info", &UTLuaLog::Info);
		Log.set_function("Warn", &UTLuaLog::Warn);
		Log.set_function("Error", &UTLuaLog::Error);
		Log.set_function("Critical", &UTLuaLog::Critical);


		// Override print to use our logger
		Lua.set_function("print", [](const std::string& Msg)
		{
			UTLog::Logger::Instance().Log(UTLog::ELogLevel::Info, Msg, __FILE__, __LINE__);
		});
	}
}