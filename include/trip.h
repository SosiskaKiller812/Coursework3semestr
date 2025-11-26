#pragma once
#include <QDateTime>
#include <QDebug>
#include <compare>

class Route;

class Trip {
public:
    explicit Trip(const QDateTime &departure = QDateTime());

    QDateTime departure() const;
    QDateTime arrival(const Route &route) const;

    // Перегрузка операций
    bool operator==(const Trip& other) const;
    auto operator<=>(const Trip& other) const;

    // Дружественная функция для вывода в отладку
    friend QDebug operator<<(QDebug debug, const Trip& trip) {
        debug.nospace() << "Trip(departure=" << trip.m_departure.toString("dd.MM.yyyy HH:mm") << ")";
        return debug;
    }

private:
    QDateTime m_departure;
};
