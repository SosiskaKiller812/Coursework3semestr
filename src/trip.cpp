#include "trip.h"
#include "route.h"

Trip::Trip(const QDateTime &departure) : m_departure(departure) {}

QDateTime Trip::departure() const { return m_departure; }

QDateTime Trip::arrival(const Route &route) const {
    return m_departure.addSecs(route.totalDuration() * 60);
}

bool Trip::operator==(const Trip& other) const {
    return m_departure == other.m_departure;
}

auto Trip::operator<=>(const Trip& other) const {
    return m_departure <=> other.m_departure;
}
