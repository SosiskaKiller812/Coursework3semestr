#pragma once
#include "stop.h"
#include <QString>
#include <QVector>
#include <QDateTime>
#include <memory>

class Trip;

class Route {
public:
    explicit Route(const QString &name = "");
    ~Route();
    Route(const Route& other);
    Route& operator=(const Route& other);

    QString name() const;

    void addStop(const QString &city, int durationMinutes, double price);
    void insertStop(int position, const QString &city, int durationMinutes, double price);
    void removeStop(int position);
    std::shared_ptr<Stop> getStop(int position) const;
    std::shared_ptr<Stop> firstStop() const;
    QVector<std::shared_ptr<Stop>> getAllStops() const;

    int totalDuration() const;
    double totalPrice() const;
    int totalStops() const;
    QString info() const;
    QString detailedInfo() const;

    void addTrip(const QDateTime &departure);
    QVector<std::shared_ptr<Trip>>& trips();
    const QVector<std::shared_ptr<Trip>>& trips() const;

private:
    QString m_name;
    std::shared_ptr<Stop> m_head = nullptr;
    std::shared_ptr<Stop> m_tail = nullptr;
    QVector<std::shared_ptr<Trip>> m_trips;
};
