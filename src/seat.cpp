#include "seat.h"
#include "passenger.h"

Seat::Seat(int number, SeatType type, bool isAvailable)
    : m_number(number), m_type(type), m_isAvailable(isAvailable) {
}

int Seat::number() const {
    return m_number;
}

void Seat::setNumber(int number) {
    m_number = number;
}

Seat::SeatType Seat::type() const {
    return m_type;
}

void Seat::setType(SeatType type) {
    m_type = type;
}

bool Seat::isAvailable() const {
    return m_isAvailable && m_passenger == nullptr;
}

void Seat::setAvailable(bool available) {
    m_isAvailable = available;
    if (!available) {
        m_passenger = nullptr;
    }
}

void Seat::reserve(std::shared_ptr<Passenger> passenger) {
    if (m_isAvailable) {
        m_passenger = passenger;
        m_isAvailable = false;
    }
}

void Seat::release() {
    m_passenger = nullptr;
    m_isAvailable = true;
}

std::shared_ptr<Passenger> Seat::passenger() const {
    return m_passenger;
}

bool Seat::operator==(const Seat& other) const {
    return m_number == other.m_number;
}

auto Seat::operator<=>(const Seat& other) const {
    return m_number <=> other.m_number;
}

QString getSeatInfo(const Seat& seat) {
    QString typeStr;
    if (seat.m_type == Seat::SeatType::Window) {
        typeStr = "Window";
    } else if (seat.m_type == Seat::SeatType::Aisle) {
        typeStr = "Aisle";
    } else {
        typeStr = "ExtraLegroom";
    }
    QString status = seat.m_isAvailable ? "Available" : "Occupied";
    
    return QString("Seat #%1 (%2) - %3").arg(seat.m_number).arg(typeStr).arg(status);
}


