#pragma once
#include "route.h"
#include <QVector>
#include <QString>
#include <QDateTime>
#include <memory>
#include <algorithm>

class Company;

class RouteFinder {
public:
    RouteFinder();
    ~RouteFinder() = default;
    
    // Поиск маршрутов по городу отправления и назначения
    QVector<std::shared_ptr<Route>> findRoutes(const QString& fromCity, 
                                                const QString& toCity,
                                                const QVector<Company>& companies) const;
    
    // Поиск маршрутов с пересадками
    QVector<QVector<std::shared_ptr<Route>>> findRoutesWithTransfers(
        const QString& fromCity,
        const QString& toCity,
        const QVector<Company>& companies,
        int maxTransfers = 2) const;
    
    // Поиск маршрутов по дате отправления
    QVector<std::shared_ptr<Route>> findRoutesByDate(const QDate& date,
                                                      const QVector<Company>& companies) const;
    
    // Поиск маршрутов в ценовом диапазоне
    QVector<std::shared_ptr<Route>> findRoutesByPriceRange(double minPrice,
                                                            double maxPrice,
                                                            const QVector<Company>& companies) const;
    
    // Поиск самого быстрого маршрута
    std::shared_ptr<Route> findFastestRoute(const QString& fromCity,
                                             const QString& toCity,
                                             const QVector<Company>& companies) const;
    
    // Поиск самого дешевого маршрута
    std::shared_ptr<Route> findCheapestRoute(const QString& fromCity,
                                              const QString& toCity,
                                              const QVector<Company>& companies) const;
    
    // Проверка наличия прямого маршрута
    bool hasDirectRoute(const QString& fromCity,
                        const QString& toCity,
                        const QVector<Company>& companies) const;
    
private:
    bool routeContainsCity(std::shared_ptr<Route> route, const QString& city) const;
    int findCityPosition(std::shared_ptr<Route> route, const QString& city) const;
    QVector<std::shared_ptr<Route>> getAllRoutes(const QVector<Company>& companies) const;
};


