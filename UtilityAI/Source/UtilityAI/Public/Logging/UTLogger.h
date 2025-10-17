#pragma once
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

namespace UTLog
{
enum class ELogLevel 
{
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

enum class EVerbosity {
	Minimal,   // Just the message
	Normal,    // Timestamp + message
	Detailed   // Timestamp + file:line + message
};

inline const char* ToString(ELogLevel InLevel) 
{
	switch (InLevel) 
	{
	case ELogLevel::Debug:    return "DEBUG";
	case ELogLevel::Info:     return "INFO";
	case ELogLevel::Warning:  return "WARN";
	case ELogLevel::Error:    return "ERROR";
	case ELogLevel::Critical: return "CRIT";
	}
	return "UNKNOWN";
}

struct LogMessage 
{
	ELogLevel Level;
	std::string Message;
	const char* File;
	int Line;
	std::chrono::system_clock::time_point Timestamp;
};

inline std::string FormatTimestamp(const std::chrono::system_clock::time_point& TP)
{
	return std::format("{:%Y-%m-%d %H:%M:%S}", TP);
}

inline std::string FormatMessage(const LogMessage& InMsg, EVerbosity InVerbosity = EVerbosity::Normal)
{
	std::ostringstream OSS;
	switch (InVerbosity)
	{
	case EVerbosity::Minimal:
		OSS << InMsg.Message;
		break;

	case EVerbosity::Normal:
		OSS << FormatTimestamp(InMsg.Timestamp)
			<< " [" << ToString(InMsg.Level) << "] "
			<< InMsg.Message;
		break;

	case EVerbosity::Detailed:
		OSS << FormatTimestamp(InMsg.Timestamp)
			<< " [" << ToString(InMsg.Level) << "] "
			<< InMsg.File << ":" << InMsg.Line << " | "
			<< InMsg.Message;
		break;
	}
	return OSS.str();
}

class ILogSink 
{
public:
	virtual ~ILogSink() = default;
	virtual void Log(const LogMessage& InMsg) = 0;
};

class ConsoleSink : public ILogSink 
{
public:
	explicit ConsoleSink(EVerbosity InVerbosity = EVerbosity::Normal) 
		: Verbosity(InVerbosity) 
	{}

	virtual void Log(const LogMessage& InMsg) override
	{
		std::lock_guard<std::mutex> LogLock(LogMutex);
		std::cout << FormatMessage(InMsg, Verbosity) << std::endl;
	}
private:
	EVerbosity Verbosity;
	std::mutex LogMutex;
};

class FileSink : public ILogSink 
{
public:
	explicit FileSink(const std::string& InFileName, EVerbosity InVerbosity = EVerbosity::Normal)
		: File(InFileName, std::ios::app)
		, Verbosity(InVerbosity)
	{}

	void Log(const LogMessage& InMsg) override
	{
		std::lock_guard<std::mutex> LogLock(LogMutex);
		File << FormatMessage(InMsg, Verbosity) << std::endl;
	}
private:
	EVerbosity Verbosity;
	std::ofstream File;
	std::mutex LogMutex;
};

class Logger 
{
public:
	static Logger& Instance() 
	{
		static Logger Singleton;
		return Singleton;
	}

	void AddSink(std::shared_ptr<ILogSink> InSink) 
	{
		std::lock_guard<std::mutex> SinksLock(SinksMutex);
		Sinks.push_back(InSink);
	}

	void Log(ELogLevel InLevel, const std::string& InMessage, const char* InFile, int InLine) 
	{
		LogMessage Msg
		{
			InLevel,
			InMessage,
			InFile,
			InLine,
			std::chrono::system_clock::now()
		};

		{
			std::lock_guard<std::mutex> QueueLock(QueueMutex);
			LogQueue.push(std::move(Msg));
		}

		CondVar.notify_one();
	}

	void Shutdown() 
	{
		bIsRunning = false;
		CondVar.notify_one();
		if (WorkerThread.joinable()) 
		{
			WorkerThread.join();
		}
	}

private:
	Logger() : bIsRunning(true), WorkerThread(&Logger::WorkerLoop, this) {}
	~Logger() { Shutdown(); }

	std::vector<std::shared_ptr<ILogSink>> Sinks;
	std::mutex SinksMutex;

	std::queue<LogMessage> LogQueue;
	std::mutex QueueMutex;
	std::condition_variable CondVar;

	std::atomic<bool> bIsRunning;
	std::thread WorkerThread;

	void WorkerLoop() 
	{
		while (bIsRunning || !LogQueue.empty()) 
		{
			std::unique_lock<std::mutex> QueueLock(QueueMutex);
			CondVar.wait(QueueLock, [this] { return !LogQueue.empty() || !bIsRunning; });

			while (!LogQueue.empty()) 
			{
				LogMessage Message = LogQueue.front();
				LogQueue.pop();
				QueueLock.unlock();

				std::lock_guard<std::mutex> SinksLock(SinksMutex);
				for (auto& Sink : Sinks) 
				{
					Sink->Log(Message);
				}

				QueueLock.lock();
			}
		}
	}
};
}

// Macros
#define LOG_DEBUG(Msg)    UTLog::Logger::Instance().Log(UTLog::ELogLevel::Debug,    Msg, __FILE__, __LINE__);
#define LOG_INFO(Msg)     UTLog::Logger::Instance().Log(UTLog::ELogLevel::Info,     Msg, __FILE__, __LINE__);
#define LOG_WARN(Msg)     UTLog::Logger::Instance().Log(UTLog::ELogLevel::Warning,  Msg, __FILE__, __LINE__);
#define LOG_ERROR(Msg)    UTLog::Logger::Instance().Log(UTLog::ELogLevel::Error,    Msg, __FILE__, __LINE__);
#define LOG_CRITICAL(Msg) UTLog::Logger::Instance().Log(UTLog::ELogLevel::Critical, Msg, __FILE__, __LINE__);