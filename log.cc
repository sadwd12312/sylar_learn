#include"log.h"
#include<map>

namespace syalr {

	const char* LogLevel::ToString(LogLevel::Level level)
	{
		switch (level)
#define XX(name) \
	case LogLevel::name: \
		return #name;\
		break;

			XX(DEBUG);
		XX(INFO);
		XX(WARN);
		XX(ERROR);
		XX(FATAL);
#undef XX
	default:
		return "UNKNOW";
	}

	class MessageFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getContext();
		}
	};
	class LevelFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << LogLevel::ToString(level);
		}
	};
	class ElapseFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getElapse();
		}
	};
	class NameFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << logger->getName();
		}
	};
	class ThreadIdFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getThreadId();
		}
	};
	class FiberIdFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getFiberId();
		}
	};
	class DateTimeFormatterItem :public FormatItem
	{
	public:
		DateTimeFormatterItem(const std::string& format = "%Y:%m:%d %H:%M:%S")
			:m_format(format) {}
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getTime();
		}
	private:
		std::string m_format;
	};
	class FilenameFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getFile();
		}
	};
	class LineFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << event->getLine();
		}
	};
	class NewLineFormatterItem :public FormatItem
	{
	public:
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << std::endl;
		}
	};
	class StringFormatterItem :public FormatItem
	{
	public:
		StringFormatterItem(const std::string& str)
			:FormatterItem(str), m_string(str) {}
		void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, Logevent::ptr event) override {
			os << m_string;
		}
	private:
		std::string m_string;
	};


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

	void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			m_filestream << m_formatter.format(logger, level, event);
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
	void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		if (level >= m_level)
		{
			std::cout << m_formatter.format(logger, level, event);
		}
	}
	LogFormatter::LogFormatter(const std::string& pattern)
		:m_pattern(pattern) {}

	std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
	{
		std::stringstream ss;
		for (auto& i : m_items)
		{
			i->format(ss, logger, level, event);
		}
		return ss.str();
	}
	void LogFormatter::init() {
		std::vector<std::tuple<std::string, std::string, int>> vec;
		std::string nstr;
		size_t last_pos = 0;
		for (size_t i = 0; i < m_pattern.size(); ++i)
		{
			if (m_pattern[i] != "%")
			{
				nstr.append(1, m_pattern[i]);
				continue;
			}


			size_t n = i + 1;
			int fmt_stauts = 0;
			size_t fmt_begin = 0;

			std::string str;
			std::string fmt;
			while (++n < m_pattern.size()) {
				if (isspace(m_pattern[n]))
				{
					break;
				}
				if (fmt_status == 0)
				{
					if (m_pattern[n] == '{') {
						str = m_pattern.substr(i + 1, n - i);
						fmt_status = 1;//解析格式
						++n;
						fmt_begin = n;
						continue;
					}
				}
				if (fmt_staues == 1)
				{
					if (m_pattern[n] == '}')
					{
						fmt = m_pattern.substr(fmt_begin + 1, n - fmt_egin - 1);
						fmt_status = 2;
						break;
					}
				}
			}
			if (fmt_status == 0)
			{
				if (!nstr.empty())
				{
					vec.push_back(std::make_pair(nstr, "", 0));
				}
				str = m_pattern.substr(i + 1, n - i - 1);
				vec.push_back(std::make_tuple(str, fmt, 1));
			}
			else if (fmt_status == 1)
			{
				stc::cout << "pattern parse error:" << m_pattern << " - " << m_pattern.substr(i) << std::endl;
				vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
			}
			else if (fmt_stauts == 2)
			{
				vec.push_back(std::make_tuple(str, fmt, 1));
			}
			i = n;
		}
		if (!nstr.empty())
		{
			vec.push_back(std::make_pair(nstr, "", 0));
		}
		//%m -- 消息体
		//%p -- Level
		//%r -- 启动后的时间
		//%c -- 日志名称
		//%t -- 线程id
		//%n -- 回车换行
		//%d -- 时间
		//%f -- 文件名
		//%g -- 行号
	}
	static std::map<std::string, std::function<ForamtItem::ptr(const std::string & str)>> s_format_items =
	{
#define XX(str,C) \
		{#str,[](const std::string& fmt){return FormatItem(new C(fmt));}}

		XX(m, MessageFormatItem);
		XX(p, LevelFormatItem);
		XX(r, ElapseFormatItem);
		XX(c, NameFormatItem);
		XX(t, ThreadIdFormatItem);
		XX(n, NewLineFormatItem);
		XX(d, DateTimeFormatItem);
		XX(f, FilenameFormatItem);
		XX(l, LineFormatItem);
#undef XX
	};
	for (auto& i : vec)
	{
		if (std::get<2>[i] == 0)
		{
			m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>[i])));
		}
		else
		{
			auto it = s_format_items.find(std::<0>[i]);
			if (it == s_format_items.end())
			{
				m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
			}
			else
			{
				m_items.push_back(it->second(std::get<1>(i)));
			}
		}
		std::cout << std::get<0>(i) << " - " << std::get<1>(i) << " - " << std::get<2>(i) << std::endl;
	}
}










