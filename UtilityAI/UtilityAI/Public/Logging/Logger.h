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


enum class LogLevel 
{
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

inline const char* ToString(LogLevel InLevel) 
{
	switch (InLevel) 
	{
	case LogLevel::Debug:    return "DEBUG";
	case LogLevel::Info:     return "INFO";
	case LogLevel::Warning:  return "WARN";
	case LogLevel::Error:    return "ERROR";
	case LogLevel::Critical: return "CRIT";
	}
	return "UNKNOWN";
}

class ILogSink 
{
public:
	virtual ~ILogSink() = default;
	virtual void Log(LogLevel InLevel, const std::string& InMessage) = 0;
};

class ConsoleSink : public ILogSink 
{
public:
	virtual void Log(LogLevel InLevel, const std::string& InMessage) 
	{
		std::lock_guard<std::mutex> LogLock(LogMutex);
		std::cout << "[" << ToString(InLevel) << "] " << InMessage << std::endl;
	}
private:
	std::mutex LogMutex;
};

class FileSink : public ILogSink 
{
public:
	explicit FileSink(const std::string& InFileName) 
		: File(InFileName, std::ios::app) 
	{
	}

	void Log(LogLevel InLevel, const std::string& InMessage) override 
	{
		std::lock_guard<std::mutex> LogLock(LogMutex);
		File << "[" << Timestamp() << "][" << ToString(InLevel) << "] " << InMessage << std::endl;
	}
private:
	std::ofstream File;
	std::mutex LogMutex;

	std::string Timestamp() 
	{
		auto Now = std::chrono::system_clock::now();
		return std::format("{:%Y-%m-%d %H:%M:%S}", Now);
	}
};

class Logger {
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

	void Log(LogLevel InLevel, const std::string& InMessage, const char* InFile, int InLine) 
	{
		std::ostringstream OSS;
		OSS << InFile << ":" << InLine << " | " << InMessage;

		{
			std::lock_guard<std::mutex> QueueLock(QueueMutex);
			LogQueue.push({ InLevel, OSS.str() });
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

	struct LogMessage 
	{
		LogLevel Level;
		std::string Text;
	};

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
				auto Message = LogQueue.front();
				LogQueue.pop();
				QueueLock.unlock();

				std::lock_guard<std::mutex> SinksLock(SinksMutex);
				for (auto& Sink : Sinks) 
				{
					Sink->Log(Message.Level, Message.Text);
				}

				QueueLock.lock();
			}
		}
	}
};

// Macros
#define LOG_DEBUG(Msg)    Logger::Instance().Log(LogLevel::Debug,    Msg, __FILE__, __LINE__);
#define LOG_INFO(Msg)     Logger::Instance().Log(LogLevel::Info,     Msg, __FILE__, __LINE__);
#define LOG_WARN(Msg)     Logger::Instance().Log(LogLevel::Warning,  Msg, __FILE__, __LINE__);
#define LOG_ERROR(Msg)    Logger::Instance().Log(LogLevel::Error,    Msg, __FILE__, __LINE__);
#define LOG_CRITICAL(Msg) Logger::Instance().Log(LogLevel::Critical, Msg, __FILE__, __LINE__);