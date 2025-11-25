#include "schedule.h"
#include "route.h"
#include "trip.h"

Schedule::Schedule() {
}

void Schedule::addTrip(std::shared_ptr<Route> route, std::shared_ptr<Trip> trip) {
    if (route && trip) {
        m_schedule.append({route, trip});
    }
}

void Schedule::removeTrip(std::shared_ptr<Route> route, std::shared_ptr<Trip> trip) {
    m_schedule.erase(std::remove_if(m_schedule.begin(), m_schedule.end(),
        [&route, &trip](const auto& pair) {
            return pair.first == route && pair.second == trip;
        }), m_schedule.end());
}

QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> Schedule::getTripsByDate(const QDate& date) const {
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> result;
    for (const auto& pair : m_schedule) {
        if (pair.second && pair.second->departure().date() == date) {
            result.append(pair);
        }
    }
    return result;
}

QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> Schedule::getTripsByRoute(std::shared_ptr<Route> route) const {
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> result;
    for (const auto& pair : m_schedule) {
        if (pair.first == route) {
            result.append(pair);
        }
    }
    return result;
}

QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> Schedule::getTripsByTimeRange(
    const QDateTime& start, const QDateTime& end) const {
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> result;
    for (const auto& pair : m_schedule) {
        if (pair.second) {
            QDateTime departure = pair.second->departure();
            if (departure >= start && departure <= end) {
                result.append(pair);
            }
        }
    }
    return result;
}

QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> Schedule::getAllTrips() const {
    return m_schedule;
}

void Schedule::sortByDepartureTime() {
    std::sort(m_schedule.begin(), m_schedule.end(),
        [](const auto& a, const auto& b) {
            if (!a.second || !b.second) return false;
            return a.second->departure() < b.second->departure();
        });
}

void Schedule::sortByRoute() {
    std::sort(m_schedule.begin(), m_schedule.end(),
        [](const auto& a, const auto& b) {
            if (!a.first || !b.first) return false;
            return a.first->name() < b.first->name();
        });
}

Schedule::ScheduleIterator Schedule::begin() {
    return m_schedule.begin();
}

Schedule::ScheduleIterator Schedule::end() {
    return m_schedule.end();
}

Schedule::ConstScheduleIterator Schedule::begin() const {
    return m_schedule.begin();
}

Schedule::ConstScheduleIterator Schedule::end() const {
    return m_schedule.end();
}

Schedule& Schedule::operator+=(const std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>& item) {
    addTrip(item.first, item.second);
    return *this;
}

Schedule Schedule::operator+(const Schedule& other) const {
    Schedule result = *this;
    for (const auto& item : other.m_schedule) {
        result.addTrip(item.first, item.second);
    }
    return result;
}

QString getScheduleInfo(const Schedule& schedule) {
    QString info = QString("Schedule contains %1 trips\n").arg(schedule.m_schedule.size());
    for (const auto& pair : schedule.m_schedule) {
        if (pair.first && pair.second) {
            info += QString("Route: %1, Departure: %2\n")
                .arg(pair.first->name())
                .arg(pair.second->departure().toString("dd.MM.yyyy HH:mm"));
        }
    }
    return info;
}


