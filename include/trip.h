#pragma once
#include <QDateTime>
#include <QDebug>

class Route;

class Trip {
public:
    Trip(const QDateTime &departure = QDateTime());

    QDateTime departure() const;
    QDateTime arrival(const Route &route) const;

    // Перегрузка операций
    bool operator==(const Trip& other) const;
    bool operator!=(const Trip& other) const;
    bool operator<(const Trip& other) const;
    bool operator>(const Trip& other) const;

    // Дружественная функция для вывода в отладку
    friend QDebug operator<<(QDebug debug, const Trip& trip);

private:
    QDateTime m_departure;
};
