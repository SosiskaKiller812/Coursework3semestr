#pragma once
#include "iserializable.h"
#include <QString>
#include <QDate>
#include <memory>

class Ticket;

class Passenger : public ISerializable {
public:
    explicit Passenger(const QString& name = "", const QString& passport = "", const QDate& birthDate = QDate());
    ~Passenger() override = default;

    QString name() const;
    void setName(const QString& name);
    
    QString passport() const;
    void setPassport(const QString& passport);
    
    QDate birthDate() const;
    void setBirthDate(const QDate& date);
    
    int age() const;
    
    // ISerializable interface
    QString serialize() const override;
    void deserialize(const QString& line) override;
    
    // Перегрузка операций
    bool operator==(const Passenger& other) const;
    
    // Дружественная функция для вывода
    friend class Ticket;
    
private:
    QString m_name;
    QString m_passport;
    QDate m_birthDate;
};


