#pragma once
#include <stdexcept>
#include <QString>

class DatabaseException : public std::runtime_error {
public:
    DatabaseException(const QString& message)
        : std::runtime_error(message.toStdString()) {}
};
