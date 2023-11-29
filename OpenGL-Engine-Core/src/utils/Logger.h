#pragma once


#include "Singleton.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace OpenGL_Engine {  

	class Logger : public Singleton
	{
	private:
		Logger();
		~Logger();
	public:
		static Logger& GetInstance();
		inline static std::shared_ptr<spdlog::logger> GetEngineLogger() { return GetInstance().s_EngineLogger; }
	private:
		static void Init();
	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
	};
} 

// Engine Log Macros
#ifdef ARC_FINAL
#define ARC_LOG_FATAL
#define ARC_LOG_ERROR
#define ARC_LOG_WARN
#define ARC_LOG_INFO
#define ARC_LOG_TRACE
#else
#define ARC_LOG_FATAL(...) OpenGL_Engine::Logger::GetEngineLogger()->critical(__VA_ARGS__)
#define ARC_LOG_ERROR(...) OpenGL_Engine::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define ARC_LOG_WARN(...)  OpenGL_Engine::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define ARC_LOG_INFO(...)  OpenGL_Engine::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define ARC_LOG_TRACE(...) OpenGL_Engine::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#endif