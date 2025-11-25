#pragma once
#include "route.h"
#include "discount.h"
#include <QString>
#include <memory>

class Passenger;

class PriceCalculator {
public:
    PriceCalculator();
    ~PriceCalculator() = default;
    
    // Расчет базовой цены маршрута
    static double calculateBasePrice(std::shared_ptr<Route> route);
    
    // Расчет цены с учетом скидки
    double calculatePrice(std::shared_ptr<Route> route, 
                          std::shared_ptr<Discount> discount = nullptr,
                          std::shared_ptr<Passenger> passenger = nullptr) const;
    
    // Расчет цены для сегмента маршрута
    static double calculateSegmentPrice(std::shared_ptr<Route> route, 
                                         int fromStop, int toStop);
    
    // Применение нескольких скидок (выбирается максимальная)
    double calculatePriceWithMultipleDiscounts(std::shared_ptr<Route> route,
                                               QVector<std::shared_ptr<Discount>> discounts,
                                               std::shared_ptr<Passenger> passenger = nullptr) const;
    
    // Перегрузка операций
    double operator()(std::shared_ptr<Route> route) const;
    double operator()(std::shared_ptr<Route> route, std::shared_ptr<Discount> discount) const;
    
private:
    static constexpr double BASE_PRICE_MULTIPLIER = 1.0;
};


