#pragma once
#include <stdexcept>
#include <QString>

class ValidationException : public std::runtime_error {
public:
    explicit ValidationException(const QString& message)
        : std::runtime_error(message.toStdString()) {}
};
