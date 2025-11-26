#include "pricecalculator.h"
#include "route.h"
#include "discount.h"
#include "passenger.h"
#include <algorithm>

PriceCalculator::PriceCalculator() = default;

double PriceCalculator::calculateBasePrice(std::shared_ptr<Route> route) {
    if (!route) {
        return 0.0;
    }
    return route->totalPrice();
}

double PriceCalculator::calculatePrice(std::shared_ptr<Route> route, 
                                        std::shared_ptr<Discount> discount,
                                        std::shared_ptr<Passenger> passenger) const {
    double basePrice = calculateBasePrice(route);
    
    if (discount && passenger) {
        double discountAmount = discount->calculateDiscount(basePrice, passenger);
        return basePrice - discountAmount;
    }
    
    return basePrice;
}

double PriceCalculator::calculateSegmentPrice(std::shared_ptr<Route> route, 
                                               int fromStop, int toStop) {
    if (!route || fromStop < 0 || toStop < 0 || fromStop >= toStop) {
        return 0.0;
    }
    
    double price = 0.0;
    auto stops = route->getAllStops();
    
    if (toStop > stops.size()) {
        return 0.0;
    }
    
    const int endIndex = std::min(toStop, static_cast<int>(stops.size()));
    for (int i = fromStop; i < endIndex; ++i) {
        if (stops[i]) {
            price += stops[i]->price;
        }
    }
    
    return price;
}

double PriceCalculator::calculatePriceWithMultipleDiscounts(std::shared_ptr<Route> route,
                                                             QVector<std::shared_ptr<Discount>> discounts,
                                                             std::shared_ptr<Passenger> passenger) const {
    double basePrice = calculateBasePrice(route);
    double maxDiscount = 0.0;
    
    for (const auto& discount : discounts) {
        if (discount && discount->isApplicable(passenger)) {
            double discountAmount = discount->calculateDiscount(basePrice, passenger);
            if (discountAmount > maxDiscount) {
                maxDiscount = discountAmount;
            }
        }
    }
    
    return basePrice - maxDiscount;
}

double PriceCalculator::operator()(std::shared_ptr<Route> route) const {
    return calculateBasePrice(route);
}

double PriceCalculator::operator()(std::shared_ptr<Route> route, std::shared_ptr<Discount> discount) const {
    return calculatePrice(route, discount, nullptr);
}


