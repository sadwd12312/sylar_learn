#ifndef _SYLAR_LOG_H_
#define _SYLAR_LOG_H_

#include<string>
#include<stdint.h>
#include<memory>
#include<list>
#include<stringstream>
#include<fstream>
#include<vector>

//��־�¼�
class LogEvent {
public:
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();
private:
	const char* m_file = nullptr; //�ļ���
	int32_t m_line = 0;//�к�
	uint32_t m_elapse = 0;//�������������ڵĺ�����
	uint32_t m_threadId = 0;//�߳�id
	uint32_t m_fiberId = 0;//Э��id
	uint64_t m_time = 0;//ʱ���
	std::string m_content;
};
//��־����
class LogLevel {
public:
	enum Level
	{
		DEBUG = 1,
		INFO=2,
		WARN=3,
		ERROR=4,
		FATAL=5
	};
};

//��־��ʽ��
class LogFormatter
{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	LogFormatter(const std::string& pattern);

	
	std::string format(LogEvent::ptr event);
private:
	class FormatItem
	{
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		virtual ~FormatItem(){}
		virtual void format(std::ostream& os,LogEvent::ptr event) = 0;
	};
	void init();


private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_items;
};

//��־�����
class LogAppender
{
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender() {}

	virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

	void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
	LogFormatter::ptr getFormatter()const { return m_formatter; }
protected:
	LogLevel::Level m_level;
	LogFormatter::ptr m_formatter;
};

//��־��
class Logger {
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(const std::string& name = "root");
	void log(LogLevel::Level level, LogEvent::ptr event);

	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);

	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);
	LogLevel::Level getLevel()const { return m_level; }
	void setLevel(LogLevel::Level val) { m_level = val; }
private:
	std::string m_name;//��־����
	LogLevel::Level m_level;//��־����
	std::list<LogAppender::ptr> m_appenders;
};

//���������̨��Appender
class StdoutLogAppender :public LogAppender {
public:
	typedef std::shared_ptr<stdoutLogAppender> ptr;
	void log(LogLevel::Level, LogEvent::ptr event)override;
};
//������ļ���Appender
class FileLogAppender :public LogAppender{
public:
	typedef std::shared_ptr<fileLogAppender> ptr;
	FileLogAppender(const std::string& filename);
	void log(LogLevel::Level, LogEvent::ptr event)override;
	//���´��ļ�,�ļ��򿪳ɹ�,����true
	bool reopen();
private:
	std::string m_filename;
	std::ofstrram m_filestream;
}
//#endif































