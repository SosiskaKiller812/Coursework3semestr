#pragma once
#include "trip.h"
#include "route.h"
#include <QVector>
#include <QDateTime>
#include <QString>
#include <memory>
#include <algorithm>

// Контейнерный класс для расписания с итераторами
class Schedule {
public:
    Schedule();
    ~Schedule() = default;
    
    void addTrip(std::shared_ptr<Route> route, std::shared_ptr<Trip> trip);
    void removeTrip(std::shared_ptr<Route> route, std::shared_ptr<Trip> trip);
    
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> getTripsByDate(const QDate& date) const;
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> getTripsByRoute(std::shared_ptr<Route> route) const;
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> getTripsByTimeRange(
        const QDateTime& start, const QDateTime& end) const;
    
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> getAllTrips() const;
    
    void sortByDepartureTime();
    void sortByRoute();
    
    // Итераторы для STL-совместимости
    using ScheduleIterator = QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>>::iterator;
    using ConstScheduleIterator = QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>>::const_iterator;
    
    ScheduleIterator begin();
    ScheduleIterator end();
    ConstScheduleIterator begin() const;
    ConstScheduleIterator end() const;
    
    // Перегрузка операций
    Schedule& operator+=(const std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>& item);
    Schedule operator+(const Schedule& other) const;
    
    // Дружественная функция для вывода
    friend QString getScheduleInfo(const Schedule& schedule);
    
private:
    QVector<std::pair<std::shared_ptr<Route>, std::shared_ptr<Trip>>> m_schedule;
};

// Дружественная функция
QString getScheduleInfo(const Schedule& schedule);


