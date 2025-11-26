#pragma once
#include "iserializable.h"
#include <QString>
#include <QVector>
#include <memory>

class Driver;
class Seat;

class Bus : public ISerializable {
public:
    enum class BusType {
        Standard,
        Luxury,
        Express
    };
    
    explicit Bus(const QString& licensePlate = "", const QString& model = "", 
                 int capacity = 0, BusType type = BusType::Standard);
    ~Bus() override = default;
    
    QString licensePlate() const;
    void setLicensePlate(const QString& plate);
    
    QString model() const;
    void setModel(const QString& model);
    
    int capacity() const;
    void setCapacity(int capacity);
    
    BusType type() const;
    void setType(BusType type);
    
    void setDriver(std::shared_ptr<Driver> driver);
    std::shared_ptr<Driver> driver() const;
    
    QVector<std::shared_ptr<Seat>>& seats();
    const QVector<std::shared_ptr<Seat>>& seats() const;
    
    int availableSeats() const;
    bool isFull() const;
    
    // ISerializable interface
    QString serialize() const override;
    void deserialize(const QString& line) override;
    
    // Перегрузка операций
    bool operator==(const Bus& other) const;
    
    // Дружественная функция для вывода
    friend QString getBusInfo(const Bus& bus);
    
private:
    QString m_licensePlate;
    QString m_model;
    int m_capacity;
    BusType m_type;
    std::shared_ptr<Driver> m_driver = nullptr;
    QVector<std::shared_ptr<Seat>> m_seats;
};

// Дружественная функция
QString getBusInfo(const Bus& bus);


