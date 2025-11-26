#include "routefinder.h"
#include "company.h"
#include "trip.h"

RouteFinder::RouteFinder() = default;

QVector<std::shared_ptr<Route>> RouteFinder::findRoutes(const QString& fromCity, 
                                                          const QString& toCity,
                                                          const QVector<Company>& companies) const {
    QVector<std::shared_ptr<Route>> result;
    auto allRoutes = getAllRoutes(companies);
    
    for (const auto& route : allRoutes) {
        int fromPos = findCityPosition(route, fromCity);
        int toPos = findCityPosition(route, toCity);
        
        if (fromPos >= 0 && toPos >= 0 && fromPos < toPos) {
            result.append(route);
        }
    }
    
    return result;
}

QVector<QVector<std::shared_ptr<Route>>> RouteFinder::findRoutesWithTransfers(
    const QString& fromCity,
    const QString& toCity,
    const QVector<Company>& companies,
    int) const {
    QVector<QVector<std::shared_ptr<Route>>> result;
    // Упрощенная реализация поиска с пересадками
    auto directRoutes = findRoutes(fromCity, toCity, companies);
    for (const auto& route : directRoutes) {
        result.append({route});
    }
    return result;
}

QVector<std::shared_ptr<Route>> RouteFinder::findRoutesByDate(const QDate& date,
                                                                const QVector<Company>& companies) const {
    QVector<std::shared_ptr<Route>> result;
    auto allRoutes = getAllRoutes(companies);
    
    for (const auto& route : allRoutes) {
        for (const auto& trip : route->trips()) {
            if (trip && trip->departure().date() == date) {
                result.append(route);
                break;
            }
        }
    }
    
    return result;
}

QVector<std::shared_ptr<Route>> RouteFinder::findRoutesByPriceRange(double minPrice,
                                                                      double maxPrice,
                                                                      const QVector<Company>& companies) const {
    QVector<std::shared_ptr<Route>> result;
    auto allRoutes = getAllRoutes(companies);
    
    for (const auto& route : allRoutes) {
        double price = route->totalPrice();
        if (price >= minPrice && price <= maxPrice) {
            result.append(route);
        }
    }
    
    return result;
}

std::shared_ptr<Route> RouteFinder::findFastestRoute(const QString& fromCity,
                                                      const QString& toCity,
                                                      const QVector<Company>& companies) const {
    auto routes = findRoutes(fromCity, toCity, companies);
    if (routes.isEmpty()) {
        return nullptr;
    }
    
    std::shared_ptr<Route> fastest = routes.first();
    int minDuration = fastest->totalDuration();
    
    for (const auto& route : routes) {
        int duration = route->totalDuration();
        if (duration < minDuration) {
            minDuration = duration;
            fastest = route;
        }
    }
    
    return fastest;
}

std::shared_ptr<Route> RouteFinder::findCheapestRoute(const QString& fromCity,
                                                       const QString& toCity,
                                                       const QVector<Company>& companies) const {
    auto routes = findRoutes(fromCity, toCity, companies);
    if (routes.isEmpty()) {
        return nullptr;
    }
    
    std::shared_ptr<Route> cheapest = routes.first();
    double minPrice = cheapest->totalPrice();
    
    for (const auto& route : routes) {
        double price = route->totalPrice();
        if (price < minPrice) {
            minPrice = price;
            cheapest = route;
        }
    }
    
    return cheapest;
}

bool RouteFinder::hasDirectRoute(const QString& fromCity,
                                  const QString& toCity,
                                  const QVector<Company>& companies) const {
    return !findRoutes(fromCity, toCity, companies).isEmpty();
}

bool RouteFinder::routeContainsCity(std::shared_ptr<Route> route, const QString& city) const {
    if (!route) return false;
    
    auto stops = route->getAllStops();
    return std::ranges::any_of(stops, [&city](const auto& stop) {
        return stop && stop->city == city;
    });
}

int RouteFinder::findCityPosition(std::shared_ptr<Route> route, const QString& city) const {
    if (!route) return -1;
    
    auto stops = route->getAllStops();
    for (qsizetype i = 0; i < stops.size(); ++i) {
        if (stops[i] && stops[i]->city == city) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

QVector<std::shared_ptr<Route>> RouteFinder::getAllRoutes(const QVector<Company>& companies) const {
    QVector<std::shared_ptr<Route>> allRoutes;
    for (const auto& company : companies) {
        for (const auto& route : company.routes()) {
            allRoutes.append(route);
        }
    }
    return allRoutes;
}


