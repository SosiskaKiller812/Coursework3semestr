#include "logger.h"
#include <QDebug>

Logger& Logger::instance = *new Logger();

Logger::Logger() {}
Logger::~Logger() {}

void Logger::setLogFile(const QString& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logFile = filename;
    m_file = std::make_unique<QFile>(m_logFile);
    if (m_file->open(QIODevice::Append | QIODevice::Text)) {
        m_stream = std::make_unique<QTextStream>(m_file.get());
    }
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logLevel = level;
}

void Logger::setConsoleOutput(bool enabled) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_consoleOutput = enabled;
}

void Logger::log(LogLevel level, const QString& message) {
    if (level < m_logLevel) return;
    writeLog(level, message);
}

void Logger::debug(const QString& message)    { log(LogLevel::Debug, message); }
void Logger::info(const QString& message)     { log(LogLevel::Info, message); }
void Logger::warning(const QString& message)  { log(LogLevel::Warning, message); }
void Logger::error(const QString& message)    { log(LogLevel::Error, message); }
void Logger::critical(const QString& message) { log(LogLevel::Critical, message); }

void Logger::logToStream(std::ofstream& stream, LogLevel level, const QString& message) const {
    stream << formatLogMessage(level, message).toStdString() << std::endl;
}

Logger& Logger::operator<<(const QString& message) {
    log(LogLevel::Info, message);
    return *this;
}

QString Logger::levelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::Debug:    return "DEBUG";
    case LogLevel::Info:     return "INFO";
    case LogLevel::Warning:  return "WARNING";
    case LogLevel::Error:    return "ERROR";
    case LogLevel::Critical: return "CRITICAL";
    }
    return "UNKNOWN";
}

void Logger::writeLog(LogLevel level, const QString& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    QString formatted = formatLogMessage(level, message);

    if (m_consoleOutput) {
        qDebug().noquote() << formatted;
    }
    if (m_stream) {
        (*m_stream) << formatted << "\n";
        m_stream->flush();
    }
}

QString formatLogMessage(Logger::LogLevel level, const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString levelStr;
    switch (level) {
    case Logger::LogLevel::Debug:    levelStr = "DEBUG"; break;
    case Logger::LogLevel::Info:     levelStr = "INFO"; break;
    case Logger::LogLevel::Warning:  levelStr = "WARNING"; break;
    case Logger::LogLevel::Error:    levelStr = "ERROR"; break;
    case Logger::LogLevel::Critical: levelStr = "CRITICAL"; break;
    }
    return QString("[%1] [%2] %3").arg(timestamp, levelStr, message);
}
