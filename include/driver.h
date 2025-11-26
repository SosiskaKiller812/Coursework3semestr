#pragma once
#include "iserializable.h"
#include <QString>
#include <QDate>
#include <QVector>
#include <memory>
#include <compare>

class Bus;

class Driver : public ISerializable {
public:
    explicit Driver(const QString& name = "", const QString& licenseNumber = "", 
                    const QDate& licenseExpiry = QDate(), int experienceYears = 0);
    ~Driver() override = default;
    
    QString name() const;
    void setName(const QString& name);
    
    QString licenseNumber() const;
    void setLicenseNumber(const QString& number);
    
    QDate licenseExpiry() const;
    void setLicenseExpiry(const QDate& date);
    
    int experienceYears() const;
    void setExperienceYears(int years);
    
    bool isLicenseValid() const;
    
    void assignBus(std::shared_ptr<Bus> bus);
    QVector<std::shared_ptr<Bus>> assignedBuses() const;
    
    // ISerializable interface
    QString serialize() const override;
    void deserialize(const QString& line) override;
    
    // Перегрузка операций
    bool operator==(const Driver& other) const;
    auto operator<=>(const Driver& other) const; // по опыту
    
    // Дружественная функция для вывода
    friend QString getDriverInfo(const Driver& driver);
    
private:
    QString m_name;
    QString m_licenseNumber;
    QDate m_licenseExpiry;
    int m_experienceYears;
    QVector<std::shared_ptr<Bus>> m_assignedBuses;
};

// Дружественная функция
QString getDriverInfo(const Driver& driver);


