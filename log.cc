#include"log.h"

namespace syalr {
	Logger::Logger(const std::string& name)
		:m_name(name) {}


	void Logger::addAppender(LogAppender::ptr appender);
	{
		m_appenders.push_back(appender);
	}
	void Logger::delAppender(LogAppender::ptr appender);
	{
		for (auto it = m_appenders.begin(); it!m_appenders.den(); ++it)
			if (*it == appender)
			{
				m_appenders.erase(it);
				break;
			}
	}
	void Logger::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			for (auto& i : m_appenders)
			{
				i->log(level, event);
			}
		}
	}

	void Logger::debug(LogEvent::ptr event);
	{
		debug(LogLevel::DEBUG, level);
	}
	void Logger::info(LogEvent::ptr event);
	{
		debug(LogLevel::INFO, level);
	}
	void Logger::warn(LogEvent::ptr event);
	{
		debug(LogLevel::WARN, level);
	}
	void Logger::error(LogEvent::ptr event);
	{
		debug(LogLevel::ERROR, level);
	}
	void Logger::fatal(LogEvent::ptr event);
	{
		debug(LogLevel::FATAL, level);
	}

	FileLogAppender::FileLogAppender(const std::string& filename)
		:m_filename(filename) {}

	void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			m_filestream << m_formatter.format(event);
		}
	}
	
	bool FileLogAppender::reopen()
	{
		if (m_filestream)
		{
			m_filestream.close();
		}
		m_filestream.open(m_filename);
		return !!m_filestream;
	}
	void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			std::cout << m_formatter.format(event);
		}
	}
}