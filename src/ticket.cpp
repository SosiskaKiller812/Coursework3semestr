#include "ticket.h"
#include "route.h"
#include "discount.h"

Ticket::Ticket(int id, std::shared_ptr<Passenger> passenger, 
               std::shared_ptr<Route> route, 
               std::shared_ptr<Trip> trip,
               int seatNumber, double price)
    : m_id(id), m_passenger(passenger), m_route(route), m_trip(trip),
      m_seatNumber(seatNumber), m_price(price) {
}

int Ticket::id() const {
    return m_id;
}

void Ticket::setId(int id) {
    m_id = id;
}

std::shared_ptr<Passenger> Ticket::passenger() const {
    return m_passenger;
}

void Ticket::setPassenger(std::shared_ptr<Passenger> passenger) {
    m_passenger = passenger;
}

std::shared_ptr<Route> Ticket::route() const {
    return m_route;
}

void Ticket::setRoute(std::shared_ptr<Route> route) {
    m_route = route;
}

std::shared_ptr<Trip> Ticket::trip() const {
    return m_trip;
}

void Ticket::setTrip(std::shared_ptr<Trip> trip) {
    m_trip = trip;
}

int Ticket::seatNumber() const {
    return m_seatNumber;
}

void Ticket::setSeatNumber(int seatNumber) {
    m_seatNumber = seatNumber;
}

double Ticket::price() const {
    return m_price;
}

void Ticket::setPrice(double price) {
    m_price = price;
}

void Ticket::applyDiscount(std::shared_ptr<Discount> discount) {
    if (discount && m_passenger) {
        double discountAmount = discount->calculateDiscount(m_price, m_passenger);
        m_price -= discountAmount;
    }
}

QString Ticket::status() const {
    return m_status;
}

void Ticket::setStatus(const QString& status) {
    m_status = status;
}

QString Ticket::serialize() const {
    QString routeName = m_route ? m_route->name() : "";
    QString passengerName = m_passenger ? m_passenger->name() : "";
    QString tripTime = m_trip ? m_trip->departure().toString(Qt::ISODate) : "";
    
    return QString("%1|%2|%3|%4|%5|%6|%7")
        .arg(m_id)
        .arg(passengerName)
        .arg(routeName)
        .arg(tripTime)
        .arg(m_seatNumber)
        .arg(m_price)
        .arg(m_status);
}

void Ticket::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() >= 7) {
        m_id = parts[0].toInt();
        // Остальные поля требуют восстановления объектов из базы данных
        m_seatNumber = parts[4].toInt();
        m_price = parts[5].toDouble();
        m_status = parts[6];
    }
}

bool Ticket::operator==(const Ticket& other) const {
    return m_id == other.m_id;
}

auto Ticket::operator<=>(const Ticket& other) const {
    return m_id <=> other.m_id;
}

QString formatTicketInfo(const Ticket& ticket) {
    QString info = QString("Ticket #%1\n").arg(ticket.m_id);
    if (ticket.m_passenger) {
        info += QString("Passenger: %1\n").arg(ticket.m_passenger->name());
    }
    if (ticket.m_route) {
        info += QString("Route: %1\n").arg(ticket.m_route->name());
    }
    info += QString("Seat: %1\n").arg(ticket.m_seatNumber);
    info += QString("Price: %1\n").arg(ticket.m_price);
    info += QString("Status: %1").arg(ticket.m_status);
    return info;
}


