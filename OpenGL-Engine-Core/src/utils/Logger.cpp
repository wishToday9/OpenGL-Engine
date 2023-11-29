#include "pch.h"
#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace OpenGL_Engine
{
	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;

	Logger::Logger()
	{
		Init();
	}

	Logger::~Logger()
	{

	}

	Logger& Logger::GetInstance()
	{
		static Logger logger;
		return logger;
	}

	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_EngineLogger = spdlog::stdout_color_mt("Arcane");
		s_EngineLogger->set_level(spdlog::level::trace);
	}
}