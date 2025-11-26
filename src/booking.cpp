#include "booking.h"
#include "passenger.h"
#include "route.h"
#include "trip.h"
#include "seat.h"

Booking::Booking(int id, std::shared_ptr<Passenger> passenger,
                 std::shared_ptr<Route> route,
                 std::shared_ptr<Trip> trip,
                 std::shared_ptr<Seat> seat,
                 const QDateTime& bookingTime)
    : m_id(id), m_passenger(passenger), m_route(route), m_trip(trip),
      m_seat(seat), m_bookingTime(bookingTime) {
}

int Booking::id() const {
    return m_id;
}

void Booking::setId(int id) {
    m_id = id;
}

std::shared_ptr<Passenger> Booking::passenger() const {
    return m_passenger;
}

void Booking::setPassenger(std::shared_ptr<Passenger> passenger) {
    m_passenger = passenger;
}

std::shared_ptr<Route> Booking::route() const {
    return m_route;
}

void Booking::setRoute(std::shared_ptr<Route> route) {
    m_route = route;
}

std::shared_ptr<Trip> Booking::trip() const {
    return m_trip;
}

void Booking::setTrip(std::shared_ptr<Trip> trip) {
    m_trip = trip;
}

std::shared_ptr<Seat> Booking::seat() const {
    return m_seat;
}

void Booking::setSeat(std::shared_ptr<Seat> seat) {
    m_seat = seat;
}

QDateTime Booking::bookingTime() const {
    return m_bookingTime;
}

void Booking::setBookingTime(const QDateTime& time) {
    m_bookingTime = time;
}

Booking::BookingStatus Booking::status() const {
    return m_status;
}

void Booking::setStatus(BookingStatus status) {
    m_status = status;
}

bool Booking::isExpired(int expirationMinutes) const {
    if (m_status != BookingStatus::Pending) {
        return false;
    }
    return m_bookingTime.addSecs(expirationMinutes * 60) < QDateTime::currentDateTime();
}

void Booking::confirm() {
    m_status = BookingStatus::Confirmed;
}

void Booking::cancel() {
    m_status = BookingStatus::Cancelled;
    if (m_seat) {
        m_seat->release();
    }
}

bool Booking::operator==(const Booking& other) const {
    return m_id == other.m_id;
}

auto Booking::operator<=>(const Booking& other) const {
    return m_bookingTime <=> other.m_bookingTime;
}

QString getBookingInfo(const Booking& booking) {
    using enum Booking::BookingStatus;
    QString statusStr = (booking.m_status == Pending) ? "Pending" :
                        (booking.m_status == Confirmed) ? "Confirmed" :
                        (booking.m_status == Cancelled) ? "Cancelled" : "Expired";
    
    QString info = QString("Booking #%1\n").arg(booking.m_id);
    if (booking.m_passenger) {
        info += QString("Passenger: %1\n").arg(booking.m_passenger->name());
    }
    if (booking.m_route) {
        info += QString("Route: %1\n").arg(booking.m_route->name());
    }
    info += QString("Status: %1\n").arg(statusStr);
    info += QString("Time: %1").arg(booking.m_bookingTime.toString("dd.MM.yyyy HH:mm"));
    return info;
}


