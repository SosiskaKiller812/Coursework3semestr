#include "logger.h"
#include <QDateTime>
#include <iostream>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() = default;

Logger::~Logger() {
    if (m_stream) {
        m_stream->flush();
    }
}

void Logger::setLogFile(const QString& filename) {
    m_logFile = filename;
    m_file = std::make_unique<QFile>(filename);
    if (m_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_stream = std::make_unique<QTextStream>(m_file.get());
        m_stream->setEncoding(QStringConverter::Utf8);
    }
}

void Logger::setLogLevel(LogLevel level) {
    m_logLevel = level;
}

void Logger::setConsoleOutput(bool enabled) {
    m_consoleOutput = enabled;
}

void Logger::log(LogLevel level, const QString& message) {
    if (level < m_logLevel) {
        return;
    }
    
    writeLog(level, message);
}

void Logger::debug(const QString& message) {
    log(LogLevel::Debug, message);
}

void Logger::info(const QString& message) {
    log(LogLevel::Info, message);
}

void Logger::warning(const QString& message) {
    log(LogLevel::Warning, message);
}

void Logger::error(const QString& message) {
    log(LogLevel::Error, message);
}

void Logger::critical(const QString& message) {
    log(LogLevel::Critical, message);
}

void Logger::logToStream(std::ofstream& stream, LogLevel level, const QString& message) const {
    if (!stream.is_open()) {
        return;
    }
    
    QString formatted = formatLogMessage(level, message);
    stream << formatted.toStdString() << std::endl;
}

Logger& Logger::operator<<(const QString& message) {
    info(message);
    return *this;
}

QString Logger::levelToString(LogLevel level) const {
    using enum LogLevel;
    switch (level) {
        case Debug: return "DEBUG";
        case Info: return "INFO";
        case Warning: return "WARNING";
        case Error: return "ERROR";
        case Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::writeLog(LogLevel level, const QString& message) {
    std::scoped_lock lock(m_mutex);
    
    QString formatted = formatLogMessage(level, message);
    
    if (m_consoleOutput) {
        std::cout << formatted.toStdString() << std::endl;
    }
    
    if (m_stream) {
        *m_stream << formatted << "\n";
        m_stream->flush();
    }
}

QString formatLogMessage(Logger::LogLevel level, const QString& message) {
    using enum Logger::LogLevel;
    QString levelStr;
    switch (level) {
        case Debug: levelStr = "DEBUG"; break;
        case Info: levelStr = "INFO"; break;
        case Warning: levelStr = "WARNING"; break;
        case Error: levelStr = "ERROR"; break;
        case Critical: levelStr = "CRITICAL"; break;
    }
    
    return QString("[%1] [%2] %3")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
        .arg(levelStr)
        .arg(message);
}


