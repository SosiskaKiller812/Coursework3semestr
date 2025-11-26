#include "bus.h"
#include "driver.h"
#include "seat.h"

Bus::Bus(const QString& licensePlate, const QString& model, 
         int capacity, BusType type)
    : m_licensePlate(licensePlate), m_model(model), 
      m_capacity(capacity), m_type(type) {
    // Инициализация мест
    for (int i = 1; i <= capacity; ++i) {
        Seat::SeatType seatType = (i % 2 == 0) ? Seat::SeatType::Window : Seat::SeatType::Aisle;
        m_seats.append(std::make_shared<Seat>(i, seatType, true));
    }
}

QString Bus::licensePlate() const {
    return m_licensePlate;
}

void Bus::setLicensePlate(const QString& plate) {
    m_licensePlate = plate;
}

QString Bus::model() const {
    return m_model;
}

void Bus::setModel(const QString& model) {
    m_model = model;
}

int Bus::capacity() const {
    return m_capacity;
}

void Bus::setCapacity(int capacity) {
    m_capacity = capacity;
}

Bus::BusType Bus::type() const {
    return m_type;
}

void Bus::setType(BusType type) {
    m_type = type;
}

void Bus::setDriver(std::shared_ptr<Driver> driver) {
    m_driver = driver;
}

std::shared_ptr<Driver> Bus::driver() const {
    return m_driver;
}

QVector<std::shared_ptr<Seat>>& Bus::seats() {
    return m_seats;
}

const QVector<std::shared_ptr<Seat>>& Bus::seats() const {
    return m_seats;
}

int Bus::availableSeats() const {
    int count = 0;
    for (const auto& seat : m_seats) {
        if (seat && seat->isAvailable()) {
            count++;
        }
    }
    return count;
}

bool Bus::isFull() const {
    return availableSeats() == 0;
}

QString Bus::serialize() const {
    QString typeStr = (m_type == BusType::Standard) ? "Standard" : 
                      (m_type == BusType::Luxury) ? "Luxury" : "Express";
    QString driverName = m_driver ? m_driver->name() : "";
    
    return QString("%1|%2|%3|%4|%5")
        .arg(m_licensePlate, m_model)
        .arg(m_capacity)
        .arg(typeStr)
        .arg(driverName);
}

void Bus::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() >= 5) {
        m_licensePlate = parts[0];
        m_model = parts[1];
        m_capacity = parts[2].toInt();
        
        using enum BusType;
        QString typeStr = parts[3];
        if (typeStr == "Luxury") {
            m_type = Luxury;
        } else if (typeStr == "Express") {
            m_type = Express;
        } else {
            m_type = Standard;
        }
        // Водитель должен быть восстановлен из базы данных
    }
}

bool Bus::operator==(const Bus& other) const {
    return m_licensePlate == other.m_licensePlate;
}

QString getBusInfo(const Bus& bus) {
    QString info = QString("Bus: %1\n").arg(bus.m_licensePlate);
    info += QString("Model: %1\n").arg(bus.m_model);
    info += QString("Capacity: %1\n").arg(bus.m_capacity);
    info += QString("Available seats: %1").arg(bus.availableSeats());
    return info;
}


