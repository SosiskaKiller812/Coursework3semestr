#pragma once
#include <stdexcept>
#include <QString>

class RouteException : public std::runtime_error {
public:
    RouteException(const QString& message)
        : std::runtime_error(message.toStdString()) {}
};
