#pragma once
#include <QString>

class ISerializable{
public:
    virtual ~ISerializable() = default;
    virtual QString serialize() const = 0;
    virtual void deserialize(const QString &line) = 0;
};
