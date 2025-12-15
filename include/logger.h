#pragma once
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <memory>
#include <mutex>
#include <fstream>

class Logger {
public:
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Объявляем ссылку на синглтон
    static Logger& instance;

    void setLogFile(const QString& filename);
    void setLogLevel(LogLevel level);
    void setConsoleOutput(bool enabled);

    void log(LogLevel level, const QString& message);
    void debug(const QString& message);
    void info(const QString& message);
    void warning(const QString& message);
    void error(const QString& message);
    void critical(const QString& message);

    void logToStream(std::ofstream& stream, LogLevel level, const QString& message) const;

    Logger& operator<<(const QString& message);

    friend QString formatLogMessage(LogLevel level, const QString& message);

private:
    QString m_logFile;
    LogLevel m_logLevel = LogLevel::Info;
    bool m_consoleOutput = true;
    std::unique_ptr<QFile> m_file;
    std::unique_ptr<QTextStream> m_stream;
    std::mutex m_mutex;

    QString levelToString(LogLevel level) const;
    void writeLog(LogLevel level, const QString& message);
};

// Дружественная функция
QString formatLogMessage(Logger::LogLevel level, const QString& message);
