#pragma once
#include "route.h"
#include <QString>
#include <memory>

class Passenger;

class PriceCalculator {
public:
    PriceCalculator();
    ~PriceCalculator() = default;
    
    // Расчет базовой цены маршрута
    static double calculateBasePrice(std::shared_ptr<Route> route);
    
    // Расчет цены для сегмента маршрута
    static double calculateSegmentPrice(std::shared_ptr<Route> route, 
                                         int fromStop, int toStop);
    
    
    // Перегрузка операций
    double operator()(std::shared_ptr<Route> route) const;
    
private:
    static constexpr double BASE_PRICE_MULTIPLIER = 1.0;
};


