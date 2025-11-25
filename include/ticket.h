#pragma once
#include "iserializable.h"
#include "passenger.h"
#include "trip.h"
#include <QString>
#include <QDateTime>
#include <memory>

class Route;
class Discount;

class Ticket : public ISerializable {
public:
    Ticket(int id = 0, std::shared_ptr<Passenger> passenger = nullptr, 
           std::shared_ptr<Route> route = nullptr, 
           std::shared_ptr<Trip> trip = nullptr,
           int seatNumber = 0, double price = 0.0);
    ~Ticket() override = default;
    
    int id() const;
    void setId(int id);
    
    std::shared_ptr<Passenger> passenger() const;
    void setPassenger(std::shared_ptr<Passenger> passenger);
    
    std::shared_ptr<Route> route() const;
    void setRoute(std::shared_ptr<Route> route);
    
    std::shared_ptr<Trip> trip() const;
    void setTrip(std::shared_ptr<Trip> trip);
    
    int seatNumber() const;
    void setSeatNumber(int seatNumber);
    
    double price() const;
    void setPrice(double price);
    
    void applyDiscount(std::shared_ptr<Discount> discount);
    
    QString status() const;
    void setStatus(const QString& status);
    
    // ISerializable interface
    QString serialize() const override;
    void deserialize(const QString& line) override;
    
    // Перегрузка операций
    bool operator==(const Ticket& other) const;
    bool operator!=(const Ticket& other) const;
    bool operator<(const Ticket& other) const;
    
    // Дружественная функция для вывода
    friend QString formatTicketInfo(const Ticket& ticket);
    
private:
    int m_id;
    std::shared_ptr<Passenger> m_passenger;
    std::shared_ptr<Route> m_route;
    std::shared_ptr<Trip> m_trip;
    int m_seatNumber;
    double m_price;
    QString m_status; // "reserved", "paid", "cancelled"
};

// Дружественная функция
QString formatTicketInfo(const Ticket& ticket);


