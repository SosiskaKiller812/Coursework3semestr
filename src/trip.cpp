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

bool Trip::operator!=(const Trip& other) const {
    return !(*this == other);
}

bool Trip::operator<(const Trip& other) const {
    return m_departure < other.m_departure;
}

bool Trip::operator>(const Trip& other) const {
    return m_departure > other.m_departure;
}

QDebug operator<<(QDebug debug, const Trip& trip) {
    debug.nospace() << "Trip(departure=" << trip.m_departure.toString("dd.MM.yyyy HH:mm") << ")";
    return debug;
}
